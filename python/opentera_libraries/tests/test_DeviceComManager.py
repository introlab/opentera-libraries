from opentera_libraries.device.DeviceComManager import DeviceComManager
from opentera_libraries.user.UserComManager import UserComManager
import opentera_libraries.user.UserAPI as UserAPI
from BaseTest import BaseTest

import logging
import os


class TestDeviceComManager(BaseTest):

    def setUp(self):
        self.com_manager = DeviceComManager('127.0.0.1', 40075, allow_insecure=True)
        self.com_manager.set_logging_level(logging.DEBUG)

        # Create test file to stream
        self.test_file_size = 1024 * 100
        f = open('testfile', 'wb')
        f.write(os.urandom(self.test_file_size))
        f.close()

        # Get a device token from OpenTera to test
        user_com = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.assertEqual(200, user_com.login_with_username('admin', 'admin').status_code)
        response = user_com.do_get(endpoint=UserAPI.ENDPOINT_USER_DEVICES, params={'id_device': 1})
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        device_info = response_json[0]
        self.device_token = device_info['device_token']

    def tearDown(self):
        super().tearDown()
        if os.path.exists('testfile'):
            os.remove('testfile')

    def test_device_register(self):
        user_com = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.assertEqual(200, user_com.login_with_username('admin', 'admin').status_code)
        response = user_com.do_get(endpoint=UserAPI.ENDPOINT_USER_SERVER_SETTINGS, params={'device_register_key': True})
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
        response = user_com.do_get(endpoint=UserAPI.ENDPOINT_USER_SERVER_SETTINGS, params={'device_register_key': True})
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
        # Try to login now
        response = self.com_manager.login_with_token(self.device_token)
        self.assertEqual(200, response.status_code)
        self.assertIsNotNone(self.com_manager.token)

    def test_device_login_with_bad_token(self):
        response = self.com_manager.login_with_token('BADTOKEN')
        self.assertEqual(401, response.status_code)
        self.assertIsNone(self.com_manager.token)

    def test_device_file_upload(self):
        self.com_manager.token = self.device_token

        # Get a session for that device
        user_com = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.assertEqual(200, user_com.login_with_username('admin', 'admin').status_code)
        response = user_com.do_get(endpoint=UserAPI.ENDPOINT_USER_SESSIONS, params={'id_device': 1})
        self.assertEqual(200, response.status_code)
        session_id = response.json()[0]['id_session']

        response = self.com_manager.upload_file(id_session=session_id, asset_name='Test Asset', file_path='testfile')
        self.assertEqual(200, response.status_code)

        self.com_manager.token = None

