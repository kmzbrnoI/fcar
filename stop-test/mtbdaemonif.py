"""
Interface to MTB Daemon JSON TCP server
"""

from typing import Dict, Any, Self, List
from types import TracebackType
import socket
import logging
import json
import select
import time


DEFAULT_HOST = '127.0.0.1'
DEFAULT_PORT = 3841


class EMtbDaemon(Exception):
    pass


class EMtbDaemonTimeout(EMtbDaemon):
    pass


class MtbDaemonIFace:
    def __init__(self, host: str = DEFAULT_HOST, port: int = DEFAULT_PORT):
        self.host = host
        self.port = port
        self.buf_received = ''
        self.id: int = 0
        self.connect()

    def connect(self) -> None:
        logging.info(f'Connecting to {self.host}:{self.port} ...')
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        logging.info('Connected')

    def disconnect(self) -> None:
        logging.info('Disconnecting from mtb-daemon ...')
        self.sock.close()
        logging.info('Disconnected')

    def __enter__(self) -> Self:
        return self

    def __exit__(self, exception_type: Any, exception_value: Any,
                 exception_traceback: TracebackType) -> None:
        self.disconnect()

    def send_message(self, data: Dict[str, Any]) -> None:
        logging.debug(f'Send: {data}')
        self.sock.send((json.dumps(data)+'\n').encode('utf-8'))

    def send_request(self, data: Dict[str, Any]) -> None:
        data['type'] = 'request'
        self.send_message(data)

    def expect_message(self, command: str, timeout: float = 1) -> Dict[str, Any]:
        start = time.time()
        while True:
            readable, _, _ = select.select([self.sock], [], [], timeout)
            if self.sock in readable:
                self.buf_received += self.sock.recv(0xFFFF).decode('utf-8')
                # logging.debug(f'{self.buf_received=}')
                while '\n' in self.buf_received:
                    offset = self.buf_received.find('\n')
                    message = json.loads(self.buf_received[:offset])
                    self.buf_received = self.buf_received[offset+1:]

                    logging.debug(f'Received: {message}')
                    assert isinstance(message, dict)

                    # Strictly forbid receiving of other commands and events,
                    # allow only empty message.
                    if message == {}:
                        continue

                    assert 'command' in message
                    if message['command'] == command:
                        return message

            if (time.time() - start) >= timeout:
                raise EMtbDaemonTimeout(
                    f'Timeout waiting for mtb-daemon"s response to {command} command!'
                )

    def expect_response(self, command: str, timeout: float = 1, ok: bool = True) -> Dict[str, Any]:
        response = self.expect_message(command, timeout)
        assert 'status' in response
        if ok:
            assert response['status'] == 'ok', 'Error: ' + str(response.get('error', 'unknown'))
        assert 'type' in response
        assert response['type'] == 'response'
        return response

    def request_response(self, request: Dict[str, Any], timeout: float = 1,
                         ok: bool = True) -> Dict[str, Any]:
        assert 'command' in request
        if 'id' not in request:
            request['id'] = self.id
            self.id += 1
        self.send_request(request)
        response = self.expect_response(request['command'], timeout, ok)
        assert 'id' in response
        assert response['id'] == request['id']
        return response

    def expect_event(self, command: str, timeout: float = 1) -> Dict[str, Any]:
        response = self.expect_message(command, timeout)
        assert 'type' in response
        assert response['type'] == 'event'
        return response

    def expect_no_message(self, timeout: float = 0.5) -> None:
        start = time.time()
        while True:
            readable, _, _ = select.select([self.sock], [], [], timeout)
            if self.sock in readable:
                self.buf_received += self.sock.recv(0xFFFF).decode('utf-8')
                while '\n' in self.buf_received:
                    offset = self.buf_received.find('\n')
                    message = json.loads(self.buf_received[:offset])
                    self.buf_received = self.buf_received[offset+1:]
                    logging.debug(f'Received: {message}')
                    assert isinstance(message, dict)
                    assert message == {}, f'Nonempty message received: {message}!'

            if (time.time() - start) >= timeout:
                return

    def set_single_output(self, addr: int, output: int, value: int) -> None:
        self.request_response({
            'command': 'module_set_outputs',
            'address': addr,
            'outputs': {str(output): {'type': 'plain', 'value': value}}
        })


class MtbDaemonError:
    INVALID_JSON = 1000
    MODULE_INVALID_ADDR = 1100
    MODULE_INVALID_PORT = 1101
    MODULE_FAILED = 1102
    INVALID_SPEED = 1105
    INVALID_DV = 1106
    MODULE_ACTIVE = 1107
    FILE_CANNOT_ACCESS = 1010
    MODULE_ALREADY_WRITING = 1110
    UNKNOWN_COMMAND = 1020

    DEVICE_DISCONNECTED = 2004
    ALREADY_STARTED = 2012

    MODULE_UPGRADING_FW = 3110
    MODULE_IN_BOOTLOADER = 3111
    MODULE_CONFIG_SETTING = 3112
    MODULE_REBOOTING = 3113
    MODULE_FWUPGD_ERROR = 3114

    MODULE_UNKNOWN_COMMAND = 0x1001
    MODULE_UNSUPPORTED_COMMAND = 0x1002
    MODULE_BAD_ADDRESS = 0x1003
    SERIAL_PORT_CLOSED = 0x1010
    USB_NO_RESPONSE = 0x1011
    BUS_NO_RESPONSE = 0x1012


class ModuleSubscription:
    def __init__(self, daemon: MtbDaemonIFace, addrs: List[int]):
        self.daemon: MtbDaemonIFace = daemon
        self.addrs: List[int] = addrs

    def __enter__(self) -> Self:
        response = self.daemon.request_response({
            'command': 'module_subscribe',
            'addresses': self.addrs,
        })
        assert response['addresses'] == self.addrs
        return self

    def __exit__(self, exception_type: Any, exception_value: Any,
                 exception_traceback: TracebackType) -> None:
        self.daemon.request_response({
            'command': 'module_unsubscribe',
            'addresses': self.addrs,
        })


class TopoSubscription:
    def __init__(self, daemon: MtbDaemonIFace):
        self.daemon: MtbDaemonIFace = daemon

    def __enter__(self) -> Self:
        self.daemon.request_response({'command': 'topology_subscribe'})
        return self

    def __exit__(self, exception_type: Any, exception_value: Any,
                 exception_traceback: TracebackType) -> None:
        self.daemon.request_response({'command': 'topology_unsubscribe'})
