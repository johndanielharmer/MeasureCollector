#!/usr/bin/env python
#
# This script parses c and h files for compliance with the CIS*2750 assignment outline when paired with a corresponding JSON string.
# Copyright (c) 2018 John Harmer jharmer@uoguelph.ca
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
# OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

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
from includecheck import improperCount

#Parse the JSON string for information and translate that into a list which can be interpreted by other functions
#INPUT: The directory passed as a command line argument where student folders exist, the JSON file which was written for the assignment
#OUTPUT: Four lists which contain all of the expected folder, files, functions and readme headings
def getExpectedStructure(idirectory, jsonString):
	expectedFolderNames = []
	expectedFileNames = []
	expectedFunctionDeclarations = []
	expectedReadmeStructure = []
	newExpectedFolderNames = []
	newExpectedFileNames = []
	
	#Test file of prewritten JSON to match files
	with open(jsonString) as f:
		data = json.load(f)
	expectedFolderNames = data["compliance"]["folderNames"]
	for efn in expectedFolderNames:
		newExpectedFolderNames.append(idirectory+"/"+efn)
	newExpectedFolderNames = [str(x) for x in newExpectedFolderNames]
	#print "From JSON string:",newExpectedFolderNames
	[x.encode('ascii') for x in expectedFolderNames]
	expectedFileNames = data["compliance"]["fileNames"]
	for efn in expectedFileNames:
		newExpectedFileNames.append(idirectory+"/"+efn)
	newExpectedFileNames = [str(x) for x in newExpectedFileNames]
	[x.encode('UTF8') for x in expectedFileNames]
	expectedReadmeStructure = data["compliance"]["readmeCategories"]
	[x.encode('UTF8') for x in expectedReadmeStructure]
	
	for files in expectedFileNames:
		tempList = []
		try:
			for functions in data["compliance"]["functionNames"][files]:
				tempList.append(functions.encode("utf-8"))
		except:
			#Do nothing
			pass
			
		expectedFunctionDeclarations.append(files.encode("utf-8"))
		expectedFunctionDeclarations.append(tempList)
	return newExpectedFileNames, newExpectedFolderNames, expectedFunctionDeclarations, expectedReadmeStructure

#Scrape a directory for all folders, files and functions in the given directory
#INPUT: File path to a directory to parse
#OUTPUT: 3 lists containing the folders, files and function declarations in the directory
def getActualStructure(path):
	#print path
	actualFolders = []
	actualFiles = []
	actualFunctions = []
	for newPath, dirs, files in os.walk(path):
		#print newPath
		if (newPath != path):
			actualFolders.append(newPath)
		for f in files:
			if f != '':
				actualFiles.append(newPath+"/"+f)
	return actualFolders, actualFiles, actualFunctions

#TO DO: FINALIZE AND UPDATE THIS FUNCTION
#Currently not used
def getFunctionHeaders(fileToRead):
	allHeaders = []
	count = 0
	result = subprocess.Popen(['ctags', '-x', '--c-types=f', fileToRead], stdout=subprocess.PIPE)
	#print (result.stdout.decode('utf-8'))
	while True:
		line = result.stdout.readline()
		#print line
		if line == '':
			break
		else:
			#print line.strip()
			allHeaders.append(line.strip())
			count = count+1
	#print count
	for header in allHeaders:
		print header

#Read a README file for specific headers as defined by the JSON compliance file
#INPUT: The file address of the readme and a list of the expected categories for the readme. Optional variables: csv determines whether or not the output prints or gathers results for csv formatting, csvList is the list of all data for the current file up until this point.
#OUTPUT: Returns a blank list if CSV is set to false, or a populated list of number of missing headers and unexpected folders in the directory if csv=true

def checkReadme(readme, readmeCategories, csv=False, csvList=[]):
	categoryCount = len(readmeCategories)
	categoryFound = 0
	try:
		file_object = open(readme, "r")
	except IOError:
		if (csv == True):
			csvList.append(0)
			return csvList
		else:
			print "Cannot open README file, check that it exists and is named as per the specification"
			return []
	str_list = filter(None, file_object.read().splitlines())
	#print str_list
	for entry in str_list:
		if categoryFound >= categoryCount:
			if (csv == False):
				print "All Readme Headings found"
			if (csv==True):
				csvList.append(categoryFound)
				return csvList
			return []
		for category in readmeCategories:
			if category in entry:
				categoryFound = categoryFound + 1
				break
	if (csv == False):
		print "Missing",categoryCount - categoryFound,"Readme Headings, check assignment outline"
	if (csv==True):
		csvList.append(categoryFound)
		return csvList
	return []

#Compares expected file structure to actual file structure in a given directory
#INPUT: A list of expected file names populated from the JSON string passed to this file, a list of actual file names created by scraping the directory being parsed. Optional variables: csv determines whether or not the output prints or gathers results for csv formatting, csvList is the list of all data for the current file up until this point
#OUTPUT: Returns a blank list if CSV is set to false, or a populated list of number of missing files and unexpected files in the directory if csv=true
def compareFiles(expectedFileNames, actualFileNames, csv=False, csvList=[]):
	found = False
	missingCount = 0
	for expected in expectedFileNames:
		for actual in actualFileNames:
			if (expected == actual):
				found = True
		if (found == False):
			if (csv == False):
				print "ERROR: Missing or improperly named file:", expected
			missingCount = missingCount +1
		found = False
	if (csv == False):
		print "Missing a total of",missingCount,"Files"
	
	found = False
	extraCount = 0
	for actual in actualFileNames:
		for expected in expectedFileNames:
			if (expected == actual):
				found = True
		if (found == False):
			if (csv == False):
				print "WARNING: Extra non-specification outlined file:", actual
			extraCount = extraCount +1
		found = False
	if (csv == False):
		print "Directory contains a total of",extraCount,"non-specification outlined files"
	if (csv==True):
		csvList.append(missingCount)
		csvList.append(extraCount)
		return csvList
	return []


#Compares expected folder structure to actual folder structure in a given directory
#INPUT: A list of expected folder names populated from the JSON string passed to this file, a list of actual folder names created by scraping the directory being parsed. Optional variables: csv determines whether or not the output prints or gathers results for csv formatting, csvList is the list of all data for the current file up until this point
#OUTPUT: Returns a blank list if CSV is set to false, or a populated list of number of missing folders and unexpected folders in the directory if csv=true
def compareFolders(expectedFolderNames, actualFolderNames, csv=False, csvList=[]):
	#print "Expected:", expectedFolderNames
	#print "Actual:", actualFolderNames
	
	found = False
	missingCount = 0
	for expected in expectedFolderNames:
		for actual in actualFolderNames:
			if (expected == actual):
				found = True
		if (found == False):
			if (csv == False):
				print "ERROR: Missing or improperly named folder:", expected
			missingCount = missingCount +1
		found = False
	if (csv == False):
		print "Missing a total of",missingCount,"folders"
	
	found = False
	extraCount = 0
	for actual in actualFolderNames:
		for expected in expectedFolderNames:
			if (expected == actual):
				found = True
		if (found == False):
			if (csv == False):
				print "WARNING: Extra non-specification outlined folder:", actual
			extraCount = extraCount +1
		found = False
	if (csv == False):
		print "Directory contains a total of",extraCount,"non-specification outlined folders"
	if (csv==True):
		csvList.append(missingCount)
		csvList.append(extraCount)
		return csvList
	return []

#Handles all the compliance measures being calculated.
#Input: A folder with student submission inside. Optional variables: csv determines whether or not the output prints or gathers results for csv formatting, csvList is the list of all data for the current file up until this point
#OUTPUT: Returns a blank list if CSV is set to false, or a populated list of number of measures calculated by this file if csv=true

def complianceManager(idirectory, csv=False, csvList=[]):
	actualFolderNames = []
	actualFileNames = []
	actualFunctionNames = []
	expectedFolderNames = []
	expectedFileNames = []
	expectedFunctionDeclarations = []
	expectedReadmeCategories = []
	
	expectedFileNames, expectedFolderNames, expectedFunctionDeclarations, expectedMakefileCategories = getExpectedStructure(idirectory, "src/compliance.json")

	actualFolderNames, actualFileNames, actualFunctionDeclarations = getActualStructure(idirectory)

	
	for entry in expectedFileNames:
		entry = entry.encode("utf-8")
	
	#print expectedFolderNames
	#print actualFolderNames
	#print expectedFileNames
	#print actualFileNames
	if (csv == False):
		print "---------- Compliance for directory", idirectory,"----------"
	
	csvList = compareFolders(expectedFolderNames,actualFolderNames, csv, csvList)
	csvList = compareFiles(expectedFileNames,actualFileNames, csv, csvList)
	csvList = checkReadme(idirectory+"/assign1/README", expectedMakefileCategories, csv, csvList)
	csvList = improperCount(idirectory, csv, csvList)
	return csvList
	#getFunctionHeaders("./assign1/src/hash.c")

def main(argv):

	
	
	
	idirectory = ''
	#Make sure a file directory is provided
	if (len(argv) <= 1):
		print "Please provide a directory to search for C files."
		sys.exit()
	else:
		#Get command line arguments and put them into list
		options = { 'tool':'', 'includecheck':'', 'jsonInput':'', 'ifsOff':'',
					'dir':''
					}

		# define command line arguments and check if the script call is valid
		opts, args = getopt.getopt(argv,'t:d:j:i:h',
			['tool=','directory=', 'jsonInput=', 'ifsOff=', 'help'])
		
		#Set options and tool being selected
		#Currentl only grabs includecheck.py but can be expanded in the future
		for opt, arg in opts:
			if opt in ('--tool', '-t'):
				options['tool'] = arg
			elif opt in ('directory', '-d'):
				idirectory = arg
				if not (os.path.isdir(idirectory)):
					sys.stderr.write( 'Error. Directory ' + idirectory + ' does not exist.\n' )
					sys.exit()
			elif opt in ('--jsonInput', '-j'):
				options['jsonInput'] = arg
			elif opt in ('--ifsOff', '-i'):
				options['ifs'] = False


		if idirectory != '':
			options['dir'] = idirectory
		
	complianceManager(idirectory)
		
if __name__ == '__main__':
	main(sys.argv[1:])
