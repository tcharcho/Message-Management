#!/usr/bin/python

import sys
import glob
import os

if __name__ == '__main__':
	if len(sys.argv) == 1:
		print "Error: must provide author ID and stream name" 
		sys.exit(1)

	command = "./db -getTotal"

	for x in xrange(1,len(sys.argv)):
		command = command + " " + sys.argv[x]

	os.system(command)

	sys.exit(0)
	

