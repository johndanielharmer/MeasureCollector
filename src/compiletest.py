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

from compliance import complianceManager

#Copies the contents of a student assignment folder (only .c and .h files are allowed) to the correct position in the destination folder.
#INPUT: The file location of the root student assignment directory, the destination folder where the files will be copied to, an optional variable which allows for files to be excluded from the copy
#OUTPUT: Files will be copied from source to destination
def copyFiles(studentFolder, destination, excludeFiles=[]):
	excludedStudentFiles = []
	foundFlag = False
	# Get all .c and .h files in the folder
	studentFiles = glob.glob(studentFolder+'/*.h') + glob.glob(studentFolder+'/*.c')
	#Look for all excluded files, if any
	for exclude in excludeFiles:
		for files in studentFiles:
			if exclude in files:
				excludedStudentFiles.append(files)
				break
	#Remove exlucded files from student file list
	for files in excludedStudentFiles:
		studentFiles.remove(files)
		#print files
		#copyfile(studentFiles, destination)
	for copyFile in studentFiles:
		#print copyFile, destination
		shutil.copy(copyFile, destination)

#Runs the compile, testing and cleanup script. Also checks output and either displays it or stores it for CSV purposes.
#INPUT: Folder address of the folder to parse for the list of specific measures to calculate. Optional variables: csv determines whether or not the output prints or gathers results for csv formatting, csvList is the list of all data for the current file up until this point
#OUTPUT: Returns a blank list if CSV is set to false, or a populated list of measures in the directory if csv=true
def compileManager(projectFiles, runharness, showErrors, csv=False, csvList=[]):
	srcDirectory = "./"+projectFiles+"/assign1/src"
	includeDirectory = "./"+projectFiles+"/assign1/include"
	
	#Copy all files to the correct location
	copyFiles(srcDirectory, "./compiletest/studentCode")
	copyFiles(includeDirectory, "./compiletest/studentInclude", ["GEDCOMparser.h", "LinkedListAPI.h"])

	#Execute the .subexecute script
	#Makes the test harness with the new student files, then runs the script
	if (runharness == True):
		result = subprocess.Popen("./.subexecute.sh", stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

	else:
		result = subprocess.Popen("./.checkharness.sh", stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
	#Trim garbage output
	listResult = list(result)
	
	#Call the cleanup script
	#Remove all student files and leftover files from the project
	csvListCompliance = complianceManager(projectFiles, csv, [])
	csvList = csvListCompliance+csvList
	
	#Look for warning that the script failed
	#TO DO: Implement a better method in the future
	if "ERROR: Compilation Error" in listResult[0]:
		if (csv==True):
			csvList.append("Failure")
		else:
			print "Compilation Success: False"
			if (showErrors == True):
				print listResult[1]
				print listResult[0]
			
	else:
		if (csv==True):
			csvList.append("Success")
			#print "Compilation Success: True"
		elif (csv==False):
			print "Compilation Success: True"
			if (runharness == True):
				print listResult[0]
	
	subprocess.call("./.cleanup.sh", stdout=subprocess.PIPE)
	return csvList

#ONLY WORKS WITH testFiles FOLDER FILES
def main():
	#Copy all files to the correct location
	copyFiles("./tests/testFiles/assign1/src", "./compiletest/studentCode",  ["GEDCOMparser.c"])
	copyFiles("./tests/testFiles/assign1/include", "./compiletest/studentInclude", ["GEDCOMparser.h", "LinkedListAPI.h"])

	#Execute the .subexecute script
	#Makes the test harness with the new student files, then runs the script
	result = subprocess.Popen("./.subexecute.sh", stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

	#Trim garbage output
	listResult = list(result)
	listResult = listResult[:-1]

	#Look for warning that the script failed
	#TO DO: Implement a better method in the future
	if "Exiting" in listResult[0]:
		print "Error: Failure to Compile"
	else:
		print "Compilation Success"
	print listResult[0]

	#Call the cleanup script
	#Remove all student files and leftover files from the project
	subprocess.call("./.cleanup.sh", stdout=subprocess.PIPE)

if __name__ == '__main__':
	main()
	
	