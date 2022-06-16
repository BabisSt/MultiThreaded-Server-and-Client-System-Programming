#pragma once

#include "./patient.h"

typedef struct String_of_stats          //Domh pou exei ta stoixeia toy string
{
    char* age_til_20;
    char* age_til_40;
    char* age_til_60;
    char* age_over_60;

    char* file;
    char* country;
    char* virus;

}String_of_stats;

typedef struct Node_of_stats           
{
    String_of_stats* item;
    struct Node_of_stats* next;
}Node_of_stats;

typedef struct List_of_Stats
{
    Node_of_stats* first;
    Node_of_stats* last;
}List_of_Stats;


String_of_stats* initOfChar();          
List_of_Stats* initListOfStats();
void freeStats(String_of_stats*);       
void emptyStats(List_of_Stats*);        
void informCharList(List_of_Stats*,Patient*);  
char* concatStats(String_of_stats*);   
void concatPrint(char*);  