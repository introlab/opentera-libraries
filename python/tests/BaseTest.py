import unittest
from requests import get


class BaseTest(unittest.TestCase):

    def reset_test_db(self):
        response = get(self.com_manager.server_url + '/api/test/database/reset', verify=False)
        self.assertEqual(200, response.status_code)