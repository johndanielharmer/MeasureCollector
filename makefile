all: compiletest
	mkdir -p ./compiletest/.depend
	chmod +x .subexecute.sh
	chmod +x .checkharness.sh
	chmod +x .cleanup.sh

clean:
	rm src/*.pyc
	./.cleanup.sh
	
