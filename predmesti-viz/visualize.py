#!/usr/bin/env python3

import xml.etree.ElementTree as ET
import serial
import re

# ".//{http://www.w3.org/2000/svg}path[@id='FH10']"

def darken_probes(root):
    items = root.findall(".//{http://www.w3.org/2000/svg}path")
    for item in items:
        if item.attrib['id'].startswith('FH'):
            item.set('style', 'fill:#00ff00;fill-opacity:0.3;')



if __name__ == '__main__':
    tree = ET.parse('../predmesti/predmesti.svg')
    root = tree.getroot()
    darken_probes(root)

    with serial.Serial('/dev/ttyUSB0', 9600, timeout=1) as ser:
        line = ser.readline()
        path = re.search(r'^\d*: Path (.*?): (\d*), (\d*)$')
        magnet = re.search(r'^\d*: Magnet (.*?): (\d*)$')

        if path:
            pid, pstate, pvehicle = path.groups()
            item = root.find(f".//{http://www.w3.org/2000/svg}path[@id={pid}]")
            COLORS = {
                0: '#555555'
                1: '#0000ff'
                2: '#ff0000'
                3: '#00ffff'
                4: '#ffff00'
            }
            item.set('style', f'fill:{COLORS[pstate]};')
            tree.write(open('test.svg', 'wb'))

        elif magnet:
            mid, mstate = path.groups()
            item = root.find(f".//{http://www.w3.org/2000/svg}path[@id={mid}]")
            color = '#ff0000' if mstate == 0 else '#00ff00';
            item.set('style', f'fill:{color};')
            tree.write(open('test.svg', 'wb'))

        else:
            print(line)
