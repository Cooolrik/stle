# ctle Copyright (c) 2024 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import unittest
import sys
import os

# Add the src directory to sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'code_gen')))

# Now you can import your_module
import ctlepy

class TestFormattedOutput(unittest.TestCase):
	def setUp(self):
		os.makedirs('./outputs/TestFormattedOutput', exist_ok=True)

	def test_basic_functionality(self):
		f = ctlepy.formatted_output()
		f.generate_license_header()
		f.write_lines_to_file('./outputs/TestFormattedOutput/testout.txt')

if __name__ == '__main__':
	unittest.main()
