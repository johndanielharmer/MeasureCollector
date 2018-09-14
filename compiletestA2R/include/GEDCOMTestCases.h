#ifndef GEDCOM_TEST_CASES_H
#define GEDCOM_TEST_CASES_H

#include "TestHarness.h"

testRec* _tValidFileTest1(int testNum);
testRec* _tValidFileTest2(int testNum);
testRec* _tValidFileTest3(int testNum);
testRec* _tValidFileTest4(int testNum);
testRec* _tValidFileTest5(int testNum);
testRec* _tValidFileTest6(int testNum);
testRec* _tValidFileTest7(int testNum);
testRec* _tValidFileTest8(int testNum);
testRec* _tValidFileTest9(int testNum);
testRec* _tValidFileTest10(int testNum);
testRec* _tValidFileTest11(int testNum);

testRec* _tinvalidFileTests1(int testNum);
testRec* _tinvalidFileTests2(int testNum);
testRec* _tinvalidFileTests3(int testNum);
testRec* _tinvalidFileTests4(int testNum);

testRec* _tDeleteGEDCOMTest1(int testNum);
testRec* _tDeleteGEDCOMTest2(int testNum);
testRec* _tDeleteGEDCOMTest3(int testNum);
testRec* _tDeleteGEDCOMTest4(int testNum);

testRec* _tPrintGEDCOMTest(int testNum);
testRec* _tPrintGEDCOMTestNULL(int testNum);

testRec* _tPrintGEDCOMerrorTest(int testNum);

testRec* _tFindPersonTest(int testNum);
testRec* _tFindPersonTestNull(int testNum);

testRec* _tGetDescendantsTest1(int testNum);
testRec* _tGetDescendantsTest2(int testNum);
testRec* _tGetDescendantsTest3(int testNum);
testRec* _tGetDescendantsTest4(int testNum);
testRec* _tGetDescendantsTest5(int testNum);


//A2

//Module 1
testRec* _tWriteTest(int testNum);

testRec* _tWriteTest1(int testNum);
testRec* _tWriteTest2(int testNum);
testRec* _tWriteTest3(int testNum);
testRec* _tWriteTest4(int testNum);
testRec* _tWriteTest5(int testNum);
testRec* _tWriteTest6(int testNum);
testRec* _tWriteTest7(int testNum);
testRec* _tWriteTest8(int testNum);
testRec* _tWriteTest9(int testNum);

testRec* _tWriteTestWFile(int testNum);
testRec* _tWriteTestNull(int testNum);
testRec* _tValidateTest(int testNum);
testRec* _tValidateTestInv(int testNum);


//Module 2
testRec* _tGetDescNTestInvalidArguments(int testNum);
testRec* _tGetDescNTestValidEdgeCases(int testNum);
testRec* _tGetDescNTestBasicCorrect(int testNum);
testRec* _tGetDescNTestAdvancedCorrect(int testNum);

testRec* _tGetAnsNTestValidEdgeCases(int testNum);
testRec* _tGetAnsNTestAdvancedCorrect(int testNum);
testRec* _tGetAnsNTestBasicCorrect(int testNum);
testRec* _tGetAnsNTestInvalidArguments(int testNum);
testRec* _tGetAnsNTestValidEdgeCases(int testNum);

//Module 3
testRec* _tTestIndToJSON(int testNum);
testRec* _tTestIndToJSONInv(int testNum);

testRec* _tTestJSONtoInd(int testNum);
testRec* _tTestJSONtoIndInv(int testNum);

testRec* _tTestJSONtoGEDCOM(int testNum);
testRec* _tTestJSONtoGEDCOMInv(int testNum);

testRec* _tAddIndividual(int testNum);

testRec* _tTestiListToJSON(int testNum);
testRec* _tTestiListToJSONInv(int testNum);

testRec* _tTestgListToJSON(int testNum);
testRec* _tTestgListToJSONInv(int testNum);

//******************************** indToJSON ********************************




#endif
