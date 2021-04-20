#ifndef INEPROMODBUSREGISTER_H
#define INEPROMODBUSREGISTER_H

enum InputRegisters {
    Phase1ToNeutralVolts   = 1,
    Phase2ToNeutralVolts   = 3,
    Phase3ToNeutralVolts   = 5,
    Phase1Current          = 7,
    Phase2Current          = 9,
    Phase3Current          = 11,
    Phase1Power            = 13,
    Phase2Power            = 15,
    Phase3Power            = 17,
    Phase1ApparentPower    = 19,
    Phase2ApparentPower    = 21,
    Phase3ApparentPower    = 23,
    Phase1ReactivePower    = 25,
    Phase2ReactivePower    = 27,
    Phase3ReactivePower    = 29,
    Phase1PowerFactor      = 31,
    Phase2PowerFactor      = 33,
    Phase3PowerFactor      = 35,
    Phase1Angle            = 37,
    Phase2Angle            = 39,
    Phase3Angle            = 41,
    Frequency            = 71,
    ImportActiveEnergy   = 73,
    ExportActiveEnergy   = 75,
    ImportReactiveEnergy = 77,
    ExportReactiveEnergy = 79,
    TotalActiveEnergy    = 343,
    TotalReactiveEnergy  = 345
};

enum HoldingRegisters {
    RelayPulseWidth     = 13,
    NetworkParityStop   = 19,
    NetworkPortNode     = 21,
    NetworkBaudRate     = 29,
    Pulse1Output        = 87,
    Pulse1Constant      = 63761,
    MeasurementMode     = 63776
};

#endif // BGETECHMODBUSREGISTER_H
