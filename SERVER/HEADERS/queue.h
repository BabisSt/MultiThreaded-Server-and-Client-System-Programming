#pragma once


typedef struct ListNode             //Komvos Listas
{ 
    char* port;
    int* client_socket; 
    int socket;
    int job;
    struct ListNode *next; 
}ListNode; 

typedef struct List
{
    int buffersize;
    ListNode* first;
    ListNode* last;
}List;

List* initList(int);

void appendList(List*,int*,int);//Prosthikh
ListNode* popList(List*);
