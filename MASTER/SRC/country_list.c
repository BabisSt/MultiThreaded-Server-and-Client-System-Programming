#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "../HEADERS/country_list.h"


CountryList* initCountry()
{
    CountryList* a = malloc(sizeof(CountryList));
    a->first = NULL;
    a->last =NULL;
    return a;
}

void appendListCountry(CountryList* list, char* new_data) 
{ 
    
    CountryListNode* new_node =  malloc(sizeof(struct CountryListNode)); 
    new_node->data = malloc((strlen(new_data)+1));
    strcpy(new_node->data,new_data);

    if (list->first ==  NULL)
    {
        list->first = new_node;
        list->last =list->first;
        list->first->next = NULL;
    }
    else
    {
        list->last->next = new_node;
        list->last = new_node;
        list->last->next = NULL;
    }
    

}


void printListCountry(CountryListNode* head)
{
    CountryListNode* tmp = head;
    while (tmp !=NULL)
    {
        printf("Country : %s \n",tmp->data);
        tmp = tmp->next;
    }

}

void freeListCountry(CountryList* list)
{
   CountryListNode* tmp=list->first;
   while (list->first != NULL)
   {  
       tmp= list->first;
       list->first = list->first->next;
       free(tmp->data);
       free(tmp);  
   }   

}

