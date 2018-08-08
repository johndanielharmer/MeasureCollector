import sys, getopt, os
import io, json
from pprint import pprint
import re
import string
from subprocess import call
from subprocess import Popen, PIPE
import shlex
import glob
from cStringIO import StringIO
import subprocess
import shutil

#Copies the contents of a student assignment folder to the correct position in the destination folder.

def copyFiles(studentFolder, destination, excludeFiles=[]):
	excludedStudentFiles = []
	foundFlag = False
	studentFiles = glob.glob(studentFolder+'/*.h') + glob.glob(studentFolder+'/*.c')
	for exclude in excludeFiles:
		for files in studentFiles:
			if exclude in files:
				excludedStudentFiles.append(files)
				break
	for files in excludedStudentFiles:
		studentFiles.remove(files)
		#print files
		#copyfile(studentFiles, destination)
	for copyFile in studentFiles:
		print copyFile, destination
		shutil.copy(copyFile, destination)

	

def main():
	copyFiles("./tests/testFiles/assign1/src", "./compiletest/studentCode")
	copyFiles("./tests/testFiles/assign1/include", "./compiletest/studentInclude", ["GEDCOMparser.h", "LinkedListAPI.h"])
	#result = subprocess.Popen(['ls', '-l'], stdout=subprocess.PIPE)
	#result = subprocess.Popen(['make', '-C', './compiletest'], stdout=subprocess.PIPE)
	#result = subprocess.Popen(['./compiletest/bin/GEDCOMtests'], stdout=subprocess.PIPE)
	#while True:
		#line = result.stdout.readline()
		#if len(line) == 0:
			#break
		#print line

if __name__ == '__main__':
	main()
	
	