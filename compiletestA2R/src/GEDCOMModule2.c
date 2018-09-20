#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>

//******************************** TEST DATA ********************************

static bool compareIndividualsBool(const void* first,const void* second){
    if(first == second){
        return true;
    }
    
    if(first == NULL || second == NULL)
    {
        return false;
    }
    
    bool boolresult;
    int intresult;
    
    char* firstName = _tStrCatTst(NULL, ((Individual *) first)->givenName, '\0');
    firstName = _tStrCatTst(firstName, ",", '\0');
    firstName = _tStrCatTst(firstName, ((Individual *) first)->surname, '\0');
    
    char* secondName = _tStrCatTst(NULL, ((Individual *) second)->givenName, '\0');
    secondName = _tStrCatTst(secondName, ",", '\0');
    secondName = _tStrCatTst(secondName, ((Individual *) second)->surname, '\0');
    
    intresult = strcmp(firstName, secondName);
    if(intresult == 0)
    {
        boolresult = true;
    }
    else
    {
        boolresult = false;
    }
    
    free(firstName);
    free(secondName);
    
    return boolresult;
}

static bool _tIsListEmpty(List l){
    if (l.head->data == NULL || l.tail->data == NULL){
        return true;
    }else{
        return false;
    }
}

static bool _compareGList(List gList, int gListSize, int individualsSize[], char staticGList[30][30][2][255])
{
    int i, j;

    if(_tIsListEmpty(gList) == true)
    {
        return false;
    }

//
//    //Print test generation list
//    void * elem2;
//    ListIterator iter2 = createIterator(gList);
//    while ((elem2 = nextElement(&iter2)) != NULL){
//        char * print2 = printGeneration(elem2);
//        printf("%s\n", print2);
//        free(print2);
//    }
    
    i = 0;
    void * elem;
    ListIterator iter = createIterator(gList);
    while ((elem = nextElement(&iter)) != NULL){

        j = 0;
        void * elem2;
        ListIterator iter2 = createIterator(*((List *) elem));
        while ((elem2 = nextElement(&iter2)) != NULL){
            Individual * individual = elem2;

            if(i >= gListSize || j >= individualsSize[i])
            {
                return false;
            }

            char * firstName = staticGList[i][j][0];
            char * lastName = staticGList[i][j][1];

            Individual* refIndividual = _tCreateIndividual(firstName, lastName);

            if(!compareIndividualsBool(individual, refIndividual))
            {
                return false;
            }

            j++;
        }

        if(j != individualsSize[i])
        {
            return false;
        }

        i++;
    }

    return i == gListSize ? true : false;
}

static int _tListElemCount(List l){
    int count = 0;
    void* elem = NULL;
    
    ListIterator iter = createIterator(l);
    
    //Assume getLength() is broken/unimplemented
    while ((elem = nextElement(&iter)) != NULL){
        count += 1;
    }
    
    return count;
}

static GEDCOMobject* _tValidNoC(void){
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
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd1, refInd2);
    _tInsertBack(&refObj->families, refFam);
    
    //Set up mutual family and individual references
    _tInsertBack(&refInd1->families, refFam);
    _tInsertBack(&refInd2->families, refFam);
    
    return refObj;
}

static GEDCOMobject* _tOneIndivNoFam1(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
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
    
    Individual* refInd = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd);
    
    return refObj;
}

static GEDCOMobject* _tOneFamNoIndiv(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
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
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(NULL, NULL);
    _tInsertBack(&refObj->families, refFam);
    
    return refObj;
}

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

static GEDCOMobject* _tSimpleValidMultiGeneration(void){
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
    

    Individual* refInd4 = _tCreateIndividual("Cat", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd4->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd4);
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd1, refInd2);
    _tInsertBack(&refObj->families, refFam);

    Family* refFam2 = _tCreateFamily(refInd3, NULL);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd3);
    _tInsertBack(&refFam2->children, refInd4);
    
    _tInsertBack(&refInd1->families, refFam);
    _tInsertBack(&refInd2->families, refFam);
    _tInsertBack(&refInd3->families, refFam);
    _tInsertBack(&refInd3->families, refFam2);
    _tInsertBack(&refInd4->families, refFam2);
    
    return refObj;
}

static GEDCOMobject* _tSimpleValidMultiGeneration2(void){
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
    
    Individual* refInd4 = _tCreateIndividual("Cat", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd4->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd4);

    Individual* refInd5 = _tCreateIndividual("Jack", "Trent");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd5->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd5);
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd1, refInd2);
    _tInsertBack(&refObj->families, refFam);

    Family* refFam2 = _tCreateFamily(refInd3, NULL);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd3);
    _tInsertBack(&refFam->children, refInd5);
    _tInsertBack(&refFam2->children, refInd4);
    
    _tInsertBack(&refInd1->families, refFam);
    _tInsertBack(&refInd2->families, refFam);
    _tInsertBack(&refInd3->families, refFam);
    _tInsertBack(&refInd5->families, refFam);

    _tInsertBack(&refInd3->families, refFam2);
    _tInsertBack(&refInd4->families, refFam2);
    
    return refObj;
}

static GEDCOMobject* _tFamRemarriedMother(void){
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
    
    Individual* refInd3 = _tCreateIndividual("Fred", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd3->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd3);
    
    Individual* refInd4 = _tCreateIndividual("Mary", "Smith");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd4->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd4);
    
    Individual* refInd5 = _tCreateIndividual("Sterling", "Archer");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd5->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd5);
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd1, refInd2);
    _tInsertBack(&refObj->families, refFam);
    
    Family* refFam2 = _tCreateFamily(refInd3, refInd4);
    _tInsertBack(&refObj->families, refFam2);
    
    Family* refFam3 = _tCreateFamily(refInd5, refInd4);
    _tInsertBack(&refObj->families, refFam3);
    
    //Set up mutual family and individual references
    
    //Fam1 has no children
    
    //Ind1 is the child in 2 families: fam 2 and fam 3
    _tInsertBack(&refFam2->children, refInd1);
    _tInsertBack(&refFam3->children, refInd1);
    
    //Ind1 belongs to 3 families: husband in fam 1, son in fam 2 and fam 3
    _tInsertBack(&refInd1->families, refFam);
    _tInsertBack(&refInd1->families, refFam2);
    _tInsertBack(&refInd1->families, refFam3);
    
    //Ind2 belongs to 1 family: wife in fam 1
    _tInsertBack(&refInd2->families, refFam);
    
    //Ind3 belongs to 2 families: father in fam 1, husband in fam 2
    _tInsertBack(&refInd3->families, refFam);
    _tInsertBack(&refInd3->families, refFam2);
    
    //Ind4 belongs to 3 families: mother in fam 1, wife in fam 2 and fam 3
    _tInsertBack(&refInd4->families, refFam);
    _tInsertBack(&refInd4->families, refFam2);
    _tInsertBack(&refInd4->families, refFam3);
    
    //Ind5 belongs to 2 families: father in fam 2, husband in fam 3
    _tInsertBack(&refInd5->families, refFam2);
    _tInsertBack(&refInd5->families, refFam3);
    
    return refObj;
}

//******************************** TEST CASES **********************************

//******************************** getDescendantListN ********************************

static SubTestRec _tGetDescSubTest1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValid();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 2);
   
    int gListSize[] = {1};
    char staticGList[30][30][2][255] = {
        {
            {"James", "Smith"},
        }
    };

    if(!_compareGList(descendants, 1, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list for SimpleValid case, single generation, James Smith",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants (1 generation, 1 individual - James Smith).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetDescSubTest2(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValidMultiGeneration();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 2);
   
    int gListSize[] = {1, 1};
    char staticGList[30][30][2][255] = {
        {
            {"James", "Smith"}
        },
        {
            {"Cat", "Smith"}
        }
    };

    if(!_compareGList(descendants, 2, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list for SimpleValid case, 2 generations",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants (2 generations, 2 individuals - [[James Smith], [Cat Smith]]).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetDescSubTest3(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValidMultiGeneration2();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 2);
   
    int gListSize[] = {2, 1};
    char staticGList[30][30][2][255] = {
        {
            {"James", "Smith"},
            {"Jack", "Trent"}
        },
        {
            {"Cat", "Smith"}
        }
    };

    if(!_compareGList(descendants, 2, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list for SimpleValid case, 2 generations, [2 children, 1 child]",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants (2 generations, 3 individuals - [[[James Smith], [Jack Trent]], [[Cat Smith]]]).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetDescAdvancedSubTest1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;

    GEDCOMobject* obj = NULL;

    createGEDCOM("testFiles/valid/shakespeare.ged", &obj);
    
    Individual* refIndividual = _tCreateIndividual("William", "Shakespeare");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 2);
   
    int gListSize[] = {3, 4};
    char staticGList[30][30][2][255] = {
        {
            {"Hamnet", "Shakespeare"},
            {"Judith", "Shakespeare"},
            {"Susanna", "Shakespeare"}
        },
        {
            {"Richard", "Quiney"},
            {"Shakespeare", "Quiney"},
            {"Thomas", "Quiney"},
            {"Elizabeth", "Shakespeare"}
        }
    };

    if(!_compareGList(descendants, 2, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list William Shakespeare in shakespeare.ged",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        deleteGEDCOM(obj);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants for William Shakespeare in shakespeare.ged",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    deleteGEDCOM(obj);
    return result;
}

static SubTestRec _tGetDescAdvancedSubTest2(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;

    GEDCOMobject* obj = NULL;

    createGEDCOM("testFiles/valid/shakespeare.ged", &obj);
    
    Individual* refIndividual = _tCreateIndividual("Mary", "Arden");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 1);
   
    int gListSize[] = {8};
    char staticGList[30][30][2][255] = {
        {
            {"Anne", "Shakespeare"},
            {"Edmund", "Shakespeare"},
            {"Gilbert", "Shakespeare"},
            {"Joan", "Shakespeare"},
            {"Joan", "Shakespeare"},
            {"Margaret", "Shakespeare"},
            {"Richard", "Shakespeare"},
            {"William", "Shakespeare"},
        }
    };

    if(!_compareGList(descendants, 1, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list Mary Arden in shakespeare.ged",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        deleteGEDCOM(obj);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants for Mary Arden in shakespeare.ged",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    deleteGEDCOM(obj);
    return result;
}

static SubTestRec _tGetDescAdvancedSubTest3(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;

    GEDCOMobject* obj = NULL;

    createGEDCOM("testFiles/valid/shakespeare.ged", &obj);
    
    Individual* refIndividual = _tCreateIndividual("Mary", "Arden");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 0);
   
    int gListSize[] = {8, 3, 4};
    char staticGList[30][30][2][255] = {
        {
            {"Anne", "Shakespeare"},
            {"Edmund", "Shakespeare"},
            {"Gilbert", "Shakespeare"},
            {"Joan", "Shakespeare"},
            {"Joan", "Shakespeare"},
            {"Margaret", "Shakespeare"},
            {"Richard", "Shakespeare"},
            {"William", "Shakespeare"},
        },
        {
            {"Hamnet", "Shakespeare"},
            {"Judith", "Shakespeare"},
            {"Susanna", "Shakespeare"}
        },
        {
            {"Richard", "Quiney"},
            {"Shakespeare", "Quiney"},
            {"Thomas", "Quiney"},
            {"Elizabeth", "Shakespeare"}
        }
    };

    if(!_compareGList(descendants, 3, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list Mary Arden in shakespeare.ged with maxGen 0",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        deleteGEDCOM(obj);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants for Mary Arden in shakespeare.ged with maxGen 0",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    deleteGEDCOM(obj);
    return result;
}

testRec* _tGetDescNTestBasicCorrect(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendantListN with basic files", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);

    runSubTest(testNum, subTest, rec, &_tGetDescSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescSubTest3);

    return rec;
}

testRec* _tGetDescNTestAdvancedCorrect(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendantListN (Advanced) with shakespeare.ged", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescAdvancedSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescAdvancedSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescAdvancedSubTest3);
    return rec;
}

//******************************** getDescendantListN - edge cases ********************************

//Family with no children
static SubTestRec _tGetDescSubTestEdge1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tValidNoC();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List descendants = getDescendantListN(obj, individual, 2);
    
    if(descendants.head != NULL || descendants.tail != NULL ){
        sprintf(feedback, "Subtest %d.%d: Incorrect  (non-empty) descendant list for family with no children",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct (empty) list of descendants for family with no children.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

//GEDCOM with no families
static SubTestRec _tGetDescSubTestEdge2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tOneIndivNoFam1();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    List descendants = getDescendantListN(obj, refIndividual, 2);
    
    if(descendants.head != NULL || descendants.tail != NULL ){
        sprintf(feedback, "Subtest %d.%d: Incorrect  (non-empty) descendant list for GEDCOM object with no families",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct (empty) list of descendants for GEDCOM with no families.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

//GEDCOM with no individuals
static SubTestRec _tGetDescSubTestEdge3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tOneFamNoIndiv();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    List descendants = getDescendantListN(obj, refIndividual, 2);
    
    if(descendants.head != NULL || descendants.tail != NULL ){
        sprintf(feedback, "Subtest %d.%d: Incorrect (non-empty) descendant list for GEDCOM object with no individuals",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct (empty) list of descendants for GEDCOM with no individuals.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tGetDescNTestValidEdgeCases(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendantListN (edge cases)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescSubTestEdge1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescSubTestEdge2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescSubTestEdge3);
    return rec;
}


//******************************** getDescendantListN ********************************

static SubTestRec _tGetDescNTestInvalidArgsSubTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* obj = _tSimpleValid();
    
    List descendants = getDescendantListN(obj, NULL, 1);
    
    if (descendants.head == NULL && descendants.tail == NULL){
        sprintf(feedback, "Subtest %d.%d: returned an empty list when object is valid, person is NULL, and maxGen >= 0",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: failed to return an empty list when object is valid, person is NULL, and maxGen >= 0",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGetDescNTestInvalidArgsSubTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* obj = NULL;
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    getDescendantListN(obj, individual, 1);
    
    sprintf(feedback, "Subtest %d.%d: did not crash when object is NULL, person is valid, and maxGen >= 0",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetDescNTestInvalidArgsSubTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* obj = _tSimpleValid();;
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    getDescendantListN(obj, individual, -1);
    
    sprintf(feedback, "Subtest %d.%d: did not crash when object is valid, person is valid, and maxGen < 0",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tGetDescNTestInvalidArguments(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendantListN (invalid arguments)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescNTestInvalidArgsSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescNTestInvalidArgsSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescNTestInvalidArgsSubTest3);
    return rec;
}

//******************************** getAncestorListN - invalid ********************************

static SubTestRec _tGetAnsSubTest1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValid();
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List anscestors = getAncestorListN(obj, individual, 2);
   
    int gListSize[] = {2};
    char staticGList[30][30][2][255] = {
        {
            {"John", "Smith"},
            {"Elizabeth", "Stansfield"}
        }
    };
    if(!_compareGList(anscestors, 1, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors list for SimpleValid case, single generation, John Smith, Elizabeth Stansfield",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors (1 generation, 2 individuals - John Smith, Elizabeth Stansfield).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetAnsSubTest2(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValidMultiGeneration();
    
    Individual* refIndividual = _tCreateIndividual("Cat", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List anscestors = getAncestorListN(obj, individual, 2);
   
    int gListSize[] = {1, 2};
    char staticGList[30][30][2][255] = {
        {
            {"James", "Smith"}
        },
        {
            {"John", "Smith"},
            {"Elizabeth", "Stansfield"}
        }
    };

    if(!_compareGList(anscestors, 2, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors list for SimpleValid case, (2 generations, [1 father, 2 grandparents])",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors (2 generations, [1 father, 2 grandparents]).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetAnsSubTest3(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValidMultiGeneration2();
    
    Individual* refIndividual = _tCreateIndividual("Cat", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List anscestors = getAncestorListN(obj, individual, 2);
   
    int gListSize[] = {1, 2};
    char staticGList[30][30][2][255] = {
        {
            {"James", "Smith"}
        },
        {
            {"John", "Smith"},
            {"Elizabeth", "Stansfield"}
        }
    };

    if(!_compareGList(anscestors, 2, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors list for SimpleValid case, (2 generations, [1 father, 2 grandparents])",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors (2 generations, [1 father, 2 grandparents]).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tGetAnsNTestBasicCorrect(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getAncestorListN", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTest3);

    return rec;
}

//******************************** getAncestorListN - advanced cases ********************************

static SubTestRec _tGetAnsSubTestAdv1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tFamRemarriedMother();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List anscestors = getAncestorListN(obj, individual, 2);
    
    int gListSize[] = {3};
    char staticGList[30][30][2][255] = {
        {
            {"Sterling", "Archer"},
            {"Fred", "Smith"},
            {"Mary", "Smith"}
        }
    };
    int refNumAns = 3;
    List* testL = (List*)anscestors.head->data;
    int testNumAns = _tListElemCount(*testL);
    if(!_compareGList(anscestors, 1, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors list for a case where individual has more than 2 parents (expected %d parents, received %d) ",testNum, subTest, refNumAns, testNumAns);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors for a case where individual has more than 2 parents [mother, father, stepfather]).",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetAnsSubTestAdv2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = NULL;
    
    createGEDCOM("testFiles/valid/shakespeare.ged", &obj);
    
    Individual* refIndividual = _tCreateIndividual("Elizabeth", "Shakespeare");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List anscestors = getAncestorListN(obj, individual, 0);
    
    int gListSize[] = {2, 2, 3, 4};
    char staticGList[30][30][2][255] = {
        {
            {"John", "Hall"},
            {"Susanna", "Shakespeare"}
        },
        {
            {"Anne", "Hathaway"},
            {"William", "Shakespeare"}
        },
        {
            {"Mary", "Arden"},
            {"Richard", "Hathaway"},
            {"John", "Shakespeare"}
        },
        {
            {"Robert", "Arden"},
            {"Richard", "Shakespeare"},
            {"", "Unknown"},
            {"Agnes", "Webbe"}
        }
    };
    
    if(!_compareGList(anscestors, 4, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors list long, Elizabeth Shakespeare",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        deleteGEDCOM(obj);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors for a case where long list (3 ancestor generations) of ancestors, Elizabeth Shakespeare.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    deleteGEDCOM(obj);
    return result;
}

static SubTestRec _tGetAnsSubTestAdv3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = NULL;
    
    createGEDCOM("testFiles/valid/shakespeare.ged", &obj);
    
    Individual* refIndividual = _tCreateIndividual("Judith", "Shakespeare");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List anscestors = getAncestorListN(obj, individual, 2);
    
    int gListSize[] = {2, 3};
    char staticGList[30][30][2][255] = {
        {
            {"Anne", "Hathaway"},
            {"William", "Shakespeare"}
        },
        {
            {"Mary", "Arden"},
            {"Richard", "Hathaway"},
            {"John", "Shakespeare"}
        }
    };
    
    if(!_compareGList(anscestors, 2, gListSize, staticGList)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors list long, Judith Shakespeare",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        deleteGEDCOM(obj);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors for a case where long list of ancestors (2 ancestor generations), Judith Shakespeare.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    deleteGEDCOM(obj);
    return result;
}

testRec* _tGetAnsNTestAdvancedCorrect(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getAncestorListN", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTestAdv1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTestAdv2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTestAdv3);
    
    return rec;
}

//******************************** getAncestorListN - edge cases ********************************

//Family with no children
static SubTestRec _tGetAnsSubTestEdge1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tValidNoC();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List descendants = getAncestorListN(obj, individual, 2);
    
    if(descendants.head != NULL || descendants.tail != NULL ){
        sprintf(feedback, "Subtest %d.%d: Incorrect  (non-empty) ancestor list for family with no children",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct (empty) list of ancestors for family with no children.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

//GEDCOM with no families
static SubTestRec _tGetAnsSubTestEdge2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tOneIndivNoFam1();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    List descendants = getAncestorListN(obj, refIndividual, 2);
    
    if(descendants.head != NULL || descendants.tail != NULL ){
        sprintf(feedback, "Subtest %d.%d: Incorrect  (non-empty) ancestor list for GEDCOM object with no families",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct (empty) list of ancestors for GEDCOM with no families.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

//GEDCOM with no individuals
static SubTestRec _tGetAnsSubTestEdge3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tOneFamNoIndiv();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    List descendants = getAncestorListN(obj, refIndividual, 2);
    
    if(descendants.head != NULL || descendants.tail != NULL ){
        sprintf(feedback, "Subtest %d.%d: Incorrect (non-empty) ancestor list for GEDCOM object with no individuals",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    sprintf(feedback, "Subtest %d.%d: Returned correct (empty) list of ancestors for GEDCOM with no individuals.",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tGetAnsNTestValidEdgeCases(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getAncestorListN (edge cases)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTestEdge1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTestEdge2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTestEdge3);
    return rec;
}

//******************************** getAncestorListN - invalid args ********************************

static SubTestRec _tGetAnsNTestInvalidArgsSubTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* obj = _tSimpleValid();
    
    List anscestors = getAncestorListN(obj, NULL, 1);
    
    if (anscestors.head == NULL && anscestors.tail == NULL){
        sprintf(feedback, "Subtest %d.%d: returned an empty list when object is valid, person is NULL, and maxGen >= 0",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: failed to return an empty list when object is valid, person is NULL, and maxGen >= 0",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGetAnsNTestInvalidArgsSubTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* obj = NULL;
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    getAncestorListN(obj, individual, 1);
    
    sprintf(feedback, "Subtest %d.%d: did not crash when object is NULL, person is valid, and maxGen >= 0",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

static SubTestRec _tGetAnsNTestInvalidArgsSubTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    GEDCOMobject* obj = _tSimpleValid();;
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    getAncestorListN(obj, individual, -1);
    
    sprintf(feedback, "Subtest %d.%d: did not crash when object is valid, person is valid, and maxGen < 0",testNum, subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tGetAnsNTestInvalidArguments(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getAncestorListN (invalid arguments)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetAnsNTestInvalidArgsSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsNTestInvalidArgsSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetAnsNTestInvalidArgsSubTest3);
    return rec;
}

