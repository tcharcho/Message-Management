#!/usr/bin/python

import sys
import glob
import os
import fileinput

if __name__ == '__main__':
	#"./markRead.py ". $aID. " -s ". $stream;

	if len(sys.argv) == 1:
                print "Error: must provide author ID and stream name"
                sys.exit(1)

	command = "./db -markRead"

	for x in xrange(1,len(sys.argv)):
		command = command + " " + sys.argv[x]

	os.system(command)

	sys.exit(0)




	

