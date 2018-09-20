#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** TEST DATA ********************************

static GEDCOMobject* _tSimpleValid(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+1);
    strcpy(refSub->submitterName, "Submitter");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "PAF");
    refHead->gedcVersion = 5.5;
    refHead->encoding = ASCII;
    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initialize Individuals and insert them into the list
    
    Individual* refInd1 = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd1->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd1);
    
    Individual* refInd2 = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd2->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd2);
    
    Individual* refInd3 = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd3->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd3);
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd1, refInd2);
    _tInsertBack(&refObj->families, refFam);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd3);
    
    _tInsertBack(&refInd1->families, refFam);
    _tInsertBack(&refInd2->families, refFam);
    _tInsertBack(&refInd3->families, refFam);
    
    return refObj;
}

//******************************** TEST CASES **********************************

//******************************** Write NULL ********************************

static SubTestRec _tWriteSubTest1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
	GEDCOMobject* obj = _tSimpleValid();
    
    GEDCOMerror response = writeGEDCOM(NULL, obj);
    
    if (response.type == WRITE_ERROR){
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL file name.",testNum, subTest);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to handle NULL file name.",testNum, subTest);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tWriteSubTest2(int testNum, int subTest){
    char feedback[300];
    char* fileName = "testFiles/output/someFile.ged";
    SubTestRec result;
    
    GEDCOMerror response = writeGEDCOM(fileName, NULL);
    
    if (response.type == WRITE_ERROR){
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL GEDCOMobject.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle NULL GEDCOMobject.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tWriteTestNull(int testNum){
    const int numSubs = 2;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: writeGEDCOM (NULL arguments)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tWriteSubTest2);
    return rec;
}

//******************************** Write Tests ********************************

static SubTestRec _tWriteSubTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* fileName = "testFiles/output/someFile.ged";
    GEDCOMobject* refObj = _tSimpleValid();
    GEDCOMobject* testObj;
    
    GEDCOMerror response = writeGEDCOM(fileName, refObj);
    createGEDCOM(fileName, &testObj);
    
    if (response.type != WRITE_ERROR && _tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Correctly saved GEDCOM object to file.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to save GEDCOM object to file.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tWriteTest(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: writeGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest3);
    return rec;
}

//******************************** Validate Tests (valid)  ********************************

static SubTestRec _tValidateSubTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* refObj = _tSimpleValid();
    
    ErrorCode response = validateGEDCOM(refObj);
    
    if (response == OK){
        sprintf(feedback, "Subtest %d.%d: Correctly validated a valid GEDCOM object.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to correctly validate a valid GEDCOM object.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidateTest(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: validateGEDCOM (valid objects)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tValidateSubTest1);
    return rec;
}

//******************************** Validate Tests (invalid)  ********************************

static SubTestRec _tValidateSubTestInv1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    ErrorCode response = validateGEDCOM(NULL);
    
    if (response != OK){
        sprintf(feedback, "Subtest %d.%d: Correctly validated an invalid GEDCOM object.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to correctly validate an invalid GEDCOM object.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidateTestInv(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: validateGEDCOM (invalid objects)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv1);
    return rec;
}


