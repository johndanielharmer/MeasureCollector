all: compiletest.zip
	unzip compiletest.zip -d .
	mkdir -p ./compiletest/.depend
	chmod +x .subexecute.sh
	chmod +x .checkharness.sh
	chmod +x .cleanup.sh

clean:
	rm -r compiletest
