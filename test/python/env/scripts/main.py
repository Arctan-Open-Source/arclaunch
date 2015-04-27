#!/usr/bin/python

from os import environ
from sys import stdout

# Need to read from an environment variable and write it to stdout without formatting
stdout.write(environ['TEST_ENV'])
