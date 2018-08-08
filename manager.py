#Main Manager for CIS*2750 IFS Measure Collector
#John Harmer, 2018
#jharmer@uoguelph.ca

import sys, getopt, os
sys.path.append('./src')
import io, json
from pprint import pprint
import re
import string
import glob
import csv

from compliance import complianceManager
from MeasureCollector import measureManager

def main(argv):
	i=0
	anon=False
	csvFileAddress = "output.csv"
	csvList = []
	idirectory = ''
	#Make sure a file directory is provided
	if (len(argv) <= 1):
		print "Please provide a directory to search for C files."
		sys.exit()
	else:
		#Get command line arguments and put them into list
		options = { 'tool':'', 'directory':'', 'jsonInput':'', 'ifsOff':'',
					'csv':'', 'output':'', 'anon':'','help':''
					}

		# define command line arguments and check if the script call is valid
		opts, args = getopt.getopt(argv,'t:d:j:i:co:ah',
			['tool=','directory=', 'jsonInput=', 'ifsOff=', 'csv','output=','anon','help'])
		
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
			elif opt in ('--csv', '-c'):
				options['csv'] = True
			elif opt in ('--output', '-o'):
				#print arg
				csvFileAddress = arg
			elif opt in ('--anon', '-a'):
				#print arg
				anon = True


		if idirectory != '':
			options['dir'] = idirectory
	#print options['csv']
	
	studentFiles = glob.glob(idirectory+'/*')
	studentFolders = filter(lambda f: os.path.isdir(f), studentFiles)
	#print studentFolders
	if options['csv'] != True:
		print "---------- BEGINNING CALCULATION ----------"
	
		for folder in studentFolders:
			measureManager(folder)
			complianceManager(folder)
			print ''
	else:
		with open(csvFileAddress,'wb') as csvFile:
			filewriter = csv.writer(csvFile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
			filewriter.writerow(['User',
			'Total-LOC',
			'Total-SLOC',
			'Total-Comment-Count',
			'Average-LOC-per-Module',
			'Average-SLOC-per-Module',
			'Average-comment-count-per-Module',
			'Total-number-of-includes',
			'Number-of-unique-includes',
			'Total-number-of-type-keywords',
			'Average-number-of-type-keywords-per-module',
			'Total-number-of-flow-keywords',
			'Average-number-of-flow-keywords-per-module',
			'Total-number-of-non-loop-conditional-keywords',
			'Average-number-of-non-loop-conditional-keywords-per-Module',
			'Total-number-of-functions-in-directory',
			'Average-number-of-functions-per-module',
			'Average-function-length',
			'Total-number-of-operators-across-all-files',
			'Number-of-unique-operators-across-all-files',
			'Missing-Expected-Folders',
			'Extra-Non-Specification-Folders',
			'Missing-Expected-Files',
			'Extra-Non-Specification-Files',
			'Missing-Expected-Functions',
			'Expected-Readme-Headings-Missing',
			'Total-number-of-improper/hardcoded-include-paths'])
			
			print "---------- BEGINNING CSV FILE WRITING ----------"

			for folder in studentFolders:
				i=i+1
				csvListMeasure = []
				csvListCompliance = []
				csvList = []
				csvListMeasure = measureManager(folder, True, csvListMeasure)
				csvListCompliance = complianceManager(folder, True, csvListCompliance)
				if (anon == True):
					userName = "user"+str(i)
					csvList = [userName]+csvListMeasure + csvListCompliance
				else:
					csvList = [folder]+csvListMeasure + csvListCompliance
				#print ''
				#print csvList
				filewriter.writerow(csvList)
		
if __name__ == '__main__':
	main(sys.argv[1:])