from requests import Response
import logging
import opentera_libraries.device.DeviceAPI as DeviceAPI

from opentera_libraries.common.BaseComManager import BaseComManager


class DeviceComManager(BaseComManager):

    def __init__(self, server_url: str, server_port: int, allow_insecure=False):
        super().__init__(server_url=server_url, server_port=server_port, allow_insecure=allow_insecure)

    def register_device(self, server_key: str, device_name: str, device_type_key: str,
                        device_subtype_name: str | None = None) -> Response:
        params = {'name': device_name, 'key': server_key, 'type_key': device_type_key}
        if device_subtype_name:
            params['subtype_name'] = device_type_key

        return self.do_get(DeviceAPI.ENDPOINT_DEVICE_REGISTER, params)

    def login_with_token(self, token: str) -> Response | None:
        params = {'token': token}
        response = self.do_get(DeviceAPI.ENDPOINT_DEVICE_LOGIN, params)
        if response.status_code == 200:
            self.token = token
        else:
            self.token = None

        return response

    def login_with_username(self, username: str, password: str) -> Response | None:
        logging.warning(msg='Device can\'t log with username - password')
        return None
