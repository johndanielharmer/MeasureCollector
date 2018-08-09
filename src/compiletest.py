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
		#print copyFile, destination
		shutil.copy(copyFile, destination)

#def compileManaged(projectFiles, csv=False, csvList=[]):
	

def main():
	#print "TEST1"
	copyFiles("./tests/testFiles/assign1/src", "./compiletest/studentCode",  ["GEDCOMparser.c"])
	#print "TEST2"
	copyFiles("./tests/testFiles/assign1/include", "./compiletest/studentInclude", ["GEDCOMparser.h", "LinkedListAPI.h"])
	#print "STARTING STEP 1"
	#print "TEST3"
	result = subprocess.Popen("./.subexecute.sh", stdout=subprocess.PIPE).communicate()
	#print "TEST4"
	#print result
	listResult = list(result)
	listResult = listResult[:-1]
	#print listResult[0]
	#print len(listResult)
	if "Exiting" in listResult[0]:
		print "PYTHON DETECTS FAILURE"
		#print listResult[0]
	else:
		print "PYTHON DETECTS SUCCESS"
	print listResult[0]
	#print listResult[0]
	#print "TEST"
	#for entry in newresult:
	#	print entry
	#print "STARTING STEP 2"
	subprocess.call("./.cleanup.sh", stdout=subprocess.PIPE)
	#print result
	#result = subprocess.Popen("./.cleanup.sh", stdout=subprocess.PIPE)
	#print result
	#print "STARTING STEP 3"
	
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
	
	