#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** TEST DATA ********************************
static GEDCOMobject* _tJtoSData1(CharSet encoding){
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
    refHead->encoding = encoding;
    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    return refObj;
}

static GEDCOMobject* _tJtoSData2(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+1);
    strcpy(refSub->submitterName, "Some dude");
    strcpy(refSub->address, "");
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

static List _tEmptyIList(void){
    List l = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    return l;
}

static List _tEmptyGList(void){
    List l = _tInitializeList(&printGeneration, &deleteGeneration, &compareGenerations);
    return l;
}


static List _tIList1(void){
    List l = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    Individual* ind = _tCreateIndividual("William", "Shakespeare");
    insertBack(&l, ind);
    
    return l;
}

static List _tIList2(void){
    List l = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    Individual* ind = _tCreateIndividual("William", "Shakespeare");
    insertBack(&l, ind);
    
    ind = _tCreateIndividual("Anne", "Hathaway");
    insertBack(&l, ind);
    
    return l;
}
static char _tStrGList11[] = "[[{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}]]";
static List _tGList11(void){
    List l = _tInitializeList(&printGeneration, &deleteGeneration, &compareGenerations);
    
    //Add individual to generation
    
    
    List* g1 = malloc(sizeof(List));
    List tmp = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    *g1 = tmp;
    
    Individual* ind = _tCreateIndividual("William", "Shakespeare");
    insertBack(g1, ind);
    
    //Add generation to list
    insertBack(&l, g1);
    
    return l;
}

static char _tStrGList12[] = "[[{\"givenName\":\"William\",\"surname\":\"Shakespeare\"},{\"givenName\":\"Anne\",\"surname\":\"Hathaway\"}]]";
static List _tGList12(void){
    List l = _tInitializeList(&printGeneration, &deleteGeneration, &compareGenerations);
    
    //Add individuals to generation
    List* g1 = malloc(sizeof(List));
    List tmp = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    *g1 = tmp;
    
    Individual* ind = _tCreateIndividual("William", "Shakespeare");
    insertBack(g1, ind);
    
    ind = _tCreateIndividual("Anne", "Hathaway");
    insertBack(g1, ind);
    
    //Add generation to list
    insertBack(&l, g1);
    
    return l;
}

static char _tStrGList222[] = "[[{\"givenName\":\"Al\",\"surname\":\"Foo\"},{\"givenName\":\"Bill\",\"surname\":\"Foo\"}],[{\"givenName\":\"Jill\",\"surname\":\"Foo\"},{\"givenName\":\"June\",\"surname\":\"Foo\"}]]";
static List _tGList222(void){
    List l = _tInitializeList(&printGeneration, &deleteGeneration, &compareGenerations);
    
    //Add individuals to generation
    List* g1 = malloc(sizeof(List));
    List tmp = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    *g1 = tmp;
    
    Individual* ind = _tCreateIndividual("Al", "Foo");
    insertBack(g1, ind);
    
    ind = _tCreateIndividual("Bill", "Foo");
    insertBack(g1, ind);
    
    //Add generation to list
    insertBack(&l, g1);
    
    g1 = malloc(sizeof(List));
    tmp = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    *g1 = tmp;
    
    ind = _tCreateIndividual("Jill", "Foo");
    insertBack(g1, ind);
    
    ind = _tCreateIndividual("June", "Foo");
    insertBack(g1, ind);
    
    //Add generation to list
    insertBack(&l, g1);
    
    return l;
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

static SubTestRec _tIndToJSON2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","");
    
    char* testJSON = indToJSON(refObj);
    
    if (strcmp(testJSON, "{\"givenName\":\"William\",\"surname\":\"\"}") == 0){
        sprintf(feedback, "Subtest %d.%d: Correctly created JSON from Individual struct (empty surname)",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create JSON from Individual struct (empty surname)",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tIndToJSON3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("","Shakespeare");
    
    char* testJSON = indToJSON(refObj);
    
    if (strcmp(testJSON, "{\"givenName\":\"\",\"surname\":\"Shakespeare\"}") == 0){
        sprintf(feedback, "Subtest %d.%d: Correctly created JSON from Individual struct (empty given name)",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create JSON from Individual struct (empty given name)",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tIndToJSON4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","Shakespeare");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refObj->otherFields, refField);
    
    char* testJSON = indToJSON(refObj);
    
    if (strcmp(testJSON, "{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}") == 0){
        sprintf(feedback, "Subtest %d.%d: Correctly created JSON from Individual struct (nonempty otherFields)",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create JSON from Individual struct (nonempty otherFields)",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestIndToJSON(int testNum){
    const int numSubs = 4;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: indToJSON", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tIndToJSON1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tIndToJSON2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tIndToJSON3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tIndToJSON4);
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

static SubTestRec _tJSONtoInd2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("","Shakespeare");
    Individual* testObj;
    
    char testData[] = "{\"givenName\":\"\",\"surname\":\"Shakespeare\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoInd(testJSON);
    
    if (_tIndEqualShallow(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Correctly created Individual from JSON (empty given name).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create Individual from JSON (empty given name).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tJSONtoInd3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","");
    Individual* testObj;
    
    char testData[] = "{\"givenName\":\"William\",\"surname\":\"\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoInd(testJSON);
    
    if (_tIndEqualShallow(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Correctly created Individual from JSON (empty surname).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create Individual from JSON (empty surname).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoInd(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoInd", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tJSONtoInd2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tJSONtoInd3);
    return rec;
}

//******************************** JSONtoInd - Invalid args********************************

static SubTestRec _tJSONtoInd1Inv1(int testNum, int subTest){
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

static SubTestRec _tJSONtoInd1Inv2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* testObj;
    
    char testData[] = "";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoInd(testJSON);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Correctly handled invalid argument (empty string).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle invalid argument (empty string).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tJSONtoInd1Inv3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* testObj;
    
    char testData[] = "I am a random string!";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoInd(testJSON);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Correctly handled invalid argument (non-JSON string).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle invalid argument (non-JSON string).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoIndInv(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoInd (invalid arg)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1Inv1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1Inv2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1Inv3);
    return rec;
}


//******************************** JSONtoGEDCOM ********************************

static SubTestRec _tGtoJ1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
	GEDCOMobject* refObj = _tJtoSData1(ASCII);
    GEDCOMobject* testObj;
    
    char testData[] = "{\"source\":\"Blah\",\"gedcVersion\":\"5.5\",\"encoding\":\"ASCII\",\"subName\":\"Some dude\",\"subAddress\":\"nowhere\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Successfully created GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to create GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tGtoJ2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* refObj = _tJtoSData1(ANSEL);
    GEDCOMobject* testObj;
    
    char testData[] = "{\"source\":\"Blah\",\"gedcVersion\":\"5.5\",\"encoding\":\"ANSEL\",\"subName\":\"Some dude\",\"subAddress\":\"nowhere\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Successfully created GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGtoJ3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* refObj = _tJtoSData1(UNICODE);
    GEDCOMobject* testObj;
    
    char testData[] = "{\"source\":\"Blah\",\"gedcVersion\":\"5.5\",\"encoding\":\"UNICODE\",\"subName\":\"Some dude\",\"subAddress\":\"nowhere\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Successfully created GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGtoJ4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* refObj = _tJtoSData1(UTF8);
    GEDCOMobject* testObj;
    
    char testData[] = "{\"source\":\"Blah\",\"gedcVersion\":\"5.5\",\"encoding\":\"UTF-8\",\"subName\":\"Some dude\",\"subAddress\":\"nowhere\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Successfully created GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGtoJ5(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* refObj = _tJtoSData2();
    GEDCOMobject* testObj;
    
    char testData[] = "{\"source\":\"Blah\",\"gedcVersion\":\"5.5\",\"encoding\":\"ASCII\",\"subName\":\"Some dude\",\"subAddress\":\"\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Successfully created GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create GEDCOMobject from JSON %s.",testNum, subTest, testJSON);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoGEDCOM(int testNum){
    const int numSubs = 5;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGtoJ1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGtoJ2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGtoJ3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGtoJ4);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGtoJ5);
    
    return rec;
}


//******************************** JSONtoGEDCOM - Invalid args ********************************


static SubTestRec _tGtoJNULL(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj;
    
    testObj = JSONtoGEDCOM(NULL);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL argument.",testNum, subTest);
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

static SubTestRec _tGtoJInv1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj;
    
    char testData[] = "";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Successfully handled invalid argument (empty string).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle invalid argument (empty string).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGtoJInv2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj;
    
    char testData[] = "I am a random string!";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoGEDCOM(testJSON);
    
    if (testObj == NULL){
        sprintf(feedback, "Subtest %d.%d: Successfully handled invalid argument (non-JSON string).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle invalid argument (non-JSON string).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}


testRec* _tTestJSONtoGEDCOMInv(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoGEDCOM (NULL arguments)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGtoJNULL);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGtoJInv1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGtoJInv2   );
    return rec;
}

//******************************** addIndividual ********************************

static SubTestRec _tAddIndividual1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* testObj = _tJtoSData1(ASCII);
    
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
    GEDCOMobject* testObj = _tJtoSData1(ASCII);
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

//******************************** iListToJSON - invalid args ********************************

static SubTestRec _tIListToJInv1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    json = iListToJSON(_tEmptyIList());
    
    if (strcmp(json, "[]") == 0){
        sprintf(feedback, "Subtest %d.%d: Successfully handled empty list in iListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle empty list iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestiListToJSONInv(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: iListToJSON (empty list)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tIListToJInv1);
    return rec;
}

//******************************** iListToJSON ********************************

static SubTestRec _tIListToJ1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    json = iListToJSON(_tIList1());
    
    if (strcmp(json, "[{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}]") == 0){
        sprintf(feedback, "Subtest %d.%d: Converted a list with 1 individual using  iListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Incorrect conversion of a list with 1 individual using iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tIListToJ2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    json = iListToJSON(_tIList2());
    
    if (strcmp(json, "[{\"givenName\":\"William\",\"surname\":\"Shakespeare\"},{\"givenName\":\"Anne\",\"surname\":\"Hathaway\"}]") == 0){
        sprintf(feedback, "Subtest %d.%d: Converted a list with 2 individuals using  iListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Incorrect conversion of a list with 2 individuals using iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestiListToJSON(int testNum){
    const int numSubs = 2;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: iListToJSON", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tIListToJ1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tIListToJ2);
    return rec;
}

//******************************** gListToJSON ********************************


static SubTestRec _tGListToJ1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    List gList = _tGList11();
    
    json = gListToJSON(gList);
    
    if (strcmp(json, _tStrGList11) == 0){
        sprintf(feedback, "Subtest %d.%d: Converted a list with 1 generaton, 1 individual using  gListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Incorrect conversion of a list with 1 generaton, 1 individual using iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGListToJ2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    List gList = _tGList12();
    
    json = gListToJSON(gList);
    
    if (strcmp(json, _tStrGList12) == 0){
        sprintf(feedback, "Subtest %d.%d: Converted a list with 1 generaton, 2 individuals using  gListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Incorrect conversion of a list with 1 generaton, 2 individuals using iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGListToJ3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    List gList = _tGList222();
    
    json = gListToJSON(gList);
    
    if (strcmp(json, _tStrGList222) == 0){
        sprintf(feedback, "Subtest %d.%d: Converted a list with 2 generatons, (2 individuals/generation) using  gListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Incorrect conversion of a list with 2 generatons, (2 individuals/generation) using iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestgListToJSON(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: gListToJSON", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGListToJ1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGListToJ2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGListToJ3);
    return rec;
}

//******************************** gListToJSON - invalid args ********************************

static SubTestRec _tGListToJInv1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* json;
    
    json = gListToJSON(_tEmptyGList());
    
    if (strcmp(json, "[]") == 0){
        sprintf(feedback, "Subtest %d.%d: Successfully handled empty list in iListToJSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle empty list iListToJSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestgListToJSONInv(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: gListToJSON (empty list)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGListToJInv1);
    return rec;
}
