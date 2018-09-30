CC = gcc
CFLAGS = -Wall -g -Iinclude -IstudentInclude

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CCFLAGS += -std=gnu11 
endif
ifeq ($(UNAME), Darwin)
	CCFLAGS += -std=c11
endif

cardTests:  
	$(CC)  $(CFLAGS) src/*.c  studentCode/*.c  -o bin/cardTests

calMemTest:
	$(CC)  $(CFLAGS) src/calMemTest.c studentCode/*.c  -o bin/calMemTest1
	$(CC)  $(CFLAGS) src/calMemTestPrint.c studentCode/*.c  -o bin/calMemTest2

calMemTestConst:
	$(CC)  $(CFLAGS) -DCONST_ERR=0 src/calMemTest.c studentCode/*.c  -o bin/calMemTestConst1
	$(CC)  $(CFLAGS) -DCONST_ERR=0 src/calMemTestPrint.c studentCode/*.c  -o bin/calMemTestConst2