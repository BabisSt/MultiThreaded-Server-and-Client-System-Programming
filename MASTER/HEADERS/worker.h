#pragma once
#include "country_list.h"
#include "list.h"
#include "stats.h"
#include "hash.h"

typedef struct ListNodeWorker            
{ 
    int fd_read;             
    int fd_write; 
    CountryListNode* countries;        
}ListNodeWorker; 

ListNodeWorker* initArray(int);     

int RunWorker(char*,int,int,int,int);  

void appendListCountries(ListNodeWorker*, char*);  

void emptyworkerNode(ListNodeWorker*);       


int PutPatientsIntoStructs(char*,List*, char*, char*, List_of_Stats*,List*); 

void perror_exit(char*);

int start_server(uint16_t);

void DoTheQueries(int,HashTable*,HashTable*);

