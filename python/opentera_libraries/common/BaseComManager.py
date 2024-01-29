from requests import Response, Session
from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
from abc import ABC, abstractmethod

import os
import json
import locale
import logging
import platform
import urllib3


class BaseComManager(ABC):

    def __init__(self, server_url: str, server_port: int, allow_insecure=False):
        self._token = None
        self.server_url = 'https://' + server_url + ":" + str(server_port)
        self.language = locale.getlocale()[0]  # Default language = system
        self.os_name = platform.system() + ' ' + platform.release()
        self.os_version = platform.version()
        self.client_name = 'OpenTera Libraries'
        self.client_version = '1.0.0'   # TODO: Use real version number of the libraries
        self.verify_ssl = not allow_insecure
        if not self.verify_ssl:
            urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

        # Init logger
        self.set_logging_level(level=logging.CRITICAL)

        # Init request session
        self._session = Session()
        self._set_session_headers()

    @property
    def token(self):
        return self._token

    @token.setter
    def token(self, value):
        self._token = value
        # Update headers in request session
        self._set_session_headers()

    @staticmethod
    def set_logging_level(level: int):
        logging.basicConfig(level=level, format='%(asctime)s\t[%(levelname)s]\t%(module)s\t%(message)s')

    def do_get(self, endpoint: str, params: dict | None = None, extra_headers: dict | None = None) -> Response:
        logging.debug(msg='GET ' + endpoint + ', ' + str(params))
        if extra_headers:
            headers = self._session.headers | extra_headers
            return self._session.get(url=self.server_url + endpoint, params=params, verify=self.verify_ssl,
                                     headers=headers)
        else:
            return self._session.get(url=self.server_url + endpoint, params=params, verify=self.verify_ssl)

    def do_post(self, endpoint: str, data: dict | str, extra_headers: dict | None = None) -> Response:
        logging.debug(msg='POST ' + endpoint + ', ' + str(data))
        if extra_headers:
            headers = self._session.headers | extra_headers
            return self._session.post(url=self.server_url + endpoint, json=data, headers=headers, verify=self.verify_ssl)
        else:
            return self._session.post(url=self.server_url + endpoint, json=data, verify=self.verify_ssl)

    def do_delete(self, endpoint: str, id_to_delete: int, extra_headers: dict | None = None) -> Response:
        logging.debug(msg='DELETE ' + endpoint + ', ' + str(id_to_delete))
        if extra_headers:
            headers = self._session.headers | extra_headers
            return self._session.delete(url=self.server_url + endpoint, params={'id': id_to_delete}, headers=headers,
                                        verify=self.verify_ssl)
        else:
            return self._session.delete(url=self.server_url + endpoint, params={'id': id_to_delete},
                                        verify=self.verify_ssl)

    def do_post_file(self, endpoint: str, file_infos: dict, file_path: str, callback_func=None) -> Response:
        logging.debug(msg='POST FILE ' + file_path + ' TO ' + endpoint)

        file_length = os.stat(file_path)
        fileobj = open(file_path, 'rb')
        filename = os.path.basename(file_path)

        encoder = MultipartEncoder(
            fields={'file': (filename,
                             fileobj, 'application/octet-stream', {'Content-Length': str(file_length.st_size)}),
                    'file_asset': (None, json.dumps(file_infos), 'application/json')}
        )

        monitor = MultipartEncoderMonitor(encoder, callback_func)

        extra_headers = {
            'Content-Type': monitor.content_type
        }

        headers = self._session.headers | extra_headers
        return self._session.post(url=self.server_url + endpoint, data=monitor, headers=headers,
                                  verify=self.verify_ssl)

    def _set_session_headers(self):
        headers = {
            'Accept-Language': self.language,
            'X-OS-Name': self.os_name,
            'X-OS-Version': self.os_version,
            'X-Client-Name': self.client_name,
            'X-Client-Version': self.client_version
        }

        if self._token:
            headers['Authorization'] = 'OpenTera ' + self._token

        self._session.headers = headers

    @abstractmethod
    def login_with_username(self, username: str, password: str) -> Response | None:
        pass

    @abstractmethod
    def login_with_token(self, token: str) -> Response | None:
        pass
