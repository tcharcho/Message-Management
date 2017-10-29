#!/usr/bin/python

import sys
import glob
import os
import fileinput

if __name__ == '__main__':
	#check command line parameters
	if len(sys.argv) == 1:
		print "Incorrect Usage: please provide an author ID to view a stream.\n"
		sys.exit(0)

	command = "./db -getStreams"

	for x in xrange(1,len(sys.argv)):
		command = command + " " + sys.argv[x]

	sys.exit(os.WEXITSTATUS(os.system(command)))
