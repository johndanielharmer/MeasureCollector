all: compiletest
	mkdir -p ./compiletest/bin
	mkdir -p ./compiletest/studentCode
	mkdir -p ./compiletest/studentInclude
	mkdir -p ./compiletest/.depend
	chmod +x .subexecute.sh
	chmod +x .checkharness.sh
	chmod +x .cleanup.sh
	chmod +x unzip.sh

extract: unzip.py
	python unzip.py

clean:
	rm src/*.pyc
	./.cleanup.sh
	
