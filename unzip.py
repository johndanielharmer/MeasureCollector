import sys, getopt, os
sys.path.append('./src')
import io, json
from pprint import pprint
import re
import string
import glob
import zipfile
import tarfile
import shutil

def unzip(path, i=0, k=0):
	i=1
	k=1
	zipfiles = []
	zipfiles = glob.glob(path+"/*.zip")

	#print path+"/user"+str(i)+"/__MACOSX"

	for file in zipfiles:
		#print file
		try:
			#A folder of 22 bytes or lower will not be extracted.
			#22 bytes means an empty zip folder and guarding against this prevents
			#errors not caught by zipfile exceptions
			zip_ref = zipfile.ZipFile(file, 'r')
			zip_ref.extractall(path+"/user"+str(i))
			zip_ref.close()
			if (os.path.isdir(path+"/user"+str(i)) == False):
				print "Folder was not created, most likely empty submission"
				k=k+1
			else:
				if(os.path.isdir(path+"/user"+str(i)+"/__MACOSX")):
					#print "HEY CHECK OUT USER",i
					shutil.rmtree(path+"/user"+str(i)+"/__MACOSX")
				if(os.path.isdir(path+"/__MACOSX")):
					shutil.rmtree(path+"/__MACOSX")
				i=i+1
			#print(os.path.isdir("newtest/user"+str(i)))
			#print "Successful extraction #", j
				#i=i+1
			#print "studentfolders/user"+str(i)+" belongs to", file
		except zipfile.BadZipfile:
			print "File", file,"was not a zip file, moving on"
			k=k+1
		#print path+"/user"+str(i)+"/__MACOSX"

		
	return i, k
	
def untar(path,i=0,k=0):
	tarfiles = []
	tarfiles = glob.glob(path+"/*.tar") + glob.glob(path+"/*.gz")
	for file in tarfiles:
		#print file
		try:
			if tarfile.is_tarfile(file):
				tar_ref = tarfile.open(file, 'r')
				tar_ref.extractall(path+"/user"+str(i))
				tar_ref.close()
				if (os.path.isdir(path+"/user"+str(i)) == False):
					print "Folder was not created, most likely empty submission"
					k=k+1
				else:
					if(os.path.isdir(path+"/user"+str(i)+"/__MACOSX")):
						#print "HEY CHECK OUT USER",i
						shutil.rmtree(path+"/user"+str(i)+"/__MACOSX")
					if(os.path.isdir(path+"/__MACOSX")):
						shutil.rmtree(path+"/__MACOSX")
					i=i+1
		except:
			print "File was not a tar file, moving on"
			k=k+1
	return i, k

i=0
k=0	
i, k = unzip("studentfolders")
i, k = untar("studentfolders",i,k)
print "Successful extracts =", i-1
print "Unsuccessful extracts =", k-1
#print zipfiles