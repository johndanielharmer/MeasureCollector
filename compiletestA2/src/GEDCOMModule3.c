#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** TEST DATA ********************************
static GEDCOMobject* _tJtoSData1(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);

    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+1);
    strcpy(refSub->submitterName, "Some dude");
    strcpy(refSub->address, "nowhere");
    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "Blah");
    refHead->gedcVersion = 5.5;
    refHead->encoding = ASCII;
    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    return refObj;
}

//******************************** TEST CASES **********************************

//******************************** indToJSON ********************************

static SubTestRec _tIndToJSON1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","Shakespeare");
    
    char* testJSON = indToJSON(refObj);
    
    if (strcmp(testJSON, "{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}") == 0){
        sprintf(feedback, "Subtest %d.%d: Correctly created JSON from Individual struct.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create JSON from Individual struct.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestIndToJSON(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: indToJSON", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tIndToJSON1);
    return rec;
}

//******************************** indToJSON - Invalid args ********************************

static SubTestRec _tIndToJSONNull1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    char* testJSON = indToJSON(NULL);
    
    if (strcmp(testJSON, "") == 0){
        sprintf(feedback, "Subtest %d.%d: Correctly created JSON from NULL struct.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create JSON from NULL struct.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestIndToJSONInv(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: indToJSON (NULL)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tIndToJSONNull1);
    return rec;
}

//******************************** JSONtoInd ********************************

static SubTestRec _tJSONtoInd1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","Shakespeare");
    Individual* testObj;
    
    char testData[] = "{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoInd(testJSON);
    
    if (_tIndEqualShallow(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Correctly created Individual from JSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create Individual from JSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoInd(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoInd", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1);
    return rec;
}

//******************************** JSONtoInd - Invalid args********************************

static SubTestRec _tJSONtoInd1Inv(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* testObj;
    
    testObj = JSONtoInd(NULL);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Correctly handled NULL argument.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle NULL argument.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoIndInv(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoInd (invalid arg)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1Inv);
    return rec;
}


//******************************** JSONtoGEDCOM ********************************

static SubTestRec _tGtoJ1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
	GEDCOMobject* refObj = _tJtoSData1();
    GEDCOMobject* testObj;
    
    char testData[] = "{\"source\":\"Blah\",\"gedcVersion\":\"5.5\",\"encoding\":\"ASCII\",\"subName\":\"Some dude\",\"subAddress\":\"nowhere\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Successfully created GEDCOMobject from JSON.",testNum, subTest);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to create GEDCOMobject from JSON.",testNum, subTest);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

testRec* _tTestJSONtoGEDCOM(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGtoJ1);
    return rec;
}


//******************************** JSONtoGEDCOM - Invalid args ********************************


static SubTestRec _tGtoJNULL(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj;
    
    testObj = JSONtoGEDCOM(NULL);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL argument to JSONtoGEDCOM.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle NULL argument to JSONtoGEDCOM.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoGEDCOMInv(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoGEDCOM (NULL arguments)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGtoJNULL);
    return rec;
}

//******************************** addIndividual ********************************

static SubTestRec _tAddIndividual1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj = _tJtoSData1();
    
    addIndividual(testObj, NULL);
    
    if (testObj->individuals.head == NULL && testObj->individuals.tail == NULL){
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL Individual in addIndividual.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle NULL Individual object in addIndividual.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tAddIndividual2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* testObj = _tCreateIndividual("William","Shakespeare");
    
    addIndividual(NULL, testObj);
    
    sprintf(feedback, "Subtest %d.%d: Successfully handled NULL GEDCOM object in addIndividual.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tAddIndividual3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj = _tJtoSData1();
    Individual* testInd = _tCreateIndividual("William","Shakespeare");
    
    addIndividual(testObj, testInd);
    
    if (testObj->individuals.head->data == testInd && testObj->individuals.tail->data == testInd){
        sprintf(feedback, "Subtest %d.%d: Successfully added Individual to GEDCOMobject.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to add Individual to GEDCOMobject.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tAddIndividual(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: addIndividual", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tAddIndividual1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tAddIndividual2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tAddIndividual3);
    return rec;
}



