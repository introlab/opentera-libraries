from user.UserComManager import UserComManager
from BaseTest import BaseTest
import logging


class TestUserComManager(BaseTest):
    def setUp(self):
        self.com_manager = UserComManager('127.0.0.1', 40075, allow_insecure=True)
        self.com_manager.set_logging_level(logging.DEBUG)

    def test_user_login(self):
        response = self.com_manager.login_with_username('admin', 'admin')
        self.assertEqual(200, response.status_code)
        response_json = response.json()
        self.assertTrue('user_token' in response_json)
        self.assertIsNotNone(self.com_manager.token)
