#!/usr/bin/python

import sys
import glob
import os
import fileinput

if __name__ == '__main__':
	#"./getPost.py ". $aID. " -s ". $str. " ". $currPos;

	if len(sys.argv) == 1:
		print "Error: must provide author ID, stream name and curent position." 
		sys.exit(1)

	command = "./db -getPost"

	for x in xrange(1,len(sys.argv)):
		command = command + " " + sys.argv[x]

	os.system(command)

	sys.exit(0)








