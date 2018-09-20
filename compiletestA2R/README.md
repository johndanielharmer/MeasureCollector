# University of Guelph - CIS\*2750 W18 - Assignment 2 Test Suite

## Installation

```Bash
make
./bin/GEDCOMtests
```

## Test Student Code

```Bash
# Unzip student code
unzip *.zip

# Remove old student code
rm studentCode/* studentInclude/*

# Copy source files to test suite
cp assign1/src/* studentCode/

# Copy header files to test suite. DO NOT 
cp assign1/incude/* studentInclude/

# Remake and run test suite
make
./bin/GEDCOMtests
```
