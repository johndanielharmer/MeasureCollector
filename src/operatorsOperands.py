import sys
import os.path
import re
import glob
from glob import glob

def getOperatorsList():
	operatorList = [
		"<<",
		">>",
		"<=",
		">=",
		"==",
		"!=",
		"^",
		"?:",
		"+=",
		"-=",
		"=+",
		"=-",
		"*=",
		"=*",
		"/=",
		"=/",
		"%=",
		"=%",
		">>=",
		"<<=",
		"&=",
		"^=",
		"|=",
		"->",
		"++",
		"--",
		"||",
		"(type)*",
		"sizeof",
		"&&",
		"&",
		"*",
		"/",
		"%",
		"(",
		")",
		"[",
		"]",
		"+",
		"-",
		".",
		"!",
		"~",
		"<",
		">",
		"|",
		"=",
		","
	]
	return operatorList

def getOperators(projectFiles, csv=False, csvList=[]):
	totalCount = 0
	uniqueOperatorCount = 0
	foundBreak = False
	i=0
	copiedFiles = []
	operatorsList = []
	operatorsList.append([])

	#Set each keyword
	operatorsList[0] = getOperatorsList()
	#projectFiles = glob(dir+'/*.c') + glob(dir+'/*.h')
	for files in projectFiles:
		if os.path.isfile(files) == False:
			print "File", file, "to parse does not exist, please enter a valid file"
			exit()
		#Set list counters to 0 for each keyword
		operatorsList.append([0 for i in range(48)])

	#Set each keyword


	for files in projectFiles:
		#print files
		resultant = open(files, 'r').read().split()
		copiedFiles.append(resultant)
		#print copiedFiles
	#print copiedFiles

	# Remove all special characters from words to ensure correct matching

		#for line in parse:
			#test = re.sub(r'[^A-Za-z0-9 ]+', r' ', line)
	i=0
	for files in copiedFiles:
		for word in files:
			#print word
			for operator in operatorsList[0]:
				#if operator == "->":
					#print "found it"
				if operator in word and foundBreak == False:
					totalCount = totalCount+1
					foundBreak = True
					operatorsList[1][i] = operatorsList[1][i]+1
				i=i+1
			i=0
			foundBreak = False
	i=0
	for entry in operatorsList[1]:
		if entry != 0:
			#print operatorsList[0][i]
			uniqueOperatorCount = uniqueOperatorCount + 1
		#else:
			#print operatorsList[0][i]
			#print operatorsList[0][i], operatorsList[1][i]
			#uniqueOperatorCount = uniqueOperatorCount + 1
		i=i+1
	if (csv == False):
		print "Total number of operators across all files =",totalCount
		print "Number of unique operators across all files =",uniqueOperatorCount
	if (csv==True):
		csvList.append(totalCount)
		csvList.append(uniqueOperatorCount)
		return csvList
	
	return []


def main(argv):

	if len(sys.argv) < 0:
		print "Please run with a file to parse as command line argument 1"
		exit()


	#print 'Num of arguments', len(sys.argv)
	#print 'Arg list:', str(sys.argv)

	#projectFiles = glob(argv[0]+'/*.c') + glob(argv[0]+'/*.h')
	print projectFiles

	getOperators(projectFiles)

if __name__ == '__main__':
	main(sys.argv[1:])