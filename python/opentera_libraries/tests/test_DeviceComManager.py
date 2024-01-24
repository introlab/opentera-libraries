from opentera_libraries.device.DeviceComManager import DeviceComManager
from opentera_libraries.user.UserComManager import UserComManager
from opentera_libraries.user.UserAPI import ENDPOINT_USER_DEVICES, ENDPOINT_USER_SERVER_SETTINGS
from BaseTest import BaseTest
import logging


class TestDeviceComManager(BaseTest):
    def setUp(self):
        self.com_manager = DeviceComManager('127.0.0.1', 40075, allow_insecure=True)
        self.com_manager.set_logging_level(logging.DEBUG)

    def test_device_register(self):
        user_com = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.assertEqual(200, user_com.login_with_username('admin', 'admin').status_code)
        response = user_com.do_get(endpoint=ENDPOINT_USER_SERVER_SETTINGS, params={'device_register_key': True})
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        server_key = response_json['device_register_key']

        response = self.com_manager.register_device(server_key=server_key, device_name='Test Device',
                                                    device_type_key='Test')
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        self.assertTrue('device_token' in response_json)
        self.assertIsNotNone(response_json['device_token'])
        self.reset_test_db()

    def test_device_register_with_subtype(self):
        user_com = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.assertEqual(200, user_com.login_with_username('admin', 'admin').status_code)
        response = user_com.do_get(endpoint=ENDPOINT_USER_SERVER_SETTINGS, params={'device_register_key': True})
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        server_key = response_json['device_register_key']

        response = self.com_manager.register_device(server_key=server_key, device_name='Test Device',
                                                    device_type_key='Test', device_subtype_name='Subtype #1')
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        self.assertTrue('device_token' in response_json)
        self.assertIsNotNone(response_json['device_token'])
        self.reset_test_db()

    def test_device_login_with_token(self):
        # Get a device token from OpenTera
        user_com = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.assertEqual(200, user_com.login_with_username('admin', 'admin').status_code)
        response = user_com.do_get(endpoint=ENDPOINT_USER_DEVICES, params={'id_device': 1})
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        device_info = response_json[0]
        device_token = device_info['device_token']

        # Try to login now
        response = self.com_manager.login_with_token(device_token)
        self.assertEqual(200, response.status_code)
        self.assertIsNotNone(self.com_manager.token)

    def test_device_login_with_bad_token(self):
        response = self.com_manager.login_with_token('BADTOKEN')
        self.assertEqual(401, response.status_code)
        self.assertIsNone(self.com_manager.token)

