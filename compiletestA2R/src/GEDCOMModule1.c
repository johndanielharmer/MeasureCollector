#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** TEST DATA - valid GEDCOM objects ********************************

static GEDCOMobject* _tMinValid1(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
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
    
    return refObj;
}

static GEDCOMobject* _tMinValid2(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
    strcpy(refSub->submitterName, "Submitter");
    strcpy(refSub->address, "Some sort of an address");
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

static GEDCOMobject* _tIndividualsNoFam(void){
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
    
    
    refInd = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd);
    
    refInd = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
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

static GEDCOMobject* _tSimpleValidWSurnGiven(void){
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
    
    Individual* refInd1 = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("GIVN", "John");
    _tInsertBack(&refInd1->otherFields, refField);
    
    refField = _tCreateField("SURN", "Smith");
    _tInsertBack(&refInd1->otherFields, refField);
    
    refField = _tCreateField("SEX", "M");
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

static GEDCOMobject* _tNuclearFamily(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
    strcpy(refSub->submitterName, "webTreePrint");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "webtreeprint.com");
    refHead->gedcVersion = 5.5;
    refHead->encoding = UTF8;
    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initialize Individuals and insert them into the list
    Individual* refInd[5];
    
    
    //Ind 1
    refInd[0] = _tCreateIndividual("William", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[0]);
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    Event* refEvt = _tCreateEvent("BIRT", "BEF 23 APR 1564", "Stratford-upon-Avon");
    _tInsertBack(&refInd[0]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "26 APR 1564", "Stratford-upon-Avon");
    _tInsertBack(&refInd[0]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "23 APR 1616", "Stratford-upon-Avon");
    _tInsertBack(&refInd[0]->events, refEvt);
    
    
    //Ind 2
    refInd[1] = _tCreateIndividual("Anne", "Hathaway");
    _tInsertBack(&refObj->individuals, refInd[1]);
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "APR 1556", "Shottery, Warwickshire");
    _tInsertBack(&refInd[1]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "06 AUG 1623", "Stratford-upon-Avon");
    _tInsertBack(&refInd[1]->events, refEvt);
    
    
    //Ind 3
    refInd[2] = _tCreateIndividual("Susanna", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[2]);
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "MAY 1583", "Stratford-upon-Avon");
    _tInsertBack(&refInd[2]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "26 MAY 1583", "Stratford-upon-Avon");
    _tInsertBack(&refInd[2]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "11 JUL 1649", "Stratford-upon-Avon");
    _tInsertBack(&refInd[2]->events, refEvt);
    
    
    //Ind 4
    refInd[3] = _tCreateIndividual("Hamnet", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[3]);
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[3]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "JAN 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "02 FEB 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "AUG 1596", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    refEvt = _tCreateEvent("BURI", "11 AUG 1596", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    
    //Ind 5
    refInd[4] = _tCreateIndividual("Judith", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[4]);
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[4]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "JAN 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "02 FEB 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "FEB 1662", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    refEvt = _tCreateEvent("BURI", "09 FEB 1662", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    
    //Initialize Families and insert them into the list
    
    //Fam 1
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    refEvt = _tCreateEvent("MARR", "NOV 1582", "");
    _tInsertBack(&refFam->events, refEvt);
    
    _tInsertBack(&refObj->families, refFam);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    _tInsertBack(&refFam->children, refInd[3]);
    _tInsertBack(&refFam->children, refInd[4]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    _tInsertBack(&refInd[3]->families, refFam);
    _tInsertBack(&refInd[4]->families, refFam);
    
    
    return refObj;
}

static GEDCOMobject* _tSimpleValid2Gen(void){
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
    Individual* refInd[8];
    
    refInd[0] = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[0]);
    
    refInd[1] = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[1]);
    
    
    refInd[2] = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[2]);
    
    
    refInd[3] = _tCreateIndividual("Jane", "Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[3]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[3]);
    
    
    refInd[4] = _tCreateIndividual("Jeff", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[4]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[4]);
    
    
    refInd[5] = _tCreateIndividual("Jill", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[5]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[5]);
    
    
    refInd[6] = _tCreateIndividual("Jack", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[6]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[6]);
    
    refInd[7] = _tCreateIndividual("June", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[7]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[7]);
    
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    _tInsertBack(&refObj->families, refFam);
    
    
    Family* refFam2 = _tCreateFamily(refInd[2], refInd[3]);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    
    _tInsertBack(&refFam2->children, refInd[4]);
    _tInsertBack(&refFam2->children, refInd[5]);
    _tInsertBack(&refFam2->children, refInd[6]);
    _tInsertBack(&refFam2->children, refInd[7]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    
    _tInsertBack(&refInd[2]->families, refFam2);
    _tInsertBack(&refInd[3]->families, refFam2);
    
    _tInsertBack(&refInd[4]->families, refFam2);
    _tInsertBack(&refInd[5]->families, refFam2);
    _tInsertBack(&refInd[6]->families, refFam2);
    _tInsertBack(&refInd[7]->families, refFam2);
    
    
    return refObj;
}


static GEDCOMobject* _tSimpleValid2GenE1(void){
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
    Individual* refInd[8];
    
    refInd[0] = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[0]);
    
    refInd[1] = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[1]);
    
    
    refInd[2] = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[2]);
    
    
    refInd[4] = _tCreateIndividual("Jeff", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[4]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[4]);
    
    
    refInd[5] = _tCreateIndividual("Jill", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[5]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[5]);
    
    
    refInd[6] = _tCreateIndividual("Jack", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[6]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[6]);
    
    refInd[7] = _tCreateIndividual("June", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[7]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[7]);
    
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    _tInsertBack(&refObj->families, refFam);
    
    
    Family* refFam2 = _tCreateFamily(refInd[2], NULL);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    
    _tInsertBack(&refFam2->children, refInd[4]);
    _tInsertBack(&refFam2->children, refInd[5]);
    _tInsertBack(&refFam2->children, refInd[6]);
    _tInsertBack(&refFam2->children, refInd[7]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    
    _tInsertBack(&refInd[2]->families, refFam2);
    _tInsertBack(&refInd[4]->families, refFam2);
    _tInsertBack(&refInd[5]->families, refFam2);
    _tInsertBack(&refInd[6]->families, refFam2);
    _tInsertBack(&refInd[7]->families, refFam2);
    
    
    return refObj;
}

static GEDCOMobject* _tSimpleValid2GenE2(void){
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
    Individual* refInd[4];
    
    refInd[0] = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[0]);
    
    refInd[1] = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[1]);
    
    
    refInd[2] = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[2]);
    
    
    refInd[3] = _tCreateIndividual("Jane", "Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[3]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[3]);
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    _tInsertBack(&refObj->families, refFam);
    
    
    Family* refFam2 = _tCreateFamily(refInd[2], refInd[3]);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    
    _tInsertBack(&refInd[2]->families, refFam2);
    _tInsertBack(&refInd[3]->families, refFam2);
    
    
    return refObj;
}

//******************************** TEST DATA - invalid GEDCOM objects ********************************

//***************** INV_GEDCOM *****************

//NULL Header ptr
static GEDCOMobject* _tInvalidNoHead(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
    strcpy(refSub->submitterName, "Submitter");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
   
    refObj->header = NULL;
    
    return refObj;
}

//NULL Submitter PTR ptr
static GEDCOMobject* _tInvalidNoSubm(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    refObj->submitter = NULL;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "PAF");
    refHead->gedcVersion = 5.5;
    refHead->encoding = ASCII;
    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    Submitter* refSub = malloc(sizeof(Submitter)+255);
    strcpy(refSub->submitterName, "Submitter");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    return refObj;
}

//***************** INV_HEADER *****************

//one of the fields in Header is not initialized (empty otherFields list is OK)

static GEDCOMobject* _tInvalidEmptySource(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
    strcpy(refSub->submitterName, "Submitter");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "");
    refHead->gedcVersion = 5.5;
    refHead->encoding = ASCII;
    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    return refObj;
}

static GEDCOMobject* _tInvalidNullSubmInHead(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
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
    refHead->submitter = NULL;
    
    refObj->header = refHead;
    
    return refObj;
}

//Provide a couple of cases

//***************** INV_RECORD  *****************

// Submitter name is empty
static GEDCOMobject* _tInvalidEmptySubmitterName(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+255);
    strcpy(refSub->submitterName, "");
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
    
    return refObj;
}

// One of the pointers that you need to dereference when saving a GEDCOM object to file is NULL (e.g. a NULL pointer to a Family in the families list)
//static GEDCOMobject* _tInvalidNULLPtrInIndFamilies(void){
//    //Initilize reference object
//    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
//
//    //Initilize Submitter obj
//    Submitter* refSub = malloc(sizeof(Submitter)+255);
//    strcpy(refSub->submitterName, "Submitter");
//    strcpy(refSub->address, "");
//    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
//
//    refObj->submitter = refSub;
//
//    //Initilize Header obj
//    Header* refHead = malloc(sizeof(Header));
//
//    strcpy(refHead->source, "PAF");
//    refHead->gedcVersion = 5.5;
//    refHead->encoding = ASCII;
//    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
//    refHead->submitter = refSub;
//
//    refObj->header = refHead;
//
//    //Initilize lists
//    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
//    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
//
//    //Initialize Individuals and insert them into the list
//
//    Individual* refInd1 = _tCreateIndividual("John", "Smith");
//    Field* refField = _tCreateField("SEX", "M");
//    _tInsertBack(&refInd1->otherFields, refField);
//    _tInsertBack(&refObj->individuals, refInd1);
//
//    Individual* refInd2 = _tCreateIndividual("Elizabeth", "Stansfield");
//    refField = _tCreateField("SEX", "F");
//    _tInsertBack(&refInd2->otherFields, refField);
//    _tInsertBack(&refObj->individuals, refInd2);
//
//    Individual* refInd3 = _tCreateIndividual("James", "Smith");
//    refField = _tCreateField("SEX", "M");
//    _tInsertBack(&refInd3->otherFields, refField);
//    _tInsertBack(&refObj->individuals, refInd3);
//
//
//    //Initialize Families and insert them into the list
//    Family* refFam = _tCreateFamily(refInd1, refInd2);
//    _tInsertBack(&refObj->families, refFam);
//
//    //Set up mutual family and individual references
//
//    _tInsertBack(&refFam->children, refInd3);
//
//    _tInsertBack(&refInd1->families, NULL);
//    _tInsertBack(&refInd2->families, refFam);
//    _tInsertBack(&refInd3->families, refFam);
//
//    return refObj;
//}
//
//static GEDCOMobject* _tInvalidNULLPtrInIndividuals(void){
//    //Initilize reference object
//    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
//
//    //Initilize Submitter obj
//    Submitter* refSub = malloc(sizeof(Submitter)+255);
//    strcpy(refSub->submitterName, "Submitter");
//    strcpy(refSub->address, "");
//    refSub->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
//
//    refObj->submitter = refSub;
//
//    //Initilize Header obj
//    Header* refHead = malloc(sizeof(Header));
//
//    strcpy(refHead->source, "PAF");
//    refHead->gedcVersion = 5.5;
//    refHead->encoding = ASCII;
//    refHead->otherFields = _tInitializeList(&printField, &deleteField, &compareFields);
//    refHead->submitter = refSub;
//
//    refObj->header = refHead;
//
//    //Initilize lists
//    refObj->families = _tInitializeList(&printFamily, &deleteFamily, &compareFamilies);
//    refObj->individuals = _tInitializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
//
//    //Initialize Individuals and insert them into the list
//
//    Individual* refInd1 = _tCreateIndividual("John", "Smith");
//    Field* refField = _tCreateField("SEX", "M");
//    _tInsertBack(&refInd1->otherFields, refField);
//    _tInsertBack(&refObj->individuals, refInd1);
//
//    Individual* refInd2 = _tCreateIndividual("Elizabeth", "Stansfield");
//    refField = _tCreateField("SEX", "F");
//    _tInsertBack(&refInd2->otherFields, refField);
//    _tInsertBack(&refObj->individuals, refInd2);
//
//    _tInsertBack(&refObj->individuals, NULL);
//
//
//    //Initialize Families and insert them into the list
//    Family* refFam = _tCreateFamily(refInd1, refInd2);
//    _tInsertBack(&refObj->families, NULL);
//
//    //Set up mutual family and individual references
//
//    //_tInsertBack(&refFam->children, refInd3);
//
//    _tInsertBack(&refInd1->families, refFam);
//    _tInsertBack(&refInd2->families, refFam);
//    //_tInsertBack(&refInd3->families, refFam);
//
//    return refObj;
//}

//A value of some field is too long > 200 characters



//******************************** TEST CASES **********************************

//******************************** Write NULL ********************************

static SubTestRec _tWriteSubTestInv1(int testNum, int subTest){
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

static SubTestRec _tWriteSubTestInv2(int testNum, int subTest){
    char feedback[300];
    char* fileName = "testFiles/output/someFileNULL.ged";
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
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTestInv1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tWriteSubTestInv2);
    return rec;
}

//******************************** Write Tests ********************************

static SubTestRec _tWriteSubTest(int testNum, int subTest, GEDCOMobject*(*_tCreateObject)(void), char* messStr){
    char feedback[300];
    SubTestRec result;
    char* fileName = "testFiles/output/someFile.ged";
    
    //Create reference object in memory
    GEDCOMobject* refObj = _tCreateObject();
    
    
    //Write object to file
    GEDCOMerror response = writeGEDCOM(fileName, refObj);
    if (response.type == WRITE_ERROR){
        sprintf(feedback, "Subtest %d.%d: writeGEDCOM returned WRITE_ERROR while trying to save a valid file to a vaild location (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
     //Create test object from file using student code
    GEDCOMobject* testObj;
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test writeGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    //Compare objects
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: %s.",testNum, subTest, messStr);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to %s.",testNum, subTest, messStr);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tWriteSubTest2Obj(int testNum, int subTest, GEDCOMobject*(*_tCreateObject1)(void), GEDCOMobject*(*_tCreateObject2)(void), char* messStr){
    char feedback[300];
    SubTestRec result;
    char* fileName = "testFiles/output/someFile.ged";
    
    //Create primary reference object in memory
    GEDCOMobject* refObj1 = _tCreateObject1();
    
    //Create alternative reference object in memory
    GEDCOMobject* refObj2 = _tCreateObject2();
    
    //Write primary object to file
    GEDCOMerror response = writeGEDCOM(fileName, refObj1);
    if (response.type == WRITE_ERROR){
        sprintf(feedback, "Subtest %d.%d: writeGEDCOM returned WRITE_ERROR while trying to save a valid file to a vaild location (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    //Create test object from file using student code
    GEDCOMobject* testObj;
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test writeGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    //Compare objects: testObj must match one of the two reference objects
    if ( _tObjEqual(refObj1, testObj) || _tObjEqual(refObj2, testObj) ){
        sprintf(feedback, "Subtest %d.%d: %s.",testNum, subTest, messStr);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to %s - test and ref. objects do not match.",testNum, subTest, messStr);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tWriteSubTestWithFile(int testNum, int subTest, char* refFileName, char* messStr){
    char feedback[300];
    SubTestRec result;
    char* fileName = "testFiles/output/someFile.ged";
    GEDCOMobject* refObj = NULL;
    GEDCOMerror err = createGEDCOM(refFileName, &refObj);
    
    //Create reference object from file using student code
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test writeGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, refFileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    //Save GEDCOM to file
    GEDCOMerror response = writeGEDCOM(fileName, refObj);
    if (response.type == WRITE_ERROR){
        sprintf(feedback, "Subtest %d.%d: writeGEDCOM returned WRITE_ERROR while trying to save a valid file to a vaild location (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    //Create test object from file using student code
    GEDCOMobject* testObj;
    err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test writeGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    //Compare objects
    if (_tObjEqual(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: %s.",testNum, subTest, messStr);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: test and reference objects do not match %s.",testNum, subTest, messStr);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tWriteSubTest1(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tMinValid1, "save ref. object (header and submitter, no address) to file");
}

static SubTestRec _tWriteSubTest2(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tMinValid2, "save ref. object (header and submitter with address) to file");
}

static SubTestRec _tWriteSubTest3(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tOneIndivNoFam1, "save ref. object (1 individual, 0 families) to file");
}

static SubTestRec _tWriteSubTest4(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tIndividualsNoFam, "save ref. object (3 individuals, no family) to file");
}

static SubTestRec _tWriteSubTest5(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tOneFamNoIndiv, "save ref. object (0 individuals, 1 family) to file");
}

static SubTestRec _tWriteSubTest6(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tSimpleValid, "save ref. object (3 individuals, 1 family) to file");
}

static SubTestRec _tWriteSubTest7(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tNuclearFamily, "save ref. object (5 individuals, 1 family, contains individual and family events) to file");
}

static SubTestRec _tWriteSubTest8(int testNum, int subTest){
    return _tWriteSubTest2Obj(testNum, subTest,&_tSimpleValidWSurnGiven, &_tSimpleValid, "save ref. object (3 individuals, 1 family, contains SURN/GIVN tags) to file");
}

static SubTestRec _tWriteSubTest9(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tSimpleValid2Gen, "save ref. object (8 individuals, 2 families) to file");
}

static SubTestRec _tWriteSubTest10(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tSimpleValid2GenE1, "save ref. object (7 individuals, 2 families, 2nd family has one spouse) to file");
}

static SubTestRec _tWriteSubTest11(int testNum, int subTest){
    return _tWriteSubTest(testNum, subTest,&_tSimpleValid2GenE2, "save ref. object (4 individuals, 2 families, 2nd family has no children) to file");
}

//
//testRec* _tWriteTest(int testNum){
//    const int numSubs = 11;
//    int subTest = 1;
//    char feedback[300];
//    
//    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
//    testRec * rec = initRec(testNum, numSubs, feedback);
//    
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest1);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest2);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest3);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest4);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest5);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest6);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest7);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest8);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest9);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest10);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tWriteSubTest11);
//
//    return rec;
//}

testRec* _tWriteTest1(int testNum){
    const int numSubs = 2;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tWriteSubTest2);

    return rec;
}

testRec* _tWriteTest2(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest3);
    
    return rec;
}

testRec* _tWriteTest3(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest4);
    
    return rec;
}

testRec* _tWriteTest4(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest5);
    
    return rec;
}

testRec* _tWriteTest5(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest6);
    
    return rec;
}

testRec* _tWriteTest6(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest7);
    
    return rec;
}

testRec* _tWriteTest7(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest8);
    
    return rec;
}

testRec* _tWriteTest8(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest9);
    
    return rec;
}

testRec* _tWriteTest9(int testNum){
    const int numSubs = 2;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created in memory with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest10);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tWriteSubTest11);
    
    return rec;
}

static SubTestRec _tWriteSubTest12(int testNum, int subTest){
    return _tWriteSubTestWithFile(testNum, subTest,"testFiles/valid/simpleValid1N.ged", "using ref. object created from testFiles/valid/simpleValid1N.ged");
}

static SubTestRec _tWriteSubTest13(int testNum, int subTest){
    return _tWriteSubTestWithFile(testNum, subTest,"testFiles/valid/nuclearFamily.ged", "using ref. object created from testFiles/valid/nuclearFamily.ged");
}

static SubTestRec _tWriteSubTest14(int testNum, int subTest){
    return _tWriteSubTestWithFile(testNum, subTest,"testFiles/valid/shakespeare.ged", "using ref. object created from testFiles/valid/shakespeare.ged");
}



testRec* _tWriteTestWFile(int testNum){
    const int numSubs = 3;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: testing writeGEDCOM - comparing ref. objects created from valid files with test objects created by writeGEDCOM/createGEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tWriteSubTest12);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tWriteSubTest13);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tWriteSubTest14);
    return rec;
}


//******************************** Validate Tests (valid)  ********************************

 static SubTestRec _tValidateSubTest(int testNum, int subTest, GEDCOMobject*(*_tCreateObject)(void), char* messStr){
     char feedback[300];
     SubTestRec result;
     GEDCOMobject* refObj = _tCreateObject();
     
     ErrorCode response = validateGEDCOM(refObj);
     
     if (response == OK){
         sprintf(feedback, "Subtest %d.%d: Correctly validated a valid GEDCOM object %s.",testNum, subTest, messStr);
         result = createSubResult(SUCCESS, feedback);
         return result;
     }
     else
     {
         sprintf(feedback, "Subtest %d.%d: Failed to correctly validate a valid GEDCOM object %s.",testNum, subTest, messStr);
         result = createSubResult(FAIL, feedback);
         return result;
     }
 }

static SubTestRec _tValidateSubTest1(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tMinValid1, "(header and submitter, no address)");
}

static SubTestRec _tValidateSubTest2(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tMinValid2, "(header and submitter with address)");
}

static SubTestRec _tValidateSubTest3(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tOneIndivNoFam1, "(1 individual, 0 families)");
}

static SubTestRec _tValidateSubTest4(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tIndividualsNoFam, "(3 individuals, no family)");
}

static SubTestRec _tValidateSubTest5(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tOneFamNoIndiv, "(0 individuals, 1 family)");
}

static SubTestRec _tValidateSubTest6(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tSimpleValid, "(3 individuals, 1 family)");
}

static SubTestRec _tValidateSubTest7(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tNuclearFamily, "(5 individuals, 1 family, contains individual and family events)");
}

static SubTestRec _tValidateSubTest8(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tSimpleValidWSurnGiven, "(3 individuals, 1 family, contains SURN/GIVN tags)");
}

static SubTestRec _tValidateSubTest9(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tSimpleValid2Gen, "(8 individuals, 2 families)");
}

static SubTestRec _tValidateSubTest10(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tSimpleValid2GenE1, "(7 individuals, 2 families, 2nd family has one spouse)");
}

static SubTestRec _tValidateSubTest11(int testNum, int subTest){
    return _tValidateSubTest(testNum, subTest,&_tSimpleValid2GenE2, "(4 individuals, 2 families, 2nd family has no children)");
}

testRec* _tValidateTest(int testNum){
    const int numSubs = 11;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: validateGEDCOM (valid objects)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tValidateSubTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest5);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest6);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest7);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest8);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest9);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest10);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTest11);
    return rec;
}

//******************************** Validate Tests (invalid)  ********************************

static SubTestRec _tValidateSubTestInv(int testNum, int subTest, GEDCOMobject* refObj, ErrorCode refResponse, char* messStr){
    char feedback[300];
    SubTestRec result;
    
    ErrorCode response = validateGEDCOM(refObj);
    
    if (response == refResponse){
        sprintf(feedback, "Subtest %d.%d: Correctly validated a GEDCOM object %s.",testNum, subTest, messStr);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to correctly validate a GEDCOM object %s (expected error code %d, received %d).",testNum, subTest, messStr, refResponse, response);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

//static SubTestRec _tValidateSubTestInv2Ans(int testNum, int subTest, GEDCOMobject* refObj, ErrorCode refResponse, ErrorCode altRefResponse, char* messStr){
//    char feedback[300];
//    SubTestRec result;
//    
//    ErrorCode response = validateGEDCOM(refObj);
//    
//    if (response == refResponse && response == altRefResponse){
//        sprintf(feedback, "Subtest %d.%d: Correctly validated a GEDCOM object %s.",testNum, subTest, messStr);
//        result = createSubResult(SUCCESS, feedback);
//        return result;
//    }
//    else
//    {
//        sprintf(feedback, "Subtest %d.%d: Failed to correctly validate a GEDCOM object %s (expected error code %d or %d, received %d).",testNum, subTest, messStr, refResponse, altRefResponse, response);
//        result = createSubResult(FAIL, feedback);
//        return result;
//    }
//}

static SubTestRec _tValidateSubTestInv1(int testNum, int subTest){
    return _tValidateSubTestInv(testNum, subTest, NULL, INV_GEDCOM, "NULL");
}

static SubTestRec _tValidateSubTestInv2(int testNum, int subTest){
    return _tValidateSubTestInv(testNum, subTest, _tInvalidNoHead(), INV_GEDCOM, "with NULL header in GEDCOMobject");
}

static SubTestRec _tValidateSubTestInv3(int testNum, int subTest){
    return _tValidateSubTestInv(testNum, subTest, _tInvalidNoSubm(), INV_GEDCOM, "with NULL submitter in GEDCOMobject");
}

static SubTestRec _tValidateSubTestInv4(int testNum, int subTest){
    return _tValidateSubTestInv(testNum, subTest, _tInvalidEmptySource(), INV_HEADER, "with empty source string in header");
}

static SubTestRec _tValidateSubTestInv5(int testNum, int subTest){
    return _tValidateSubTestInv(testNum, subTest, _tInvalidNullSubmInHead(), INV_HEADER, "with NULL submitter ptr in header");
}

static SubTestRec _tValidateSubTestInv6(int testNum, int subTest){
    return _tValidateSubTestInv(testNum, subTest, _tInvalidEmptySubmitterName(), INV_RECORD, "with empty submitter name in submitter");
}

//static SubTestRec _tValidateSubTestInv7(int testNum, int subTest){
//    return _tValidateSubTestInv(testNum, subTest, _tInvalidNULLPtrInIndFamilies(), INV_RECORD, "with NULL Family ptr. in an individual's families list");
//}
//
//static SubTestRec _tValidateSubTestInv8(int testNum, int subTest){
//    return _tValidateSubTestInv2Ans(testNum, subTest, _tInvalidNULLPtrInIndividuals(), INV_RECORD, INV_GEDCOM, "with NULL Individual ptr. in a GEDCOMobject's individuals list");
//}

testRec* _tValidateTestInv(int testNum){
    const int numSubs = 6;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: validateGEDCOM (invalid objects)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv4);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv5);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv6);
    subTest++;
//    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv7);
//    subTest++;
//    runSubTest(testNum, subTest, rec, &_tValidateSubTestInv8);
    return rec;
}


