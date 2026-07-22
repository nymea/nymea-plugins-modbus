#!/usr/bin/env python3

import json
import pathlib
import subprocess
import sys
import tempfile
import unittest


TOOLS_DIR = pathlib.Path(__file__).resolve().parents[1]
GENERATOR = TOOLS_DIR / "generate-connection.py"


class GenerateConnectionTest(unittest.TestCase):
    def generate(self, document, expect_success=True):
        with tempfile.TemporaryDirectory() as directory:
            directory = pathlib.Path(directory)
            source = directory / "registers.json"
            source.write_text(json.dumps(document), encoding="utf-8")
            output = directory / "generated"
            output.mkdir()
            result = subprocess.run(
                [sys.executable, str(GENERATOR), "-j", str(source), "-o", str(output)],
                text=True, capture_output=True, check=False)
            self.assertEqual(result.returncode == 0, expect_success,
                             result.stdout + result.stderr)
            files = {}
            if result.returncode == 0:
                for path in output.iterdir():
                    files[path.name] = path.read_text(encoding="utf-8")
            return result, files

    @staticmethod
    def base_document():
        return {
            "className": "FlagTest",
            "protocol": "BOTH",
            "checkReachableRegister": "status",
            "registers": [{
                "id": "status", "address": 1, "size": 1,
                "type": "uint16", "registerType": "holdingRegister",
                "access": "RW", "description": "Status", "readSchedule": "update",
            }],
        }

    def test_uint16_flags_generate_typed_api_and_conversion(self):
        document = self.base_document()
        document["flags"] = [{
            "name": "Status",
            "values": [{"key": "Ready", "value": 1},
                       {"key": "Remote", "value": 2}],
        }]
        document["registers"][0]["flags"] = "Status"
        _, files = self.generate(document)
        header = files["flagtestmodbustcpconnection.h"]
        source = files["flagtestmodbustcpconnection.cpp"]
        self.assertIn("Q_DECLARE_FLAGS(StatusFlags, StatusFlag)", header)
        self.assertIn("Q_FLAG(StatusFlags)", header)
        self.assertIn("Q_DECLARE_OPERATORS_FOR_FLAGS", header)
        self.assertIn("StatusFlags status() const", header)
        self.assertIn("void statusChanged(StatusFlags status)", header)
        self.assertIn("StatusFlags m_status", header)
        self.assertIn("static_cast<StatusFlags>(ModbusDataUtils::convertToUInt16(values))", source)
        self.assertIn("convertFromUInt16(static_cast<quint16>(status))", source)

    def test_invalid_flags_are_rejected(self):
        cases = [
            ([{"key": "Zero", "value": 0}], "uint16", None),
            ([{"key": "Combined", "value": 3}], "uint16", None),
            ([{"key": "A", "value": 1}, {"key": "B", "value": 1}], "uint16", None),
            ([{"key": "A", "value": 1}], "uint32", None),
            ([{"key": "A", "value": 1}], "uint16", "OtherEnum"),
        ]
        for values, register_type, enum_name in cases:
            with self.subTest(values=values, register_type=register_type, enum_name=enum_name):
                document = self.base_document()
                document["flags"] = [{"name": "Status", "values": values}]
                document["registers"][0]["flags"] = "Status"
                document["registers"][0]["type"] = register_type
                if enum_name:
                    document["registers"][0]["enum"] = enum_name
                self.generate(document, expect_success=False)

        document = self.base_document()
        document["registers"][0]["flags"] = "MissingFlags"
        self.generate(document, expect_success=False)

    def test_sensitive_values_are_redacted_but_existing_types_are_unchanged(self):
        document = self.base_document()
        document["protocol"] = "TCP"
        document["enums"] = [{
            "name": "Mode", "values": [{"key": "Off", "value": 0}]
        }]
        document["registers"][0].update(
            type="raw", size=2, sensitive=True)
        document["registers"].append({
            "id": "mode", "address": 3, "size": 1, "type": "uint16",
            "registerType": "holdingRegister", "access": "RO",
            "description": "Mode", "enum": "Mode",
        })
        _, files = self.generate(document)
        header = files["flagtestmodbustcpconnection.h"]
        source = files["flagtestmodbustcpconnection.cpp"]
        self.assertIn("[REDACTED]", source)
        self.assertNotIn('<< values;', source[source.index("processStatusRegisterValues"):source.index("processModeRegisterValues")])
        self.assertIn("Mode mode() const", header)
        self.assertIn("QVector<quint16> status() const", header)


if __name__ == "__main__":
    unittest.main()
