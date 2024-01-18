from requests import Response, get, post, delete
from abc import ABC, abstractmethod
import locale
import logging
import platform
import json
import urllib3


class BaseComManager(ABC):

    def __init__(self, server_url: str, server_port: int, allow_insecure=False):
        self.token = None
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

    @staticmethod
    def set_logging_level(level: int):
        logging.basicConfig(level=level, format='%(asctime)s\t[%(levelname)s]\t%(module)s\t%(message)s')

    def do_get(self, endpoint: str, params: dict | None = None, extra_headers: dict | None = None) -> Response:
        headers = self._get_request_headers()
        if extra_headers:
            headers |= extra_headers
        logging.debug(msg='GET ' + endpoint + ', ' + str(params))
        return get(url=self.server_url + endpoint, params=params, headers=headers, verify=self.verify_ssl)

    def do_post(self, endpoint: str, data: str, extra_headers: dict | None = None) -> Response:
        headers = self._get_request_headers()
        if extra_headers:
            headers |= extra_headers
        logging.debug(msg='POST ' + endpoint + ', ' + json.dumps(data))
        return post(url=self.server_url + endpoint, json=data, headers=headers, verify=self.verify_ssl)

    def do_delete(self, endpoint: str, id_to_delete: int, extra_headers: dict | None = None) -> Response:
        headers = self._get_request_headers()
        if extra_headers:
            headers |= extra_headers
        logging.debug(msg='DELETE ' + endpoint + ', ' + str(id_to_delete))
        return delete(url=self.server_url + endpoint, params={'id': id_to_delete}, headers=headers,
                      verify=self.verify_ssl)

    def _get_request_headers(self) -> dict:
        headers = {
            'Accept-Language': self.language,
            'X-OS-Name': self.os_name,
            'X-OS_Version': self.os_version,
            'X-Client-Name': self.client_name,
            'X-Client-Version': self.client_version
        }

        if self.token:
            headers['Authorization'] = 'OpenTera ' + self.token

        return headers

    @abstractmethod
    def login_with_username(self, username: str, password: str) -> Response | None:
        pass

    @abstractmethod
    def login_with_token(self, token: str) -> Response | None:
        pass
