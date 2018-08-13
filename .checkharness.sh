#!/bin/sh
  
cd compiletest 2> /dev/null
make > /dev/null
[ -f ./bin/GEDCOMtests ] || { echo "ERROR: Problem in testing program compilation\nEnsure all assignment specific files/functions are present and named correctly\nExiting";exit 1; }
