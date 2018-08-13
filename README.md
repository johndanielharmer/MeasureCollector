# MeasureCollector
This is the Measure Collector for the CIS*2750 IFS project. Note that it has no IFS integration in this form and is purely made to extract measures from data repositories.

How to Run

1. Ensure ctags is installed

Type 'brew install ctags' on OSX to install the ctags tool. ctags is not installed on the SOCS server currently, which may be an issue in the future.

2. Place student files in designated folder

Navigate to the MeasureCollector folder. Extract all student folders which are being evaluated to the studentfolders folder. NOTE: Must be in the format of 'MeasureCollector/studentfolders/*/assign1'. If the student folders are not named assign1 then I will have to do a little modification on the code as it's not entirely modular yet.

3. Extract compilation test files

Type "make" into the terminal to extract the test harness and ensure folders are all named correctly

4. Execution

Once all the student code is extracted in this format, do the following:

In the MeasureCollector folder, type 'python manager.py -d studentfolders [-c] [-a] [-r] [-e] [-o <output/file/location.csv>] '.

Optional Flags
-c indicates a CSV output file will be generated based on the results of the parsing.
-o will indicate the output file name and location of the csv file, with a default of output.csv. The tool will not print output to the screen other than a starting notice or error notices when running with -c.
-a anonymizes the user submission folder names
-r will run the test harness and give a score to the user. Should only be used for preliminary testing purposes and gathering a score for measure tracking, SHOULD NOT be in student version
-e will display any command line errors encountered when compiling

Notes:

Type 'make clean' in the directory to destroy the compiletest folder. The studentfolders folder wil not be emptied.

Some compliance measures (Files in the correct place/files have correct names, folders in the correct place/folders have correct names) are merged into single checks rather than two unique checks. The system cannot yet determine if the file is named correctly but put into the wrong spot. If the folder is named wrong, all the files inside it will be deemed incorrectly named as well. This can be addressed in the future if necessary.

Does not yet compile the files with the test harness. Can run the files with the test harness on their own fine, but ran out of time for integration.

There are still some measures and compliance checks to be implemented, but hopefully with 19 measures and 6 compliance checks it will function as a reasonable start for the purposes of the ethics doc. Need to go back and refactor some code / fix some commenting.




