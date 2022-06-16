#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HEADERS/queue.h"


// ListNode* first = NULL;
// ListNode* last = NULL;


List* initList(int buffersize)
{
    List* a = malloc(sizeof(List));
    a->first = NULL;
    a->last = NULL;
    a->buffersize = buffersize;
    return a;
}

void appendList(List* list,int* client_socket,int job) 
{ 
    
    ListNode* new_node =  malloc(sizeof(ListNode)); 
    
    new_node->job = job; 
    new_node->port = NULL;
    new_node->next = NULL; 
    new_node->socket  = *client_socket;
    
    if (list->last == NULL )   //eisagwgh sthn arxi an first einai NULL
    { 
        list->first = new_node;
    }   
    else                        //eisagwgh ston telos an h lista den einai adeia
    {
        list->last->next = new_node; 
    }  
    list->last = new_node;

} 

ListNode* popList(List* list)
{
    if(list->first == NULL)
    {
        return NULL;
    }
    else
    {
        //int* result = list->first->client_socket;
        ListNode* tmp = list->first;
        list->first = list->first->next;
        
        if(list->first==NULL)
        {
            list->last = NULL;
        }
        //free(tmp);
        return tmp;
    }
    return NULL;
    
}

// void freeListPatient(ListNode* to_empty)
// {
//     ListNode* head= to_empty;

//         while (head != NULL)
//         {
//             ListNode* tmp;
//             tmp = head;
//             head = head->next;
//             deletePatient(tmp->data);
//             free(tmp);   
//         }
//         free(to_empty);

// }
