from requests import Response
from requests.auth import _basic_auth_str
import logging
import opentera_libraries.user.UserAPI as UserAPI

from opentera_libraries.common.BaseComManager import BaseComManager


class UserComManager(BaseComManager):

    def __init__(self, server_url: str, server_port: int, allow_insecure=False):
        super().__init__(server_url=server_url, server_port=server_port, allow_insecure=allow_insecure)

    def login_with_token(self, token: str) -> Response | None:
        self.logger.log(level=logging.WARN, msg='User can\'t log with token')
        return None

    def login_with_username(self, username: str, password: str) -> Response | None:
        headers = {'Authorization': _basic_auth_str(username, password)}
        response = self.do_get(endpoint=UserAPI.ENDPOINT_USER_LOGIN, extra_headers=headers)

        if response.status_code == 200:
            # Save token for further use
            response_json = response.json()
            if 'user_token' in response_json:
                self.token = response_json['user_token']
            else:
                logging.ERROR('Got bad reply from login request.')

        return response


