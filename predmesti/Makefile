all: build/arduino.avr.mega/predmesti.ino.elf

SOURCES=$(wildcard *.h *.ino *.cpp)

build/arduino.avr.mega/predmesti.ino.elf: $(SOURCES)
	arduino-cli compile --warnings all --fqbn arduino:avr:mega -v --optimize-for-debug predmesti.ino

clean:
	rm -r build

flash:
	arduino-cli upload --fqbn arduino:avr:mega -v predmesti.ino -p /dev/ttyACM0

.PHONY: all clean flash
