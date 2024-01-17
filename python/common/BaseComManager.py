from requests import Response, get, post, delete
from abc import ABC, abstractmethod
import locale
import platform


class BaseComManager(ABC):

    def __init__(self, server_url: str, server_port: int):
        self.token = None
        self.server_url = 'https://' + server_url + ":" + str(server_port)
        self.language = locale.getlocale()[0]  # Default language = system
        self.os_name = platform.system() + ' ' + platform.release()
        self.os_version = platform.version()
        self.client_name = 'OpenTera Libraries'
        self.client_version = '1.0.0'   # TODO: Use real version number of the libraries

    def set_language(self, language: str):
        self.language = language

    def set_client_name(self, name: str):
        self.client_name = name

    def set_client_version(self, version: str):
        self.client_version = version

    def do_get(self, endpoint: str, params: dict) -> Response:
        headers = self._get_request_headers()
        return get(url=self.server_url + endpoint, params=params, headers=headers)

    def do_post(self, endpoint: str, data: str) -> Response:
        headers = self._get_request_headers()
        return post(url=self.server_url + endpoint, json=data, headers=headers)

    def do_delete(self, endpoint: str, id_to_delete: int) -> Response:
        headers = self._get_request_headers()
        return delete(url=self.server_url + endpoint, params={'id': id_to_delete}, headers=headers)

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
    def refresh_token(self):
        pass

    @abstractmethod
    def login_with_username(self, username: str, password: str):
        pass

    @abstractmethod
    def login_with_token(self, token: str):
        pass
