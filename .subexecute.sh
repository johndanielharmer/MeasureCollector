#!/bin/sh

make -C compiletest
cd compiletest
./bin/GEDCOMtests
