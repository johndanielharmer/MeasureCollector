#ifndef __A1FUNCS__
#define __A1FUNCS__
#include "GEDCOMparser.h"

//Struct keeps track of every XREF ID and also a pointer to the record that holds that ID, used to link families <-> individuals, and header <-> submitter 
typedef struct id
{
    char type[10];
    char tag[50];
    void* reference;
}ID;

/** Function takes in a pointer to a header struct, and the tokens that were previously parsed in order to
 ** successfully initialize the header record inside the GEDCOMobject.
 **/
GEDCOMerror parseHeader(Header* header, char* tokens[500][100], int* headerEndLine, List* IDlist);
/** Function takes in a pointer to a list of individuals, and the tokens that were previously parsed in order to
 ** add the individual to the GEDCOMobject, along with all of the events and otherFields.
 **/
GEDCOMerror parseIndividual(List* individuals, char* tokens[500][100], int* recordEndLine, List* IDlist);
/** Function takes in a pointer to a list of families, and the tokens that were previously parsed in order to
 ** add the family to the GEDCOMobject, along with all of the otherFields. The function also assigns every
 ** XREF pointer to the appropriate family and individual.
 **/
GEDCOMerror parseFamily(List* families, char* tokens[500][100], int* recordEndLine, List* IDlist);
/** Function takes in a pointer to a submitter struct, and the tokens that were previously parsed in order to
 ** initialize the submitter record inside the GEDCOM object. The function also assigns the XREF pointer of the
 ** submitter to the header->submitter pointer.
 **/
GEDCOMerror parseSubmitter(Submitter* submitter, char* tokens[500][100], int* recordEndLine, List* IDlist, bool* checkSubmitter);
/** Function takes in a string pointer and makes that string uppercase.
 **/
void toUpper(char** str);
/** Function is used to delete the XREF ID struct inside the created list.
 **/
void deleteID(void* toBeDeleted);
/** Function compares two IDs and if they are the same it returns 0, if not, it returns -1.
 **/
int compareIDs(const void* first, const void* second);
/** Function does nothing (used for some list initializations)
 **/
void dummyFunc();
/** Function does nothing (used for some list initializations which require a return of char*)
 **/
char* dummyPrint();
/** Function is used to delete a generation list
 **/
void deleteList(void* toBeDeleted);
/** Function is used to recursively go through a list of individuals and add their descendants to
 ** the list of generations that is inputted as a parameter
 **/
void getDescendantsRecur(const GEDCOMobject* familyRecord, List people, int genNum, List* generations);
/** Function is used to recursively go through a list of individuals and add their ancestors to
 ** the list of generations that is inputted as a parameter
 **/
void getAncestorsRecur(const GEDCOMobject* familyRecord, List people, int genNum, List* generations);
/** Function is used to compare individuals using more than just their names, it also uses events
 **/
int compareIndivs(const void* first, const void* second);
/** Function is used to determine where the sorting of the individuals in a list and where they should be
 ** inserted
 **/
int compareGenIndivs(const void* first, const void* second);

#endif