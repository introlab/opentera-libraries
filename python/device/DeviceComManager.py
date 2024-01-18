from requests import Response
import logging
import device.DeviceAPI

from common.BaseComManager import BaseComManager


class DeviceComManager(BaseComManager):

    def __init__(self, server_url: str, server_port: int, allow_insecure = False):
        super().__init__(server_url=server_url, server_port=server_port, allow_insecure=allow_insecure)

    def register_device(self, device_name: str, id_device_type: int, id_device_subtype: int | None = None) -> Response:
        params = {'device_info': {'device_name': device_name, 'id_device_type': id_device_type}}
        if id_device_subtype:
            params['device_info']['id_device_subtype'] = id_device_subtype

        return self.do_post(device.DeviceAPI.ENDPOINT_DEVICE_REGISTER, params)

    def login_with_token(self, token: str) -> Response | None:
        params = {'token': token}
        response = self.do_get(device.DeviceAPI.ENDPOINT_DEVICE_LOGIN, params)
        if response.status_code == 200:
            self.token = token
        else:
            self.token = None

        return response

    def login_with_username(self, username: str, password: str) -> Response | None:
        logging.warning(msg='Device can\'t log with username - password')
        return None
