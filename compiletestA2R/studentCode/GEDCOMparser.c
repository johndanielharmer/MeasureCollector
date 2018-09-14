/*******************************************
Mohammadamin Sheikhtaheri           0930853
CIS2750                        Assignment 2
Tuesday Feb 27, 2018         GEDCOMparser.c
*******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "a1func.h"   //My own helper function header include
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj)
{
    GEDCOMerror error;
    if(fileName == NULL || strcmp(fileName, "") == 0 || fileName[strlen(fileName) - 1] != 'd' || fileName[strlen(fileName) - 2] != 'e' || fileName[strlen(fileName) - 3] != 'g' || fileName[strlen(fileName) - 4] != '.')
    {
        error.type = INV_FILE;
        error.line = -1;
        *obj = NULL;
        return error;
    }
    FILE* file = fopen(fileName, "r");  //Attempting to open the file and verifying that it is a correct file
    if(file == NULL)
    {
        error.type = INV_FILE;
        error.line = -1;
        *obj = NULL;
        return error;
    }
    fseek(file, 0, SEEK_END); //Checking to see if the file is empty
    if(ftell(file) == 0)
    {
        fclose(file);
        error.type = INV_FILE;
        error.line = -1;
        *obj = NULL;
        return error;
    }
    fclose(file);
    file = fopen(fileName, "r"); //Re-opening file due to the seeking above (to start at the top again)

    char line[500];
    char* tokens[600][100];  //The tokens array that will be used to parse the entire GEDCOM file in to character arrays
    for(int i = 0; i < 600; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            tokens[i][j] = malloc(sizeof(char) * 100);  //Mallocing and initializing the tokens array
            strcpy(tokens[i][j], "");
        }
    }
    int lineIndex = 0;  //Variable for keeping track of the current line
    int prevTokenIndex = 0;   //Variable for keeping track of the previous token index
    int numLines = 0;   //Variable for keeping track of the number of lines in the file
    bool checkTRLR = false;     //Checking if file terminator is there
    while(fgets(line, 500, file) != NULL)   //Main loop that parses the file line by line
    {
        if(strlen(line) > 255)
        {
            fclose(file);
            for(int i = 0; i < 600; i++)
            {
                for(int j = 0; j < 100; j++)
                {
                    free(tokens[i][j]);
                }
            }
            error.type = INV_RECORD;
            error.line = numLines + 1;
            *obj = NULL;
            return error;
        }
        if(line[0] == '\n' || line[0] == '\r')
            continue;
        int tokenIndex = 0;
        char* buffer = NULL;
        buffer = strtok(line, " ");
        strcpy(tokens[lineIndex][tokenIndex], buffer);  //Acquiring the first token and putting it in to the array
        tokenIndex++;
        numLines++;
        
        while((buffer = strtok(NULL, " ")) != NULL)   //Acquiring every other token on the same line and putting it in the array 
        {
            for(int i = 0; i < strlen(buffer); i++)
            {
                if(buffer[i] == '\n' || buffer[i] == '\r')
                {
                    buffer[i] = '\0';
                    break;
                }
            }
            /*if(buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r')    //Removing the \n or \r at the end of each line
            {
                if(buffer[strlen(buffer) - 2] == '\n' || buffer[strlen(buffer) - 2] == '\r')
                    buffer[strlen(buffer) - 2] = '\0';
                else
                    buffer[strlen(buffer) - 1] = '\0';
            }*/
            if(strcmp(buffer, "TRLR") == 0)
            {
                checkTRLR = true;
            }
            if(tokenIndex == 1)
                toUpper(&buffer);   //Making every tag uppercase
            if(strcmp(buffer, "CONT") == 0 || strcmp(buffer, "CONC") == 0)
            {
                tokenIndex--;
                if(strcmp(buffer, "CONT") == 0)
                    tokens[lineIndex - 1][prevTokenIndex][strlen(tokens[lineIndex - 1][prevTokenIndex])] = '\n';    //Keep newline if there is a 'CONT'
                while((buffer = strtok(NULL, " ")) != NULL)
                {
                    for(int i = 0; i < strlen(buffer); i++)
                    {
                        if(buffer[i] == '\n' || buffer[i] == '\r')
                        {
                            buffer[i] = '\0';
                            break;
                        }
                    }
                    int checkIndex = 1;
                    while(strcmp(tokens[lineIndex - checkIndex][0], "C") == 0)
                        checkIndex++;
                    sprintf(tokens[lineIndex - checkIndex][prevTokenIndex], "%s%s", tokens[lineIndex - checkIndex][prevTokenIndex], buffer);    //Moving the tokens from the CONT/CONC tag to the previous token on the line above
                    prevTokenIndex++;
                }
                strcpy(tokens[lineIndex][0], "C");   //Replacing the line with the CONT/CONC tag with a 'C'
                tokenIndex = prevTokenIndex;
            }
            else
            {
                strcpy(tokens[lineIndex][tokenIndex], buffer);      //If theres no CONT/CONC, place the line in the token normally
                tokenIndex++;
            }
        }
        lineIndex++;
        prevTokenIndex = tokenIndex - 1;
    }
    if(checkTRLR == false)  //Freeing everything and setting the object to NULL if the file terminator is not present
    {
        fclose(file);
        for(int i = 0; i < 600; i++)
        {
            for(int j = 0; j < 100; j++)
            {
                free(tokens[i][j]);
            }
        }
        *obj = NULL;
        error.type = INV_GEDCOM;
        error.line = -1;
        return error;
    }
    for(int i = 0; i < 499; i++)
    {
        if(strcmp(tokens[i][0], "C") == 0 || strcmp(tokens[i+1][0], "C") == 0)
            continue;
        int difference = atoi(tokens[i+1][0]) - atoi(tokens[i][0]);
        if(strcmp(tokens[i+1][0], "0") != 0 && (difference > 1 || difference < -1))
        {
            fclose(file);
            for(int i = 0; i < 600; i++)
            {
                for(int j = 0; j < 100; j++)
                {
                    free(tokens[i][j]);
                }
            }
            *obj = NULL;
            error.type = INV_RECORD;
            error.line = i+2;
            return error;
        }
    }
    //Mallocing all of the required records for the GEDCOM object
    *obj = malloc(sizeof(GEDCOMobject));
    (*obj)->header = malloc(sizeof(Header));
    (*obj)->header->otherFields = initializeList(printField, deleteField, compareFields);
    (*obj)->submitter = malloc(sizeof(Submitter) + 50 * sizeof(char));
    (*obj)->submitter->otherFields = initializeList(printField, deleteField, compareFields);
    (*obj)->individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    (*obj)->families = initializeList(printFamily, deleteFamily, compareFamilies);
    List IDlist = initializeList(dummyPrint, deleteID, compareIDs);     //List keeps track of all of the XREF pointers
    bool checkSubmitter = false;
    int recordEndLine = 1;  //Variable keeps track of the end of the previously parsed record
    error = parseHeader((*obj)->header, tokens, &recordEndLine, &IDlist);   //Function will parse the header in to the header record
    if(error.type != OK)    //If parseHeader failed, free everything and return the error
    {
        fclose(file);
        clearList(&IDlist);
        for(int i = 0; i < 600; i++)
            for(int j = 0; j < 100; j++)
                free(tokens[i][j]);
        deleteGEDCOM(*obj);
        *obj = NULL;
        return error;
    }

    for(int i = recordEndLine; i < numLines; i++)
    {
        if(strcmp(tokens[i][0], "0") == 0)
        {
            if(strcmp(tokens[i][2], "INDI") == 0)   //If an individual record comes up, parse them in to the list
            {
                error = parseIndividual(&(*obj)->individuals, tokens, &recordEndLine, &IDlist);
                if(error.type != OK)
                {
                    fclose(file);
                    clearList(&IDlist);
                    for(int i = 0; i < 600; i++)
                    {
                        for(int j = 0; j < 100; j++)
                        {
                            free(tokens[i][j]);
                        }
                    }
                    deleteGEDCOM(*obj);
                    *obj = NULL;
                    return error;
                }
                i = recordEndLine;
            }
            else if(strcmp(tokens[i][2], "FAM") == 0)   //If a family record comes up, parse it in to the list
            {
                error = parseFamily(&(*obj)->families, tokens, &recordEndLine, &IDlist);
                if(error.type != OK)
                {
                    fclose(file);
                    clearList(&IDlist);
                    for(int i = 0; i < 600; i++)
                    {
                        for(int j = 0; j < 100; j++)
                        {
                            free(tokens[i][j]);
                        }
                    }
                    deleteGEDCOM(*obj);
                    *obj = NULL;
                    return error;
                }
                i = recordEndLine;
            }
            else if(strcmp(tokens[i][2], "SUBM") == 0)  //If a submitter record comes up, parse it in to the list
            {
                checkSubmitter = true;
                error = parseSubmitter((*obj)->submitter, tokens, &recordEndLine, &IDlist, &checkSubmitter);
                if(error.type != OK)
                {
                    fclose(file);
                    clearList(&IDlist);
                    for(int i = 0; i < 600; i++)
                    {
                        for(int j = 0; j < 100; j++)
                        {
                            free(tokens[i][j]);
                        }
                    }
                    deleteGEDCOM(*obj);
                    *obj = NULL;
                    return error;
                }
                i = recordEndLine;
            }
        }
        recordEndLine++;
    }
    if(checkSubmitter == false)     //Checking if submitter is present
    {
        fclose(file);
        clearList(&IDlist);
        for(int i = 0; i < 600; i++)
            for(int j = 0; j < 100; j++)
                free(tokens[i][j]);
        deleteGEDCOM(*obj);
        *obj = NULL;
        error.type = INV_GEDCOM;
        error.line = -1;
        return error;
    }
    //Closing the file and freeing non-essential variables at the end of createGEDCOM
    fclose(file);
    clearList(&IDlist);
    for(int i = 0; i < 600; i++)
        for(int j = 0; j < 100; j++)
            free(tokens[i][j]);

    return error;
}

char* printGEDCOM(const GEDCOMobject* obj)
{
    char* gedStr = malloc(sizeof(char) * 10000);    //Mallocing a large amount of memory in case the file is large
    strcpy(gedStr, "");
    if(obj == NULL)
        return gedStr;
    Header* header = obj->header;
    Submitter* submitter = obj->submitter;
    List individuals = obj->individuals;
    List families = obj->families;
    sprintf(gedStr, "\nHEADER\n*******\n");
    sprintf(gedStr, "%sHeader Source: %s\nGEDC Version: %f\nEncoding: ", gedStr, header->source, header->gedcVersion);
    if(header->encoding == ANSEL)
        sprintf(gedStr, "%sANSEL\n", gedStr);
    else if(header->encoding == UTF8)
        sprintf(gedStr, "%sUTF8\n", gedStr);
    else if(header->encoding == UNICODE)
        sprintf(gedStr, "%sUNICODE\n", gedStr);
    else if(header->encoding == ASCII)
        sprintf(gedStr, "%sASCII\n", gedStr);
    if(individuals.head != NULL)
        sprintf(gedStr, "%s\nINDIVIDUALS\n***********\n", gedStr);
    while(individuals.head != NULL)     //Adding every individual, and their events to the string that is to be printed
    {
        Individual* indiv = (Individual*)individuals.head->data;
        char* indivStr = printIndividual(indiv);
        sprintf(gedStr, "%s%s", gedStr, indivStr);
        free(indivStr);
        individuals.head = individuals.head->next;
    }
    if(families.head != NULL)     //Adding every family (including children) to the string that is to be printed
        sprintf(gedStr, "%sFAMILIES\n********\n", gedStr);
    while(families.head != NULL)
    {
        Family* fam = (Family*)families.head->data;
        char* famStr = printFamily(fam);
        sprintf(gedStr, "%s%s", gedStr, famStr);
        free(famStr);
        families.head = families.head->next;
    }
    sprintf(gedStr, "%sSUBMITTER\n*********\n", gedStr);
    sprintf(gedStr, "%sSubmitter Name: %s\nSubmitter Address: %s\n", gedStr, submitter->submitterName, submitter->address);
    return gedStr;
}

void deleteGEDCOM(GEDCOMobject* obj)
{
    if(obj == NULL)
    {
        return;
    }
    //Freeing every struct/list that needs to be freed without double freeing
    if(obj->individuals.head != NULL)
        clearList(&obj->individuals);
    if(obj->header->otherFields.head != NULL)
        clearList(&obj->header->otherFields);
    if(obj->families.head != NULL)
        clearList(&(obj->families));
    if(obj->submitter->otherFields.head != NULL)
        clearList(&(obj->submitter->otherFields));
    if(obj->submitter != NULL)
        free(obj->submitter);
    if(obj->header != NULL)
        free(obj->header);
    obj->submitter = NULL;
    obj->header = NULL;
    free(obj);
    obj = NULL;
}

char* printField(void* toBePrinted)
{
    char* fieldStr = malloc(sizeof(char) * 100);
    if(toBePrinted == NULL)
        return fieldStr;
    Field* field = (Field*) toBePrinted;
    sprintf(fieldStr, "%s, %s", field->tag, field->value);  //Adding tag and value to the string that needs to be printed
    return fieldStr;
}

void deleteField(void* toBeDeleted)
{
    if(toBeDeleted == NULL)
        return;
    free(((Field*)(toBeDeleted))->tag);     //Freeing tag and value before deleting the field itself
    free(((Field*)(toBeDeleted))->value);
    free((Field*)toBeDeleted);
}

int compareFields(const void* first, const void* second)
{
    if(first == NULL || second == NULL)
        return -1;
    Field* field1 = (Field*) first;
    Field* field2 = (Field*) second;

    char tmp1[100];
    char tmp2[100];

    sprintf(tmp1, "%s %s", field1->tag, field1->value);
    sprintf(tmp2, "%s %s", field2->tag, field2->tag);

    return(strcmp(tmp1, tmp2));
}

char* printIndividual(void* toBePrinted)
{
    char* indivStr = malloc(sizeof(char) * 5000);
    if(toBePrinted == NULL)
        return indivStr;
    Individual* indiv = (Individual*) toBePrinted;
    sprintf(indivStr, "Individual:\n   Name: %s %s\n   Events:\n      ", indiv->givenName, indiv->surname);     //Adding the individual's name to the string
    List events = indiv->events;
    int eventNum = 1;
    while(events.head != NULL)  //Loop adds every event that the individual might have to the string
    {
        Event* event = (Event*)events.head->data;
        char* eventStr = printEvent(event);
        sprintf(indivStr, "%sEvent %d - %s\n      ", indivStr, eventNum, eventStr);
        free(eventStr);
        eventNum++;
        events.head = events.head->next;
    }
    sprintf(indivStr, "%s\n", indivStr);
    return indivStr;
}

void deleteIndividual(void* toBeDeleted)
{
    if(toBeDeleted == NULL)
        return;
    //Deleting all of the structs and lists before freeing the individual
    Individual* indiv = (Individual*) toBeDeleted;
    free(indiv->givenName);
    free(indiv->surname);
    clearList(&(indiv->events));
    clearList(&(indiv->families));
    clearList(&(indiv->otherFields));
    free(indiv);
}

int compareIndividuals(const void* first, const void* second)
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

    return strcmp(tmp1, tmp2);
}

char* printFamily(void* toBePrinted)    //Function adds husband and wife to the string (if they are present), and also their children
{
    char* famStr = malloc(sizeof(char) * 5000);
    if(toBePrinted == NULL)
        return famStr;
    Family* fam = (Family*) toBePrinted;
    strcpy(famStr, "Family:\n   ");
    if(fam->husband != NULL)
    {
        Individual* husband = (Individual*) fam->husband;
        sprintf(famStr, "%sHusband: %s %s", famStr, husband->givenName, husband->surname);      //Adding husband to the string
        sprintf(famStr, "%s\n   ", famStr);
    }
    if(fam->wife != NULL)
    {
        Individual* wife = (Individual*) fam->wife;
        sprintf(famStr, "%sWife: %s %s\n   ", famStr, wife->givenName, wife->surname);      //Adding wife to the string
    }
    List events = fam->events;
    if(events.head != NULL)
        sprintf(famStr, "%sEvents:\n      ", famStr);
    int eventNum = 1;
    while(events.head != NULL)  //Loop adds every event that the individual might have to the string
    {
        Event* event = (Event*)events.head->data;
        char* eventStr = printEvent(event);
        sprintf(famStr, "%sEvent %d - %s\n      ", famStr, eventNum, eventStr);
        free(eventStr);
        eventNum++;
        events.head = events.head->next;
    }

    List children = fam->children;
    if(children.head != NULL)
        sprintf(famStr, "%sChildren:\n      ", famStr);
    int childNum = 1;
    while(children.head != NULL)
    {
        Individual* child = (Individual*) children.head->data;
        sprintf(famStr, "%sChild %d - Name: %s %s\n      ", famStr, childNum, child->givenName, child->surname);    //Adding children to the string
        childNum++;
        children.head = children.head->next;
    }
    sprintf(famStr, "%s\n", famStr);
    return famStr;
}

void deleteFamily(void* toBeDeleted)
{
    if(toBeDeleted == NULL)
        return;
    Family* fam = (Family*) toBeDeleted;
    clearList(&(fam->children));
    clearList(&(fam->otherFields));
    clearList(&(fam->events));
    free(fam);
}

int compareFamilies(const void* first, const void* second)  //Function compares families based on their sizes
{
    if(first == NULL || second == NULL)
        return -1;
    Family* tmp1 = (Family*) first;
    Family* tmp2 = (Family*) second;

    List children1 = tmp1->children;
    List children2 = tmp2->children;

    int tmp1Num = 0;
    int tmp2Num = 0;

    if(tmp1->husband != NULL)
        tmp1Num++;
    if(tmp1->wife != NULL)
        tmp1Num++;
    if(tmp2->husband != NULL)
        tmp2Num++;
    if(tmp2->wife != NULL)
        tmp2Num++;

    while(children1.head != NULL)
    {
        tmp1Num++;
        children1.head = children1.head->next;
    }

    while(children2.head != NULL)
    {
        tmp2Num++;
        children2.head = children2.head->next;
    }
    if(tmp1Num == tmp2Num)
        return 0;
    if(tmp1Num < tmp2Num)
        return -1;
    else
        return 1;
}

char* printEvent(void* toBePrinted) 
{
    char* eventStr = malloc(sizeof(char) * 1000);
    strcpy(eventStr, "");
    if(toBePrinted == NULL)
        return eventStr;
    Event* event = (Event*) toBePrinted;
    sprintf(eventStr, "Type: %s", event->type);     //Adding the event type to the string
    if(strcmp(event->date, "") != 0)
        sprintf(eventStr, "%s, Date: %s", eventStr, event->date);   //Adding the event date to the string
    if(strcmp(event->place, "") != 0)
        sprintf(eventStr, "%s, Place: %s", eventStr, event->place);     //Adding the event place to the string

    return eventStr;
}

void deleteEvent(void* toBeDeleted)
{
    if(toBeDeleted == NULL)
        return;
    Event* tmp = (Event*)toBeDeleted;
    free(tmp->date);
    free(tmp->place);
    clearList(&(tmp->otherFields));
    free(tmp);
}

int compareEvents(const void* first, const void* second)
{
    if(first == NULL || second == NULL)
        return -1;
    Event* tmp1 = (Event*) first;
    Event* tmp2 = (Event*) second;

    return strcmp(tmp1->type, tmp2->type);  //Comparing event TYPES only (as stated by the prof)
}

char* printError(GEDCOMerror err)   //Function returns errors as human-readable strings
{
    char* errorStr = malloc(sizeof(char) * 100);
    if(err.type == OK)
        strcpy(errorStr, "OK");
    else if(err.type == INV_FILE)
        strcpy(errorStr, "Invalid file!");
    else if(err.type == INV_GEDCOM)
        strcpy(errorStr, "Invalid GEDCOM!");
    else if(err.type == INV_HEADER)
    {
        sprintf(errorStr, "Invalid header (line %d)!", err.line);
    }
    else if(err.type == INV_RECORD)
    {
        sprintf(errorStr, "Invalid record (line %d)!", err.line);
    }
    else if(err.type == OTHER_ERROR)
        strcpy(errorStr, "Other error!");
    else
        strcpy(errorStr, "Writer error!");
    return errorStr;
}

Individual* findPerson(const GEDCOMobject* familyRecord, bool (*compare)(const void* first, const void* second), const void* person)
{
    if(familyRecord == NULL || person == NULL)
        return NULL;

    List individuals = familyRecord->individuals;
    Individual* match = (Individual*)findElement(individuals, compare, person);     //Using function findElement from LinkedListAPI to match the individual
    return match;
}

List getDescendants(const GEDCOMobject* familyRecord, const Individual* person)
{
    List descendants = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    if(familyRecord == NULL || person == NULL)
        return descendants;

    //List families = familyRecord->families;
    ListIterator familyIter = createIterator(familyRecord->families);
    Family* fam;
    while((fam = nextElement(&familyIter)) != NULL)
    {
        Individual* husband = fam->husband;
        Individual* wife = fam->wife;
        if(compareIndividuals(person, husband) == 0 || compareIndividuals(person, wife) == 0)   //Searching every family for the person as one of the parents, and returning their children if found
        {
            ListIterator childIter = createIterator(fam->children);
            Individual* child;
            while((child = nextElement(&childIter)) != NULL)
            {
                insertBack(&descendants, child);
            }
            break;
        }
    }
    ListIterator descendantsIter = createIterator(descendants);
    ListIterator familyIter2 = createIterator(familyRecord->families);
    Individual* child2;
    while((child2 = nextElement(&descendantsIter)) != NULL)
    {
        Family* fam;
        while((fam = nextElement(&familyIter2)) != NULL)
        {
            Individual* husband = fam->husband;
            Individual* wife = fam->wife;
            if(compareIndividuals(child2, husband) == 0 || compareIndividuals(child2, wife) == 0)   //Searching every family for the person as one of the parents, and returning their children if found
            {
                ListIterator childIter = createIterator(fam->children);
                Individual* child;
                while((child = nextElement(&childIter)) != NULL)
                {
                    insertBack(&descendants, child);
                }
                break;
            }
        }
    }
    return descendants;
}

/**********************
Assignment 2 Functions
**********************/

GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj)    //Function will save the passed in GEDCOM object to a file with the name specified by fileName
{
    GEDCOMerror error;
    error.type = OK;
    error.line = -1;
    if(fileName == NULL || strcmp(fileName, "") == 0 || obj == NULL)    //If statement checks for any invalid parameters
    {
        error.type = WRITE_ERROR;
        return error;
    }
    FILE* file = fopen(fileName, "w");
    if(file == NULL)
    {
        error.type = WRITE_ERROR;
        return error;
    }
    Header* header = obj->header;
    Submitter* submitter = obj->submitter;
    List individuals = obj->individuals;
    List families = obj->families;
    List IDlist = initializeList(dummyPrint, deleteID, compareIDs);     //A list of XREF IDs

    fprintf(file, "0 HEAD\n");
    fprintf(file, "1 SOUR %s\n", header->source);
    fprintf(file, "1 GEDC\n2 VERS %.1f\n2 FORM LINEAGE-LINKED\n", header->gedcVersion);
    fprintf(file, "1 CHAR ");
    if(header->encoding == ANSEL)
        fprintf(file, "ANSEL\n");
    else if(header->encoding == UTF8)
        fprintf(file, "UTF-8\n");
    else if(header->encoding == UNICODE)
        fprintf(file, "UNICODE\n");
    else if(header->encoding == ASCII)
        fprintf(file, "ASCII\n");
    fprintf(file, "1 SUBM @S1@\n");

    ListIterator familyIter = createIterator(families);
    Family* theFam;
    int famNum = 1;
    while((theFam = nextElement(&familyIter)) != NULL)  //While loop creates a new XREF ID for every family in the object, and adds it to the IDlist
    {
        ID* newFamID = malloc(sizeof(ID));
        sprintf(newFamID->tag, "@F%d@", famNum);
        sprintf(newFamID->type, "FAM");
        newFamID->reference = theFam;
        insertBack(&IDlist, newFamID);
        famNum++;
    }

    ListIterator indivIter = createIterator(individuals);
    Individual* person;
    int indivNum = 1;
    while((person = nextElement(&indivIter)) != NULL)   //While loop creates a new XREF ID for every individual in the object, and adds it to the IDlist
    {
        ID* newID = malloc(sizeof(ID));
        sprintf(newID->tag, "@I%d@", indivNum);
        sprintf(newID->type, "INDI");
        newID->reference = person;
        insertBack(&IDlist, newID);
        fprintf(file, "0 @I%d@ INDI\n", indivNum);
        fprintf(file, "1 NAME %s /%s/\n", person->givenName, person->surname);
        indivNum++;
        ListIterator personOtherIter = createIterator(person->otherFields);
        Field* field;
        while((field = nextElement(&personOtherIter)) != NULL)  //While loop searches the otherFields of an individual and makes sure GIVN and SURN are correctly outputted to the file
        {
            if(strcmp(field->tag, "GIVN") == 0)
                fprintf(file, "2 %s %s\n", field->tag, field->value);
            else if(strcmp(field->tag, "SURN") == 0)
                fprintf(file, "2 %s %s\n", field->tag, field->value);
            else
                fprintf(file, "1 %s %s\n", field->tag, field->value);
        }
        ListIterator personEventIter = createIterator(person->events);
        Event* event;
        while((event = nextElement(&personEventIter)) != NULL)  //While loop saves all of the events of an individual to the file
        {
            fprintf(file, "1 %s\n", event->type);
            if(strcmp(event->date, "") != 0)
                fprintf(file, "2 DATE %s\n", event->date);
            if(strcmp(event->place, "") != 0)
                fprintf(file, "2 PLACE %s\n", event->place);
        }
        ListIterator personFamIter = createIterator(person->families);
        Family* fam;
        while((fam = nextElement(&personFamIter)) != NULL)  //While loop makes sure to save the correct FAMC and FAMS tags at the end of every individual in the file
        {
            ListIterator IDfamIter = createIterator(IDlist);
            ID* famID;
            while((famID = nextElement(&IDfamIter)) != NULL)
            {
                if(strcmp(famID->type, "FAM") == 0)
                {
                    Family* familyRef = (Family*) famID->reference;
                    if(familyRef == fam)
                    {
                        if(person == familyRef->husband || person == familyRef->wife)
                        {
                            fprintf(file, "1 FAMS %s\n", famID->tag);
                        }
                        else
                        {
                            fprintf(file, "1 FAMC %s\n", famID->tag);
                        }

                        break;
                    }
                }
            }
        }
    }

    ListIterator famIter = createIterator(families);
    Family* fam;
    while((fam = nextElement(&famIter)) != NULL)    //While loop goes through every family in the GEDCOM object and saves everything that is required in the file
    {
        ListIterator IDiter = createIterator(IDlist);
        ID* famID;
        while((famID = nextElement(&IDiter)) != NULL)
        {
            if(strcmp(famID->type, "FAM") == 0)
            {
                Family* famRef = (Family*) famID->reference;
                if(famRef == fam)
                {
                    fprintf(file, "0 %s FAM\n", famID->tag);
                    break;
                }
            }
        }
        if(fam->husband != NULL)    //If the husband exists, save him to the file
        {
            ListIterator IDindivIter = createIterator(IDlist);
            ID* indivID;
            while((indivID = nextElement(&IDindivIter)) != NULL)
            {
                if(strcmp(indivID->type, "INDI") == 0)
                {
                    Individual* indivRef = (Individual*) indivID->reference;
                    if(indivRef == fam->husband)
                    {
                        fprintf(file, "1 HUSB %s\n", indivID->tag);
                    }
                }
            }
        }
        if(fam->wife != NULL)   //If the wife exists, save her to the file
        {
            ListIterator IDindivIter = createIterator(IDlist);
            ID* indivID;
            while((indivID = nextElement(&IDindivIter)) != NULL)
            {
                if(strcmp(indivID->type, "INDI") == 0)
                {
                    Individual* indivRef = (Individual*) indivID->reference;
                    if(indivRef == fam->wife)
                    {
                        fprintf(file, "1 WIFE %s\n", indivID->tag);
                    }
                }
            }
        }
        ListIterator eventIter = createIterator(fam->events);
        Event* event;
        while((event = nextElement(&eventIter)) != NULL)    //While loop saves all of the family events to the file
        {
            fprintf(file, "1 %s\n", event->type);
            if(strcmp(event->date, "") != 0)
                fprintf(file, "2 DATE %s\n", event->date);
            if(strcmp(event->place, "") != 0)
                fprintf(file, "2 PLACE %s\n", event->place);
        }
        ListIterator childIter = createIterator(fam->children);
        Individual* child;
        while((child = nextElement(&childIter)) != NULL)    //While loop saves all of the family's CHIL tags to the file
        {
            ListIterator IDindivIter = createIterator(IDlist);
            ID* indivID;
            while((indivID = nextElement(&IDindivIter)) != NULL)
            {
                if(strcmp(indivID->type, "INDI") == 0)
                {
                    Individual* indivRef = (Individual*) indivID->reference;
                    if(indivRef == child)
                    {
                        fprintf(file, "1 CHIL %s\n", indivID->tag);
                        break;
                    }
                }
            }
        }
    }

    //Finally, the submitter is saved and the TRLR is added to the end of the file
    fprintf(file, "0 @S1@ SUBM\n");
    fprintf(file, "1 NAME %s\n", submitter->submitterName);
    if(strcmp(submitter->address, "") != 0)
        fprintf(file, "1 ADDR %s\n", submitter->address);
    fprintf(file, "0 TRLR");
    fclose(file);
    clearList(&IDlist);
    return error;
}

ErrorCode validateGEDCOM(const GEDCOMobject* obj)   //Function checks if the inputted object is a valid GEDCOM file
{
    if(obj == NULL || obj->header == NULL || obj->submitter == NULL)
        return INV_GEDCOM;

    if(strcmp(obj->header->source, "") == 0 || obj->header->submitter == NULL)
        return INV_HEADER;

    if(strcmp(obj->submitter->submitterName, "") == 0 || obj->families.deleteData == NULL || obj->individuals.deleteData == NULL)
        return INV_RECORD;

    return OK;
}

List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen)    //Function uses recursion in order to return a list of generations of descendants of the inputted individual
{
    List generations = initializeList(dummyPrint, deleteList, compareGenIndivs);
    int currentGen = maxGen;
    if(maxGen == 0)
        currentGen = 20;    //If 0 is entered as maxGen, go up to 20 generations
    if(familyRecord == NULL || person == NULL)
        return generations;
    bool checkChildren = false;
    ListIterator checkIndivIter = createIterator(familyRecord->families);
    Family* checkFamily;
    while((checkFamily = nextElement(&checkIndivIter)) != NULL)
    {
        Individual* husband = checkFamily->husband;
        Individual* wife = checkFamily->wife;
        if(compareIndivs(person, husband) == 0 || compareIndivs(person, wife) == 0)
        {
            if(checkFamily->children.head != NULL)
            {
                checkChildren = true;
            }
        }
    }
    if(checkChildren == false)
        return generations;
    List* children = malloc(sizeof(List));
    *children = initializeList(dummyPrint, dummyFunc, compareGenIndivs);
    ListIterator familyIter = createIterator(familyRecord->families);
    Family* fam;
    while((fam = nextElement(&familyIter)) != NULL)
    {
        Individual* husband = fam->husband;
        Individual* wife = fam->wife;
        if(compareIndivs(person, husband) == 0 || compareIndivs(person, wife) == 0)   //Searching every family for the person as one of the parents
        {
            if(currentGen != 0)
            {
                ListIterator nextGenIter = createIterator(fam->children);
                Individual* nextChild;
                while((nextChild = nextElement(&nextGenIter)) != NULL)
                {
                    insertSorted(children, nextChild);    //Adding the child to the current generation
                }
                while((fam = nextElement(&familyIter)) != NULL)     //Making sure to look for other families that this individual could be a parent of (more than one FAMS tag)
                {
                    husband = fam->husband;
                    wife = fam->wife;
                    if(compareIndivs(person, husband) == 0 || compareIndivs(person, wife) == 0)   //Searching every family for the person as one of the parents
                    {
                        if(currentGen != 0)
                        {
                            ListIterator anotherGenIter = createIterator(fam->children);
                            while((nextChild = nextElement(&anotherGenIter)) != NULL)
                            {
                                insertSorted(children, nextChild);    //Adding the child to the current generation
                            }
                        }
                    }
                }
            }
        }
    }
    insertBack(&generations, children);
    getDescendantsRecur(familyRecord, *children, currentGen - 1, &generations);    //Function call to the recursive function with the children of the passed individual as the first iteration
    return generations;
}

List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen)   //Function uses recursion in order to return a list of generations of ancestors of the inputted individual
{
    List generations = initializeList(dummyPrint, deleteList, compareIndividuals);
    int currentGen = maxGen;
    if(maxGen == 0)
        currentGen = 20;    //If 0 is entered as maxGen, go up to 20 generations
    if(familyRecord == NULL || person == NULL)
        return generations;
    bool checkParents = false;
    ListIterator checkIndivIter = createIterator(familyRecord->families);
    Family* checkFamily;
    while((checkFamily = nextElement(&checkIndivIter)) != NULL)
    {
        if(checkFamily->children.head != NULL)
        {
            ListIterator checkChildIter = createIterator(checkFamily->children);
            Individual* child;
            while((child = nextElement(&checkChildIter)) != NULL)
            {
                if(compareIndivs(person, child) == 0)
                {
                    checkParents = true;
                }
            }
        }
    }
    if(checkParents== false)
        return generations;
    List* parents = malloc(sizeof(List));
    *parents = initializeList(dummyPrint, dummyFunc, compareGenIndivs);
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
            if(compareIndivs(person, kid) == 0)
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
            ListIterator listIter = createIterator(*parents);
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
                insertSorted(parents, husband);
            if(checkWifeDup == false)
                insertSorted(parents, wife);
        }
    }
    insertBack(&generations, parents);
    getAncestorsRecur(familyRecord, *parents, currentGen - 1, &generations);  //Function call to the recursive function with the parents of the passed in individual as the first iteration
    return generations;
}

char* indToJSON(const Individual* ind)  //Function creates a JSON string that describes an individual
{
    char* str = malloc(sizeof(char) * 300);
    if(ind != NULL)
        sprintf(str, "{\"givenName\":\"%s\",\"surname\":\"%s\"}", ind->givenName, ind->surname);
    return str;
}

Individual* JSONtoInd(const char* str)  //Function creates an individual that the inputted string gives the details of
{
    if(str == NULL || strlen(str) < 29)
        return NULL;
    int quoteNum = 0;
    char givenName[100];
    int givenCount = 0;
    char surName[100];
    int surCount = 0;
    strcpy(givenName, "");
    strcpy(surName, "");
    for(int i = 0; i < strlen(str); i++)    //For loop gets information from the inputted string character by character
    {
        if(str[i] == '\"')
            quoteNum++;
        else if(quoteNum == 3)
            givenName[givenCount++] = str[i];

        else if(quoteNum == 7)
            surName[surCount++] = str[i];
    }
    if(quoteNum != 8)
        return NULL;
    givenName[givenCount] = '\0';   //Making sure to end the string with a null terminator
    surName[surCount] = '\0';
    Individual* person = malloc(sizeof(Individual));
    person->givenName = malloc(sizeof(char) * 100);
    person->surname = malloc(sizeof(char) * 100);
    strcpy(person->givenName, givenName);
    strcpy(person->surname, surName);
    person->events = initializeList(printEvent, deleteEvent, compareEvents);
    person->families = initializeList(printFamily, dummyFunc, compareFamilies);     //Initializing all of the lists of the individual
    person->otherFields = initializeList(printField, deleteField, compareFields);

    return person;
}

GEDCOMobject* JSONtoGEDCOM(const char* str)     //Function will create a new GEDCOMobject and the inputted string will give information on the header and submitter
{
    if(str == NULL || strlen(str) < 70)
        return NULL;
    int quoteNum = 0;

    char source[100];
    int sourceCount = 0;

    char gedc[100];
    int gedcCount = 0;

    char encoding[100];
    int encoCount = 0;

    char subName[100];
    int subNameCount = 0;

    char subAddress[100];
    int subAddressCount = 0;

    strcpy(source, "");
    strcpy(gedc, "");
    strcpy(encoding, "");
    strcpy(subName, "");
    strcpy(subAddress, "");
    for(int i = 0; i < strlen(str); i++)    //For loop will parse information specified by the JSON string into separate variables which will be used to create the GEDCOMobject
    {
        if(str[i] == '\"')
            quoteNum++;
        else if(quoteNum == 3)
            source[sourceCount++] = str[i];

        else if(quoteNum == 7)
            gedc[gedcCount++] = str[i];

        else if(quoteNum == 11)
            encoding[encoCount++] = str[i];

        else if(quoteNum == 15)
            subName[subNameCount++] = str[i];

        else if(quoteNum == 19)
            subAddress[subAddressCount++] = str[i];
    }
    if(quoteNum != 20)
        return NULL;
    int dotNum = 0;
    for(int i = 0; i < strlen(gedc); i++)   //Checking if the gedcVersion is valid
    {
        if(gedc[i] == '.')
            dotNum++;
        if(gedc[i] != '.' && (gedc[i] - '0' < 0 || gedc[i] - '0' > 9))
            return NULL;
    }
    if(dotNum != 1)
    {
        return NULL;
    }
    source[sourceCount] = '\0';
    gedc[gedcCount] = '\0';
    encoding[encoCount] = '\0';
    subName[subNameCount] = '\0';
    subAddress[subAddressCount] = '\0';
    CharSet newEncoding;
    if(strcmp(encoding, "ANSEL") == 0)
        newEncoding = ANSEL;
    else if(strcmp(encoding, "UTF8") == 0 || strcmp(encoding, "UTF-8") == 0)
        newEncoding = UTF8;
    else if(strcmp(encoding, "UNICODE") == 0)
        newEncoding = UNICODE;
    else if(strcmp(encoding, "ASCII") == 0)
        newEncoding = ASCII;
    else
        return NULL;
    
    //Allocating memory for a new GEDCOMobject and all of its substructures
    GEDCOMobject* newObj = malloc(sizeof(GEDCOMobject));
    newObj->header = malloc(sizeof(Header));
    newObj->individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    newObj->families = initializeList(printFamily, deleteFamily, compareFamilies);
    newObj->submitter = malloc(sizeof(Submitter) + 100 * sizeof(char));

    //Creating the GEDCOMobject using the parsed variables from the JSON string
    strcpy(newObj->header->source, source);
    newObj->header->gedcVersion = atof(gedc);
    newObj->header->encoding = newEncoding;
    newObj->header->otherFields = initializeList(printField, deleteField, compareFields);
    strcpy(newObj->submitter->submitterName, subName);
    strcpy(newObj->submitter->address, subAddress);
    newObj->submitter->otherFields = initializeList(printField, deleteField, compareFields);
    newObj->header->submitter = newObj->submitter;  //Making sure the header points to the submitter
    
    return newObj;
}

void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded)  //Function adds the inputted individual to the individuals list in the inputted GEDCOMobject
{
    if(obj == NULL || toBeAdded == NULL)
        return;
    Individual* newInd = (Individual*) toBeAdded;
    insertBack(&obj->individuals, newInd);
}

char* iListToJSON(List iList)   //Function returns a dynamically allocated string containing informaton on a list of individuals
{
    char* listStr = malloc(sizeof(char) * 2000);
    if(iList.head == NULL)
    {
        strcpy(listStr, "[]");
        return listStr;
    }
    strcpy(listStr, "[");
    ListIterator listIter = createIterator(iList);
    Individual* person = nextElement(&listIter);
    while(person != NULL)
    {
        char* indivStr = indToJSON(person); //Calling the function indToJSON to get the string for every individual
        strcat(listStr, indivStr);
        free(indivStr);
        if((person = nextElement(&listIter)) != NULL)
            strcat(listStr, ",");
    }
    strcat(listStr, "]");

    return listStr;
}

char* gListToJSON(List gList)   //Function returns a dynamically allocated string containing information on a list of lists of individuals (generations)
{
    char* genStr = malloc(sizeof(char) * 5000);
    if(gList.head == NULL)
    {
        strcpy(genStr, "[]");
        return genStr;
    }
    strcpy(genStr, "[");
    ListIterator genIter = createIterator(gList);
    List* list = nextElement(&genIter);
    while(list != NULL)
    {
        char* listStr = iListToJSON(*list);     //Calling function iListToJSON to get a string for every generation
        strcat(genStr, listStr);
        free(listStr);
        if((list = nextElement(&genIter)) != NULL)
            strcat(genStr, ",");
    }
    strcat(genStr, "]");

    return genStr;
}

void deleteGeneration(void* toBeDeleted)
{
    
}
int compareGenerations(const void* first,const void* second)
{
    return 0;
}
char* printGeneration(void* toBePrinted)
{
    return NULL;
}
