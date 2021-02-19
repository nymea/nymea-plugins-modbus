#!/bin/bash

if ! command -v suns &> /dev/null
then
	echo "suns could not be found"
	echo "... installing"
	sudo apt update
	sudo apt install libmodbus-dev flex bison git

	if [ ! -d "sunspec"]; then
		git clone https://github.com/Boernsman/sunspec.git
	fi
	cd  ./sunspec/src
	make
	sudo make install
fi

echo "Startin sunspec test server"
sudo suns -s -vvvv -m models/test/composite_superdevice.model
