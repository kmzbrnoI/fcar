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

SVG_ROOT = './/{http://www.w3.org/2000/svg}'


if __name__ == '__main__':
    tree = ET.parse('../predmesti/predmesti.svg')
    root = tree.getroot()
    darken_probes(root)

    with serial.Serial('/dev/ttyACM0', 9600) as ser:
        while True:
            line = ser.readline().decode('utf-8').strip()
            path = re.search(r'^\d*: Path (.*?): (\d*), (-?\d*)$', line)
            magnet = re.search(r'^\d*: Magnet (.*?): (\d*)$', line)

            if path:
                pid, pstate, pvehicle = path.groups()
                item = root.find(f'{SVG_ROOT}path[@id="{pid}"]')
                if item is not None:
                    COLORS = {
                        0: '#555555',
                        1: '#0000ff',
                        2: '#ff0000',
                        3: '#00ffff',
                        4: '#ffff00',
                    }
                    item.set('style', f'fill:{COLORS[pstate]};')
                    tree.write(open('test.svg', 'wb'))
                else:
                    print(f'ERROR: unknown path: {pid}!')

            elif magnet:
                mid, mstate = [magnet.group(1), int(magnet.group(2))]
                item = root.find(f'{SVG_ROOT}path[@id="{mid}"]')
                if item is not None:
                    color = '#ff0000' if mstate == 0 else '#00ff00';
                    item.set('style', f'fill:{color};')
                    tree.write(open('test.svg', 'wb'))
                else:
                    print(f'ERROR: unknown magnet: {mid}!')

            else:
                print(line)
