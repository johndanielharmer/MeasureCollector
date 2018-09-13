all: compiletest
	mkdir -p ./compiletestA1/bin
	mkdir -p ./compiletestA1/studentCode
	mkdir -p ./compiletestA1/studentInclude
        mkdir -p ./compiletestA2/bin
        mkdir -p ./compiletestA2/studentCode
        mkdir -p ./compiletestA2/studentInclude
	mkdir -p ./compiletestA1/.depend
	mkdir -p .compiletestA2/.depend
	chmod +x .subexecute.sh
	chmod +x .checkharness.sh
	chmod +x .cleanup.sh

extract: unzip.py
	python unzip.py

clean:
	rm src/*.pyc
	./.cleanup.sh
	
