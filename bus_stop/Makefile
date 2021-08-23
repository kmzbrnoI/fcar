all: build/arduino.avr.nano/bus_stop.ino.elf

SOURCES=$(wildcard *.h *.ino *.cpp)

build/arduino.avr.nano/bus_stop.ino.elf: $(SOURCES)
	arduino-cli compile --warnings all --fqbn arduino:avr:nano -v --optimize-for-debug bus_stop.ino

clean:
	rm -r build

flash:
	arduino-cli upload --fqbn arduino:avr:nano -v bus_stop.ino -p /dev/ttyACM0

.PHONY: all clean flash
