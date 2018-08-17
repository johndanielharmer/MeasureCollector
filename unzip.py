import sys, getopt, os
sys.path.append('./src')
import io, json
from pprint import pprint
import re
import string
import glob
import zipfile
import shutil


i=1
zipfiles = []
zipfiles = glob.glob("studentfolders/*.zip")

for file in zipfiles:
	try:
		zip_ref = zipfile.ZipFile(file, 'r')
		zip_ref.extractall("studentfolders/user"+str(i))
		zip_ref.close()
	except:
		print "File was not a zip file, moving on"
	#print "studentfolders/user"+str(i)+"/_MACOSX"
	if(os.path.isdir("studentfolders/user"+str(i)+"/__MACOSX")):
		shutil.rmtree("studentfolders/user"+str(i)+"/__MACOSX")
	i=i+1
	if(os.path.isdir("studentfolders/__MACOSX")):
		shutil.rmtree("studentfolders/__MACOSX")
	
#print zipfiles