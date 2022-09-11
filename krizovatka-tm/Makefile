all: build/arduino.avr.mega/krizovatka-tm.ino.elf

SOURCES=$(wildcard *.h *.ino *.cpp)

build/arduino.avr.mega/krizovatka-tm.ino.elf: $(SOURCES)
	arduino-cli compile --warnings all --fqbn arduino:avr:mega -v --optimize-for-debug krizovatka-tm.ino

clean:
	rm -r build

flash:
	arduino-cli upload --fqbn arduino:avr:mega -v krizovatka-tm.ino -p /dev/ttyACM0

.PHONY: all clean flash
