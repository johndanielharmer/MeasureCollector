#include "CardTestUtilities.h"

//******************************** TEST DATA ********************************

//Minimal valid vCard
Card* minValid(void)
{
    Card* card = malloc(sizeof(Card));
    
    //Set FN property
    card->fn = _tCreateTestProp("FN",NULL);
    _tAddPropValue(card->fn->values, "Simon Perreault");

    card->optionalProperties = _tInitializeList(&printProperty, &deleteProperty, &compareProperties);
    card->birthday = NULL;
    card->anniversary = NULL;
    
    return card;
}

Card* testCardPropSimpleVal(void)
{
    Card* card = malloc(sizeof(Card));
    
    //Set FN property
    card->fn = _tCreateTestProp("FN",NULL);
    _tAddPropValue(card->fn->values, "Simon Perreault");

    card->optionalProperties = _tInitializeList(&printProperty, &deleteProperty, &compareProperties);
    card->birthday = NULL;
    card->anniversary = NULL;

    Property* prop = _tCreateTestProp("GENDER",NULL);
    _tAddPropValue(prop->values, "M");
    _tInsertBack(card->optionalProperties, prop);
    
    return card;
}

Card* testCardPropsSimpleVal(void)
{
    Card* card = malloc(sizeof(Card));
    
    //Set FN property
    card->fn = _tCreateTestProp("FN",NULL);
    _tAddPropValue(card->fn->values, "Simon Perreault");

    card->optionalProperties = _tInitializeList(&printProperty, &deleteProperty, &compareProperties);
    card->birthday = NULL;
    card->anniversary = NULL;

    Property* prop = _tCreateTestProp("GENDER",NULL);
    _tAddPropValue(prop->values, "M");
    _tInsertBack(card->optionalProperties, prop);

    prop = _tCreateTestProp("ORG",NULL);
    _tAddPropValue(prop->values, "Viagenie");
    _tInsertBack(card->optionalProperties, prop);
    
    return card;
}

Card* cardFnN(void)
{
    Card* card = malloc(sizeof(Card));
    
    //Set FN property
    printf("cardFnN start\n");
    card->fn = _tCreateTestProp("FN",NULL);
    _tAddPropValue(card->fn->values, "Simon Perreault");

    card->optionalProperties = _tInitializeList(&printProperty, &deleteProperty, &compareProperties);


    Property* prop = _tCreateTestProp("N",NULL);
    _tAddPropValue(prop->values, "Perreault");
    _tAddPropValue(prop->values, "Simon");
    _tAddPropValue(prop->values, "");
    _tAddPropValue(prop->values, "");
    _tAddPropValue(prop->values, "ing. jr,M.Sc.");

    _tInsertBack(card->optionalProperties, prop);

    card->birthday = NULL;
    card->anniversary = NULL;
    printf("cardFnN end\n");
    return card;
}

//******************************** TEST CASES ********************************

/*
 - min: just FN X
 - Fn + 1 prop with simple value
 - Fn + multiple props with simple values
 - Fn + 1 prop with compound value X
 - Fn + multiple props with compound values
 - Fn + 1 prop with parameters
 - Fn + multiple props with parameters
 - Fn + 1 prop with paramaters and values
 - Fn + multiple props with parameters and values
 - Fn + 1 date
 - Fn + 2 dates
 - Fn + 2 dates and multiple complex properties
 - A few folded lines

 - ~4 invalids

 - ~4 deletes

 - ~a couple of prints and print error
*/

//******************************** vCard creation ********************************

//******************************** Test 1 ********************************


SubTestRec _tCreateCardTestGeneric(int testNum, int subTest, char* fileName, void* obj){
    SubTestRec result;
    char feedback[300];
    
    Card* testObj;
    Card* refObj = (Card*)obj;

    
    VCardErrorCode err = createCard(fileName, &testObj);

    // _tPrintObj("Test card: ", testObj);
    // _tPrintObj("Reference card: ", refObj);
    
    if (err != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file - created object does not match the reference object",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static testRec* _tValidFileTestGeneric(int testNum, char fileName[], Card* refObj){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    //char fileName[] = "testFiles/valid/testCardMin.vcf";
    sprintf(feedback, "Test %d: Testing createCard. Creating vCard object from a valid file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFileAndObj(testNum, subTest, rec, fileName, refObj, &_tCreateCardTestGeneric);
    return rec;
}

testRec* _tValidFileTest1(int testNum){

    return _tValidFileTestGeneric(testNum, "testFiles/valid/testCardMin.vcf", minValid());
}

testRec* _tValidFileTest2(int testNum){

    return _tValidFileTestGeneric(testNum, "testFiles/valid/testCardProp-simpleVal.vcf", testCardPropSimpleVal());
}

testRec* _tValidFileTest3(int testNum){

    return _tValidFileTestGeneric(testNum, "testFiles/valid/testCardProps-simpleVal.vcf", testCardPropsSimpleVal());
}

testRec* _tValidFileTest4(int testNum){

    return _tValidFileTestGeneric(testNum, "testFiles/valid/testCardN-compVal.vcf", cardFnN());
}


//*********** Error handling - invalid file ***********

//Non-existent file
static SubTestRec _tInvFileTest1(int testNum, int subTest){
    SubTestRec result;
    char feedback[300];
    Card* testObj;
    char fileName[] = "testFiles/iDoNotExist.vcf";
    
    VCardErrorCode err = createCard(fileName, &testObj);
    
    if (err == INV_FILE){
        sprintf(feedback, "Subtest %d.%d: Reading a non-existent file (%s) .",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: incorrect error code for non-existent file (%s)",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tInvalidFileTests(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Creating a vCard object from invalid files", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tInvFileTest1);
    return rec;
}
//***************************************************************