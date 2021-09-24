#!/usr/bin/env python3

import serial


if __name__ == '__main__':

    with serial.Serial('/dev/ttyUSB0', 9600, rtscts=False, xonxoff=False) as ser:
        print('Started...')
        while True:
            line = ser.readline().decode('utf-8').strip()
            print(line)
