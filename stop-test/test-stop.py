#!/usr/bin/env python3

"""
Faller Car System hall probe & stop test

Usage:
  test-stop.py [options]

Options:
  -s <servername>    Specify MTB Daemon server address [default: localhost]
  -p <port>          Specify MTB Daemon port [default: 3841]
  -m <module>        MTB module address [default: 1]
  -a <hall_port>     Hall probe input port [default: 0]
  -g <magnet_port>   Magnet output port [default: 0]
  -v                 Verbose
  -h --help          Show this screen.
"""

from docopt import docopt
from mtbdaemonif import MtbDaemonIFace, ModuleSubscription, EMtbDaemonTimeout
import time


def wait_for_hall_activation(daemon: MtbDaemonIFace, module_addr: int, port: int) -> None:
    module = daemon.request_response({'command': 'module', 'address': module_addr, 'state': True})['module']
    assert module['type'].startswith('MTB-UNI')
    uni = module[module['type']]
    assert module['state'] == 'active', f'Module {module_addr} invalid state: {module["state"]}'
    input_state = uni['state']['inputs']['full'][port]
    while input_state:  # wait until input is inactive
        print(f'Waiting for hall probe IN {module_addr}:{port}=0 ...')
        try:
            event = daemon.expect_event('module_inputs_changed', timeout=5)
            input_state = event['module_inputs_changed']['inputs']['full'][port]
        except EMtbDaemonTimeout:
            pass

    # Wait until input goes from inactive to active state
    while not input_state:
        print(f'Waiting for hall probe IN {module_addr}:{port}=1 ...')
        try:
            event = daemon.expect_event('module_inputs_changed', timeout=5)
            input_state = event['module_inputs_changed']['inputs']['full'][port]
        except EMtbDaemonTimeout:
            pass


def main():
    args = docopt(__doc__)
    MODULE = int(args['-m'])
    HALL_PORT = int(args['-a'])
    MAGNET_PORT = int(args['-g'])

    with MtbDaemonIFace(args['-s'], int(args['-p'])) as daemon, \
            ModuleSubscription(daemon, [MODULE]):
        while True:
            wait_for_hall_activation(daemon, MODULE, HALL_PORT)
            print(f'Setting magnet OUT {MODULE}:{MAGNET_PORT}=1 ...')
            daemon.set_single_output(MODULE, MAGNET_PORT, 1)
            print('OK')
            time.sleep(5)
            print(f'Setting magnet OUT {MODULE}:{MAGNET_PORT}=0 ...')
            daemon.set_single_output(MODULE, MAGNET_PORT, 0)
            print('OK')


if __name__ == '__main__':
    main()
