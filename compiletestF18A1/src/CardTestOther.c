#include "CardTestUtilities.h"


//********************* Test printCard ***********************

static SubTestRec _tPrintCalTest1(int testNum, int subTest){
    SubTestRec result;
    char feedback[300];
    char fileName[] = "testFiles/valid/testCardN-compVal.vcf";
    
    Card* testObj;
    
    createCard(fileName, &testObj);
    
    char* calStr = printCard(testObj);
    fprintf(stderr, "%s\n", calStr);
    if(calStr != NULL && strlen(calStr) > 0)
    {
        sprintf(feedback, "Subtest %d.%d: Printed non-NULL Card object.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: printCard returned null or empty string for a valid vCard file",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    free(calStr);
    
}


static SubTestRec _tPrintCardTest2(int testNum, int subTest){
    SubTestRec result;
    char feedback[300];
    
    char* calStr = printCard(NULL);
    
    if(calStr == NULL || calStr != NULL )
    {
        sprintf(feedback, "Subtest %d.%d: printCard handled NULL Card object correctly.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: printCard did not handle NULL Card object correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tPrintCardTests(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Testing printCard", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tPrintCalTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintCardTest2);
    return rec;
}
//***************************************************************

//********************** Test printError ************************

static char * errVals[] = {
    "OK", "INV_FILE", "INV_CARD", "INV_PROP", "WRITE_ERROR", "OTHER_ERROR", "Invalid error code"};

static char* _tPrintError(VCardErrorCode err){
    if (err <= 5)
        return errVals[err];
    else
        return errVals[6];
}

static SubTestRec _tPrintErrorTest1(int testNum, int subTest){
    SubTestRec result;
    char feedback[300];
    
    for (int i = 0; i < 6; i++){
        char* errStr = (void*) printError(i);
        if (errStr == NULL){
            sprintf(feedback, "Subtest %d.%d: printError returns NULL for error code %d (%s)",testNum,subTest,i, _tPrintError(i));
            result = createSubResult(FAIL, feedback);
            return result;
        }
        
        if (strlen(errStr) == 0){
            sprintf(feedback, "Subtest %d.%d: printError returns an empty string for error code %d (%s)",testNum,subTest,i, _tPrintError(i));
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    
    sprintf(feedback, "Subtest %d.%d: printError test",testNum,subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tPrintCardErrorTest(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Testing printError", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tPrintErrorTest1);
    return rec;
}


//********************* Test deleteClandar **********************
//Calendar deletion - testCalSimpleUTC.ics
static SubTestRec _tDeleteCalTest1(int testNum, int subTest){
    SubTestRec result;
    char feedback[300];
    char fileName[] = "testFiles/valid/testCardMin.vcf";
    
    Card* testObj = NULL;

    VCardErrorCode err = createCard(fileName, &testObj);

    if (err != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteCard due to failure of createCard to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    deleteCard(testObj);
    
    sprintf(feedback, "Subtest %d.%d: Card object created from file %s deleted with no crashes",testNum,subTest, fileName);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tDeleteCardTests(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Testing deleteCard.  NOTE: see valgrind output for deletion memory leak report", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tDeleteCalTest1);
    return rec;
}
//***************************************************************