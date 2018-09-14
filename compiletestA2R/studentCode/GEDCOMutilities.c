/*******************************************
Mohammadamin Sheikhtaheri           0930853
CIS2750                        Assignment 2
Tuesday Feb 27, 2018      GEDCOMutilities.c
*******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "a1func.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

void dummyFunc() {}

char* dummyPrint()
{
    return NULL;
}

void deleteID(void* toBeDeleted)
{
    if(toBeDeleted == NULL)
        return;
    free(toBeDeleted);
}

int compareIDs(const void* first, const void* second)
{
    if(first == NULL || second == NULL)
        return -1;
    ID* tmp1 = (ID*) first;
    ID* tmp2 = (ID*) second;
    if(strcmp(tmp1->tag, tmp2->tag) == 0)   //Only comparing the TAGS of the IDs
        return 0;
    else
        return -1;
}

int compareIndivs(const void* first, const void* second)
{
    if(first == NULL && second == NULL)
        return 0;
    if(first == NULL || second == NULL)
        return -1;
    Individual* indiv1 = (Individual*) first;
    Individual* indiv2 = (Individual*) second;

    char tmp1[100];
    char tmp2[100];

    sprintf(tmp1, "%s,%s", indiv1->givenName, indiv1->surname);
    sprintf(tmp2, "%s,%s", indiv2->givenName, indiv2->surname);
    bool checkEvents = false;
    if(strcmp(tmp1, tmp2) == 0)
    {
        ListIterator events1Iter = createIterator(indiv1->events);
        Event* event1;
        while((event1 = nextElement(&events1Iter)) != NULL)
        {
            ListIterator events2Iter = createIterator(indiv2->events);
            Event* event2;
            while((event2 = nextElement(&events2Iter)) != NULL)
            {
                if(compareEvents(event1, event2) == 0)
                {
                    checkEvents = true;
                    break;
                }
            }
            if(checkEvents == false)
                return -1;
            else
                checkEvents = false;
        }
        return 0;
    }

    return -1;
}

int compareGenIndivs(const void* first, const void* second)
{
    if(first == NULL && second == NULL)
        return 0;
    if(first == NULL || second == NULL)
        return -1;
    Individual* indiv1 = (Individual*) first;
    Individual* indiv2 = (Individual*) second;

    char tmp1Surname[100];
    char tmp2Surname[100];

    sprintf(tmp1Surname, "%s", indiv1->surname);
    sprintf(tmp2Surname, "%s", indiv2->surname);

    if(strcmp(tmp1Surname, tmp2Surname) != 0)
    {
        return strcmp(tmp1Surname, tmp2Surname);
    }
    else
    {
        char tmp1GivenName[100];
        char tmp2GivenName[100];

        sprintf(tmp1GivenName, "%s", indiv1->givenName);
        sprintf(tmp2GivenName, "%s", indiv2->givenName);

        return strcmp(tmp1GivenName, tmp2GivenName);
    }
}

void toUpper(char** str)
{
    for(int i = 0; i < strlen(*str); i++)
        (*str)[i] = toupper((*str)[i]);
}

void deleteList(void* toBeDeleted)
{
    clearList(toBeDeleted);
    free(toBeDeleted);
}

GEDCOMerror parseHeader(Header* header, char* tokens[500][100], int* headerEndLine, List* IDlist)
{
    GEDCOMerror error;
    error.type = OK;
    error.line = -1;

    bool checkSource = false;
    bool checkGEDC = false;
    bool checkSub = false;
    bool checkChar = false;
    if(strcmp(tokens[0][0], "0") == 0 && strcmp(tokens[0][1], "HEAD") != 0)   //If header does not exist
    {
        error.type = INV_GEDCOM;
        return error;
    }
    if(strcmp(tokens[0][0], "0") != 0 && strcmp(tokens[0][1], "HEAD") == 0)   //If header does not exist
    {
        error.type = INV_HEADER;
        return error;
    }
    for(int i = 1; strcmp(tokens[i][0], "0") != 0; i++)     //For loop checks to see if all necessary header components are present
    {
        (*headerEndLine)++;

        if(strcmp(tokens[i][1], "SOUR") == 0 && strcmp(tokens[i][2], "") != 0)
            checkSource = true;
        else if(strcmp(tokens[i][1], "GEDC") == 0)
        {
            if(strcmp(tokens[i+1][1], "FORM") == 0 && strcmp(tokens[i+1][2], "") != 0)
            {
                if(strcmp(tokens[i+2][1], "VERS") == 0 && strcmp(tokens[i+2][2], "") != 0)
                    checkGEDC = true;
            }
            else if(strcmp(tokens[i+1][1], "VERS") == 0 && strcmp(tokens[i+1][2], "") != 0)
            {
                if(strcmp(tokens[i+2][1], "FORM") == 0 && strcmp(tokens[i+2][2], "") != 0)
                    checkGEDC = true;
            }
        }
        else if(strcmp(tokens[i][1], "SUBM") == 0 && strcmp(tokens[i][2], "") != 0)
            checkSub = true;
        else if(strcmp(tokens[i][1], "CHAR") == 0 && strcmp(tokens[i][2], "") != 0)
            checkChar = true;
    }

    if(checkSource == false || checkGEDC == false || checkSub == false || checkChar == false)
    {
        error.type = INV_HEADER;
        error.line = *headerEndLine;
        return error;
    }
    strcpy(header->source, "");
    for(int i = 1; strcmp(tokens[i][0], "0") != 0; i++)
    {
        if(strcmp(tokens[i][1], "SOUR") == 0)   //Parsing the header SOURCE
        {
            for(int j = 2; strcmp(tokens[i][j], "") != 0; j++)
            {
                sprintf(header->source, "%s%s", header->source, tokens[i][j]);
                if(strcmp(tokens[i][j+1], "") != 0)
                    sprintf(header->source, "%s ", header->source);
            }
        }
        else if(strcmp(tokens[i][1], "GEDC") == 0)  //Parsing the GEDC version
        {
            int x = i + 1;
            if(strcmp(tokens[x][1], "VERS") != 0)
                x++;

            header->gedcVersion = atof(tokens[x][2]);
            i++;
        }
        else if(strcmp(tokens[i][1], "SUBM") == 0)
        {
            ID* newID = malloc(sizeof(ID));     //Creating a new XREF ID, and adding it to the list for later
            strcpy(newID->tag, tokens[i][2]);
            newID->reference = header;
            insertBack(IDlist, newID);
        }
        else if(strcmp(tokens[i][1], "CHAR") == 0)  //Parsing the character formatting
        {
            if(strcmp(tokens[i][2], "ANSEL") == 0)
                header->encoding = ANSEL;
            else if(strcmp(tokens[i][2], "UTF8") == 0 || strcmp(tokens[i][2], "UTF-8") == 0)
                header->encoding = UTF8;
            else if(strcmp(tokens[i][2], "UNICODE") == 0)
                header->encoding = UNICODE;
            else if(strcmp(tokens[i][2], "ASCII") == 0)
                header->encoding = ASCII;
        }
        else    //If anything else, insert the tag and value in to an otherField list
        {
            if(strcmp(tokens[i][2], "") != 0)
            {
                Field* newField = malloc(sizeof(Field));
                newField->tag = malloc(sizeof(char) * strlen(tokens[i][1]) + 1);
                newField->value = malloc(sizeof(char) * 100);
                strcpy(newField->tag, "");
                strcpy(newField->value, "");
                strcpy(newField->tag, tokens[i][1]);
                for(int j = 2; strcmp(tokens[i][j], "") != 0; j++)
                {
                    sprintf(newField->value, "%s%s", newField->value, tokens[i][j]);
                    if(strcmp(tokens[i][j+1], "") != 0)
                        sprintf(newField->value, "%s ", newField->value);
                }
                insertBack(&(header->otherFields), newField);
            }
        }
    }

    return error;
}

GEDCOMerror parseIndividual(List* individuals, char* tokens[500][100], int* recordEndLine, List* IDlist)
{
    GEDCOMerror error;
    error.type = OK;
    error.line = -1;
    Individual* newInd = malloc(sizeof(Individual));    //Mallocing and initializing the new to be parsed individual
    newInd->givenName = malloc(sizeof(char) * 100);
    newInd->surname = malloc(sizeof(char) * 100);
    if(newInd == NULL || newInd->givenName == NULL || newInd->surname == NULL)
    {
        error.type = OTHER_ERROR;
        error.line = -1;
        return error;
    }
    strcpy(newInd->givenName, "");
    strcpy(newInd->surname, "");
    newInd->events = initializeList(printEvent, deleteEvent, compareEvents);    //Initializing all of the required lists of the individual
    newInd->families = initializeList(printFamily, dummyFunc, compareFamilies);
    newInd->otherFields = initializeList(printField, deleteField, compareFields);
    ID* newID = malloc(sizeof(ID));     //Creating a new XREF ID in the list for the individual which will be used later
    strcpy(newID->tag, tokens[*recordEndLine][1]);
    newID->reference = newInd;
    insertBack(IDlist, newID);
    (*recordEndLine)++;
    bool checkName = false;
    for(int i = (*recordEndLine); strcmp(tokens[i][0], "0") != 0; i++)
    {
        if(strcmp(tokens[i][1], "NAME") == 0)   //Parsing the individual's name (first and last)
        {
            checkName = true;
            for(int k = 2; strcmp(tokens[i][k], "") != 0; k++)
            {
                if(strstr(tokens[i][k], "/") == NULL)
                {
                    if(strcmp(newInd->givenName, "") == 0)
                        strcpy(newInd->givenName, tokens[i][k]);
                    else
                    {
                        sprintf(newInd->givenName, "%s %s", newInd->givenName, tokens[i][k]);
                    }
                }
                else
                {
                    int z = 0;
                    for(int j = 0; j < strlen(tokens[i][k]); j++)
                    {
                        if(tokens[i][k][j] != '/')
                            newInd->surname[z++] = tokens[i][k][j];
                    }
                }
            }
        }
        else if(strcmp(tokens[i][1], "BIRT") == 0 || strcmp(tokens[i][1], "CHR") == 0 || strcmp(tokens[i][1], "DEAT") == 0 || strcmp(tokens[i][1], "BURI") == 0 || strcmp(tokens[i][1], "CREM") == 0 || strcmp(tokens[i][1], "ADOP") == 0 || strcmp(tokens[i][1], "BAPM") == 0 || strcmp(tokens[i][1], "BARM") == 0
             || strcmp(tokens[i][1], "BASM") == 0 || strcmp(tokens[i][1], "BLES") == 0 || strcmp(tokens[i][1], "CHRA") == 0 || strcmp(tokens[i][1], "CONF") == 0 || strcmp(tokens[i][1], "FCOM") == 0 || strcmp(tokens[i][1], "ORDN") == 0 || strcmp(tokens[i][1], "NATU") == 0 || strcmp(tokens[i][1], "EMIG") == 0
             || strcmp(tokens[i][1], "IMMI") == 0 || strcmp(tokens[i][1], "CENS") == 0 || strcmp(tokens[i][1], "PROB") == 0 || strcmp(tokens[i][1], "WILL") == 0 || strcmp(tokens[i][1], "GRAD") == 0 || strcmp(tokens[i][1], "RETI") == 0 || strcmp(tokens[i][1], "EVEN") == 0)
        {
            //This if statement is to parse the individual's events
            Event* newEvent = malloc(sizeof(Event));
            newEvent->otherFields = initializeList(printField, deleteField, compareFields);
            strcpy(newEvent->type, "");
            strcpy(newEvent->type, tokens[i][1]);
            newEvent->date = malloc(sizeof(char) * 100);
            newEvent->place = malloc(sizeof(char) * 100);
            strcpy(newEvent->date, "");
            strcpy(newEvent->place, "");
            (*recordEndLine)++;
            for(int j = *recordEndLine; strcmp(tokens[j][0], "2") == 0; j++)
            {
                if(strcmp(tokens[j][1], "DATE") == 0)   //Parsing the event's date (if present)
                {
                    strcpy(newEvent->date, tokens[j][2]);
                    for(int k = 3; strcmp(tokens[j][k], "") != 0; k++)
                    {
                        sprintf(newEvent->date, "%s %s", newEvent->date, tokens[j][k]);
                    }
                }
                else if(strcmp(tokens[j][1], "PLAC") == 0)  //Parsing the event's place (if present)
                {
                    strcpy(newEvent->place, tokens[j][2]);
                    for(int k = 3; strcmp(tokens[j][k], "") != 0; k++)
                    {
                        sprintf(newEvent->place, "%s %s", newEvent->place, tokens[j][k]);
                    }
                }
                else    //If anything else, insert it into the otherFields list for the event
                {
                    Field* newField = malloc(sizeof(Field));
                    newField->tag = malloc(sizeof(char) * 100);
                    newField->value = malloc(sizeof(char) * 100);
                    strcpy(newField->tag, "");
                    strcpy(newField->value, "");
                    strcpy(newField->tag, tokens[j][1]);
                    for(int k = 2; strcmp(tokens[j][k], "") != 0; k++)
                    {
                        sprintf(newField->value, "%s%s", newField->value, tokens[j][k]);
                        if(strcmp(tokens[j][k+1], "") != 0)
                            sprintf(newField->value, "%s ", newField->value);
                    }
                    insertBack(&(newEvent->otherFields), newField);
                }
                (*recordEndLine) = j;
                i = j;
            }
            insertBack(&(newInd->events), newEvent);
        }
        else if(strcmp(tokens[i][1], "FAMS") != 0 && strcmp(tokens[i][1], "FAMC") != 0)
        {
            Field* newField = malloc(sizeof(Field));
            newField->tag = malloc(sizeof(char) * 100);
            newField->value = malloc(sizeof(char) * 100);
            strcpy(newField->tag, "");
            strcpy(newField->value, "");
            strcpy(newField->tag, tokens[i][1]);
            for(int k = 2; strcmp(tokens[i][k], "") != 0; k++)
            {
                sprintf(newField->value, "%s%s", newField->value, tokens[i][k]);
                if(strcmp(tokens[i][k+1], "") != 0)
                    sprintf(newField->value, "%s ", newField->value);
            }
            insertBack(&(newInd->otherFields), newField);
        }

        (*recordEndLine)++;
    }
    (*recordEndLine)--;
    if(checkName == false)
    {
        error.type = INV_RECORD;
        error.line = (*recordEndLine) + 1;
        return error;
    }
    insertBack(individuals, newInd);

    return error;
}

GEDCOMerror parseFamily(List* families, char* tokens[500][100], int* recordEndLine, List* IDlist)
{
    GEDCOMerror error;
    error.type = OK;
    error.line = -1;
    Family* newFam = malloc(sizeof(Family));    //Mallocing and initializing a new family to be parsed
    newFam->wife = NULL;
    newFam->husband = NULL;
    newFam->children = initializeList(printIndividual, dummyFunc, compareIndivs);
    newFam->otherFields = initializeList(printField, deleteField, compareFields);
    newFam->events = initializeList(printEvent, deleteEvent, compareEvents);
    (*recordEndLine)++;
    for(int i = (*recordEndLine); strcmp(tokens[i][0], "0") != 0; i++)
    {
        if(strcmp(tokens[i][1], "HUSB") == 0)   //If there is a husband, parse him
        {
            if(strcmp(tokens[i][2], "") == 0)
            {
                error.type = INV_RECORD;
                error.line = (*recordEndLine) + 1;
                return error;
            }
            List tmp = *IDlist;
            while(tmp.head != NULL)
            {
                ID* id = (ID*)tmp.head->data;
                Individual* ref = (Individual*)id->reference;

                if(strcmp(tokens[i][2], id->tag) == 0)  //If the XREF matches one of the individuals, add the husband to the family and add the family to the individual (double link)
                {
                    newFam->husband = ref;
                    insertBack(&(ref->families), newFam);
                    break;
                }

                tmp.head = tmp.head->next;
            }
        }
        else if(strcmp(tokens[i][1], "WIFE") == 0)  //If there is a wife, parse her
        {
            if(strcmp(tokens[i][2], "") == 0)
            {
                error.type = INV_RECORD;
                error.line = (*recordEndLine) + 1;
                return error;
            }
            List tmp = *IDlist;
            while(tmp.head != NULL)
            {
                ID* id = (ID*)tmp.head->data;
                Individual* ref = (Individual*)id->reference;

                if(strcmp(tokens[i][2], id->tag) == 0) //If the XREF matches one of the individuals, add the wife to the family and add the family to the individual (double link)
                {
                    newFam->wife = ref;
                    insertBack(&(ref->families), newFam);
                    break;
                }

                tmp.head = tmp.head->next;
            }
        }
        else if(strcmp(tokens[i][1], "CHIL") == 0)  //If there are children, parse them
        {
            if(strcmp(tokens[i][2], "") == 0)
            {
                error.type = INV_RECORD;
                error.line = (*recordEndLine) + 1;
                return error;
            }
            List tmp = *IDlist;
            while(tmp.head != NULL)
            {
                ID* id = (ID*)tmp.head->data;
                Individual* ref = (Individual*)id->reference;

                if(strcmp(tokens[i][2], id->tag) == 0) //If the XREF matches one of the individuals, add the child to the family and add the family to the individual (double link)
                {
                    insertBack(&(newFam->children), ref);
                    insertBack(&(ref->families), newFam);
                    break;
                }

                tmp.head = tmp.head->next;
            }
        }
        else if(strcmp(tokens[i][1], "ANUL") == 0 || strcmp(tokens[i][1], "CENS") == 0 || strcmp(tokens[i][1], "DIV") == 0 || strcmp(tokens[i][1], "DIVF") == 0 || strcmp(tokens[i][1], "ENGA") == 0 || strcmp(tokens[i][1], "MARB") == 0 || strcmp(tokens[i][1], "MARC") == 0 || strcmp(tokens[i][1], "MARR") == 0
             || strcmp(tokens[i][1], "MARL") == 0 || strcmp(tokens[i][1], "MARS") == 0)
        {
            //This if statement is to parse the individual's events
            Event* newEvent = malloc(sizeof(Event));
            newEvent->otherFields = initializeList(printField, deleteField, compareFields);
            strcpy(newEvent->type, "");
            strcpy(newEvent->type, tokens[i][1]);
            newEvent->date = malloc(sizeof(char) * 100);
            newEvent->place = malloc(sizeof(char) * 100);
            strcpy(newEvent->date, "");
            strcpy(newEvent->place, "");
            if(strcmp(tokens[*recordEndLine + 1][0], "2") == 0)
                (*recordEndLine)++;
            for(int j = *recordEndLine; strcmp(tokens[j][0], "2") == 0; j++)
            {
                if(strcmp(tokens[j][1], "DATE") == 0)   //Parsing the event's date (if present)
                {
                    strcpy(newEvent->date, tokens[j][2]);
                    for(int k = 3; strcmp(tokens[j][k], "") != 0; k++)
                    {
                        sprintf(newEvent->date, "%s %s", newEvent->date, tokens[j][k]);
                    }
                }
                else if(strcmp(tokens[j][1], "PLAC") == 0)  //Parsing the event's place (if present)
                {
                    strcpy(newEvent->place, tokens[j][2]);
                    for(int k = 3; strcmp(tokens[j][k], "") != 0; k++)
                    {
                        sprintf(newEvent->place, "%s %s", newEvent->place, tokens[j][k]);
                    }
                }
                else    //If anything else, insert it into the otherFields list for the event
                {
                    Field* newField = malloc(sizeof(Field));
                    newField->tag = malloc(sizeof(char) * 100);
                    newField->value = malloc(sizeof(char) * 100);
                    strcpy(newField->tag, "");
                    strcpy(newField->value, "");
                    strcpy(newField->tag, tokens[j][1]);
                    for(int k = 2; strcmp(tokens[j][k], "") != 0; k++)
                    {
                        sprintf(newField->value, "%s%s", newField->value, tokens[j][k]);
                        if(strcmp(tokens[j][k+1], "") != 0)
                            sprintf(newField->value, "%s ", newField->value);
                    }
                    insertBack(&(newEvent->otherFields), newField);
                }
                (*recordEndLine) = j;
                i = j;
            }
            insertBack(&(newFam->events), newEvent);
        }
        else    //If none of the above, add to otherFields of the family record
        {
            Field* newField = malloc(sizeof(Field));
            newField->tag = malloc(sizeof(char) * 100);
            newField->value = malloc(sizeof(char) * 100);
            strcpy(newField->tag, "");
            strcpy(newField->value, "");
            strcpy(newField->tag, tokens[i][1]);
            for(int k = 2; strcmp(tokens[i][k], "") != 0; k++)
            {
                sprintf(newField->value, "%s%s", newField->value, tokens[i][k]);
                if(strcmp(tokens[i][k+1], "") != 0)
                    sprintf(newField->value, "%s ", newField->value);
            }
            insertBack(&(newFam->otherFields), newField);
        }
        (*recordEndLine)++;
    }
    (*recordEndLine)--;
    insertBack(families, newFam);

    return error;
}

GEDCOMerror parseSubmitter(Submitter* submitter, char* tokens[500][100], int* recordEndLine, List* IDlist, bool* checkSubmitter)
{
    GEDCOMerror error;
    error.type = OK;
    error.line = -1;
    strcpy(submitter->address, "");
    strcpy(submitter->submitterName, "");
    List tmp = *IDlist;
    while(tmp.head != NULL)
    {
        ID* id = (ID*)tmp.head->data;

        if(strcmp(tokens[*recordEndLine][1], id->tag) == 0) //If correct submitter is found, parse it
        {
            *checkSubmitter = true;
            Header* ref = (Header*)id->reference;
            ref->submitter = submitter;
            break;
        }

        tmp.head = tmp.head->next;
    }
    (*recordEndLine)++;
    if(*checkSubmitter == false)    //If this is the wrong submitter, ignore it and return
        return error;
    for(int i = (*recordEndLine); strcmp(tokens[i][0], "0") != 0; i++)
    {
        if(strcmp(tokens[i][1], "NAME") == 0)   //Parse name of the submitter
        {
            for(int k = 2; strcmp(tokens[i][k], "") != 0; k++)
            {
                if(strcmp(submitter->submitterName, "") == 0)
                    strcpy(submitter->submitterName, tokens[i][k]);
                else
                {
                    sprintf(submitter->submitterName, "%s %s", submitter->submitterName, tokens[i][k]);
                }
            }
        }
        else if(strcmp(tokens[i][1], "ADDR") == 0)  //Parse address of submitter
        {
            for(int k = 2; strcmp(tokens[i][k], "") != 0; k++)
            {
                if(strcmp(submitter->address, "") == 0)
                    strcpy(submitter->address, tokens[i][k]);
                else
                {
                    sprintf(submitter->address, "%s %s", submitter->address, tokens[i][k]);
                }
            }
        }
        else    //If none of the above, add field to otherFields
        {
            Field* newField = malloc(sizeof(Field));
            newField->tag = malloc(sizeof(char) * 100);
            newField->value = malloc(sizeof(char) * 100);
            strcpy(newField->tag, "");
            strcpy(newField->value, "");
            strcpy(newField->tag, tokens[i][1]);
            for(int k = 2; strcmp(tokens[i][k], "") != 0; k++)
            {
                sprintf(newField->value, "%s%s", newField->value, tokens[i][k]);
                if(strcmp(tokens[i][k+1], "") != 0)
                    sprintf(newField->value, "%s ", newField->value);
            }
            insertBack(&(submitter->otherFields), newField);
        }
        (*recordEndLine)++;
    }
    if(strcmp(submitter->submitterName, "") == 0)
    {
        error.type = INV_RECORD;
        error.line = (*recordEndLine);
    }
    (*recordEndLine)--;
    return error;
}

void getDescendantsRecur(const GEDCOMobject* familyRecord, List people, int genNum, List* generations)  //Function is called by getDescendantListN and calls itself recursively in order to add all of the generations into the inputted generations list
{
    ListIterator parentIter = createIterator(people);
    Individual* person;
    List* deeper = malloc(sizeof(List));    //This list contains every child from the current generation which will later be inserted into the generations list
    *deeper = initializeList(dummyPrint, dummyFunc, compareGenIndivs);
    while((person = nextElement(&parentIter)) != NULL)
    {
        ListIterator familyIter = createIterator(familyRecord->families);
        Family* fam;
        while((fam = nextElement(&familyIter)) != NULL)
        {
            Individual* husband = fam->husband;
            Individual* wife = fam->wife;
            if(person == husband || person == wife)   //Searching every family for the person as one of the parents
            {
                if(genNum != 0)
                {
                    bool checkDuplicate = false;
                    ListIterator nextGenIter = createIterator(fam->children);
                    Individual* nextChild;
                    while((nextChild = nextElement(&nextGenIter)) != NULL)
                    {
                        ListIterator genListIter = createIterator(*generations);
                        List* generationList;
                        while((generationList = nextElement(&genListIter)) != NULL)     //While loop checks for duplicates in the list
                        {
                            ListIterator insideListIter = createIterator(*generationList);
                            Individual* listPerson;
                            while((listPerson = nextElement(&insideListIter)) != NULL)
                            {
                                if(nextChild == listPerson)
                                    checkDuplicate = true;
                            }
                        }
                        if(checkDuplicate == false)
                            insertSorted(deeper, nextChild);    //Adding the child to the current generation
                    }
                    checkDuplicate = false;
                    while((fam = nextElement(&familyIter)) != NULL)     //Making sure to look for other families that this individual could be a parent of (more than one FAMS tag)
                    {
                        husband = fam->husband;
                        wife = fam->wife;
                        if(person == husband || person == wife)   //Searching every family for the person as one of the parents
                        {
                            if(genNum != 0)
                            {
                                ListIterator anotherGenIter = createIterator(fam->children);
                                while((nextChild = nextElement(&anotherGenIter)) != NULL)
                                {
                                    ListIterator genListIter = createIterator(*generations);
                                    List* generationList;
                                    while((generationList = nextElement(&genListIter)) != NULL)     //While loop checks for duplicates in the list
                                    {
                                        ListIterator insideListIter = createIterator(*generationList);
                                        Individual* listPerson;
                                        while((listPerson = nextElement(&insideListIter)) != NULL)
                                        {
                                            if(nextChild == listPerson)
                                                checkDuplicate = true;
                                        }
                                    }
                                    if(checkDuplicate == false)
                                        insertSorted(deeper, nextChild);    //Adding the child to the current generation
                                }
                            }
                            else
                                return;
                        }
                    }
                }
                else
                    return;
            }
        }
    }
    if(deeper->head != NULL && genNum != 0)
    {
        insertBack(generations, deeper);    //Inserting the current generation to the generations list
        getDescendantsRecur(familyRecord, *deeper, genNum - 1, generations);    //Recursively calling the function using the next generation as the parents
    }
    else
    {
        free(deeper);
    }
}

void getAncestorsRecur(const GEDCOMobject* familyRecord, List people, int genNum, List* generations)    //Function is called by getAncestorListN and calls itself recursively in order to add all of the generations into the inputted generations list
{
    ListIterator childIter = createIterator(people);
    Individual* person;
    List* deeper = malloc(sizeof(List));    //This is the list of the current generation's parents
    *deeper = initializeList(dummyPrint, dummyFunc, compareGenIndivs);
    while((person = nextElement(&childIter)) != NULL)
    {
        ListIterator familyIter = createIterator(familyRecord->families);
        Family* fam;
        while((fam = nextElement(&familyIter)) != NULL)
        {
            bool checkParent = false;   //Variable states whether or not the individual is in the current family
            List children = fam->children;
            ListIterator childrenIter = createIterator(children);
            Individual* kid;
            while((kid = nextElement(&childrenIter)) != NULL)
            {
                if(kid == person)   //Checking if the person is the child of the current family
                {
                    checkParent = true;
                    break;
                }
            }
            if(checkParent == true) //If the parents are ancestors, add them to the list
            {
                Individual* husband = fam->husband;
                Individual* wife = fam->wife;
                bool checkHusbandDup = false;
                bool checkWifeDup = false;
                ListIterator listIter = createIterator(*deeper);
                Individual* ancestor;
                while((ancestor = nextElement(&listIter)) != NULL)
                {
                    if(compareIndivs(fam->husband, ancestor) == 0)
                    {
                        checkHusbandDup = true;
                    }
                    if(compareIndivs(fam->wife, ancestor) == 0)
                    {
                        checkWifeDup = true;
                    }
                }
                if(checkHusbandDup == false)
                    insertSorted(deeper, husband);
                if(checkWifeDup == false)
                    insertSorted(deeper, wife);
            }
        }
    }
    if(deeper->head != NULL && genNum != 0)
    {
        insertBack(generations, deeper);    //Adding the current generations of parents to the generations list
        getAncestorsRecur(familyRecord, *deeper, genNum - 1, generations);  //Recursively calling the function with the parents as the children
    }
    else
    {
        free(deeper);
    }
}