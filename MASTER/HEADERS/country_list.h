#pragma once

typedef struct CountryListNode             
{                                           
    char* data; 
    struct CountryListNode *next; 
}CountryListNode; 

typedef struct CountryList
{
    struct CountryListNode* first;
    struct CountryListNode* last;
}CountryList;

CountryList* initCountry();

void appendListCountry(CountryList* , char* );

void printListCountry(CountryListNode*);             

void freeListCountry(CountryList*);       
