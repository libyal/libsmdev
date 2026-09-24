#!/usr/bin/env python3
#
# Python-bindings support functions test script
#
# Copyright (C) 2010-2026, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import os
import platform
import sys
import unittest

import pysmdev


class SupportFunctionsTests(unittest.TestCase):
    """Tests the support functions."""

    def _check_read_access(self, path):
        """Check if a device file can be read.

        Args:
          path (str): path to the device file.
        """
        # Note that os.access(path, os.R_OK) can return True while open(path, 'rb') fails.
        try:
            with open(path, "rb"):
                pass
        except Exception:
            return False

        return True

    def _get_source(self):
        """Retrieves a source for testing."""
        if platform.system() == "Windows":
            return "\\\\.\\PhysicalDrive0"

        source = "/dev/sda"
        if not os.path.exists(source):
            source = "/dev/vda"

        if not self._check_read_access(source):
            raise unittest.SkipTest("missing readable source")

        return source

    def test_get_version(self):
        """Tests the get_version function."""
        version = pysmdev.get_version()
        self.assertIsNotNone(version)

    # TODO: add tests for check_device

    def test_open(self):
        """Tests the open function."""
        test_source = self._get_source()

        smdev_handle = pysmdev.open(test_source)
        self.assertIsNotNone(smdev_handle)

        smdev_handle.close()

        with self.assertRaises(TypeError):
            pysmdev.open(None)


if __name__ == "__main__":
    unittest.main(verbosity=2)
