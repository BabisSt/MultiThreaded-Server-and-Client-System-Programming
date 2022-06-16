#pragma once

#include "patient.h"

typedef struct ListNode             //Komvos Listas
{ 
    Patient* data; 
    struct ListNode *next; 
}ListNode; 

typedef struct List                 //Lista me duo deiktes ston prwto kai sto teleutaio komvo
{
    struct ListNode *last;
    struct ListNode *first;
}List;

void appendListPatient(List* , Patient* );//Prosthikh


void freeListPatient(List* );             //Diagrafh

bool searchListID(List* , char* ); //Elegxos gia diplotupo ID

void printListPatient(List*);             //Ektupwsh

List* initList();                  //Arxikopoihsh

bool updateExitDate(const List*,Patient*); // phgainei sto kombo ths listas kai allazei to exitDate