#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HEADERS/patient.h"
#include "../HEADERS/stats.h"

//NA ALLAKSW

String_of_stats* initOfChar()
{
    String_of_stats* new_char = malloc(sizeof(String_of_stats));
    new_char->age_til_20 = NULL;
    new_char->age_til_40 = NULL;
    new_char->age_til_60 = NULL;
    new_char->age_over_60 = NULL;
    new_char->country = NULL;
    new_char->file = NULL;
    new_char->virus = NULL;

    return new_char;

}

List_of_Stats* initListOfStats()
{
    List_of_Stats* a = malloc(sizeof(List_of_Stats));
    a->first = NULL;
    a->last = NULL;
    return a;
}

void freeStats(String_of_stats* string)
{
    if(string !=NULL)
    {
        free(string->age_til_20);
        free(string->age_til_40);
        free(string->age_til_60);
        free(string->age_over_60);

        free(string-> file);
        free(string-> country);
        free(string-> virus);                       
    }
    free(string);
}

void emptyStats(List_of_Stats* list)
{
    Node_of_stats* tmp;
    while(list->first !=NULL)
    {
        freeStats(list->first->item);

        tmp= list->first->next;
        list->first->next = NULL;
        free(list->first);
        list->first = tmp;

    }
    free(list->first);
}

void informCharList(List_of_Stats* list, Patient* patient)
{
    Node_of_stats* tmp = list->first;

    bool found = false;
    
    while ((tmp!=NULL) && (found==false))
    {   

        if( strcmp(patient->diseaseID,tmp->item->virus)==0 && strcmp(patient->country,tmp->item->country)==0 && strcmp(patient->entryDate, tmp->item->file)==0)
        {
             char* patient_age = strdup(patient->age);
            int patient_age_int = atoi(patient_age);

            if(patient_age_int<=20)
            {
                //enhmerwsh ths listas tmp gia thn nea hlikia
                char* age_num = strdup(tmp->item->age_til_20);
                int age_num_int = atoi(age_num)+1;
                free(tmp->item->age_til_20);
                tmp->item->age_til_20 = malloc(3*sizeof(char));
                sprintf(tmp->item->age_til_20, "%d", age_num_int);
                free(age_num);
            }
            
            else if(patient_age_int<=40)
            {
                char* age_num = strdup(tmp->item->age_til_40);
                int age_num_int = atoi(age_num)+1;
                free(tmp->item->age_til_40);
                tmp->item->age_til_40 = malloc(3*sizeof(char));
                sprintf(tmp->item->age_til_40, "%d", age_num_int);
                free(age_num);
            }
            
            else if(patient_age_int<=60)
            {
                char* age_num = strdup(tmp->item->age_til_60);
                int age_num_int = atoi(age_num)+1;
                free(tmp->item->age_til_60);
                tmp->item->age_til_60 = malloc(3*sizeof(char));
                sprintf(tmp->item->age_til_60, "%d", age_num_int);
                free(age_num);
            }
            else
            {
                char* age_num = strdup(tmp->item->age_over_60);
                int age_num_int = atoi(age_num)+1;
                free(tmp->item->age_over_60);
                tmp->item->age_over_60 = malloc(3*sizeof(char));
                sprintf(tmp->item->age_over_60, "%d", age_num_int);
                free(age_num);
            }
            free(patient_age);
            found=true;
            break;
        }
        tmp=tmp->next;
    }
    if(found==false) //AN o asthenhs den uparxei prepei na ton ftiaskoume
    {
        Node_of_stats* new_node = malloc(sizeof(Node_of_stats));
        new_node->next = NULL;
        new_node->item = initOfChar();
        new_node->item->age_til_20= strdup("0");
        new_node->item->age_til_40= strdup("0");
        new_node->item->age_til_60= strdup("0");
        new_node->item->age_over_60= strdup("0");
        new_node->item->file = strdup(patient->entryDate);
        new_node->item->country = strdup(patient->country);
        new_node->item->virus = strdup(patient->diseaseID);

        //char* patient_age = strdup(patient->age);
        int patient_age_int = atoi(patient->age);
        if(patient_age_int<=20)
        {
            free(new_node->item->age_til_20);
            new_node->item->age_til_20 = strdup("1");
        }
        else if(patient_age_int<=40)
        {
            free(new_node->item->age_til_40);
            new_node->item->age_til_40 = strdup("1");
        }
        else if(patient_age_int<=60)
        {
            free(new_node->item->age_til_60);
            new_node->item->age_til_60 = strdup("1");
        }
        else
        {
            free(new_node->item->age_over_60);
            new_node->item->age_over_60 = strdup("1");
        }

        tmp = list->first;
        if(tmp == NULL)  
        { 
            list->first = new_node;
            list->first->next = NULL;

        }
        else
        {

            while (true)
            {
                if(tmp->next == NULL)
                {
                    tmp->next = new_node;
                    break;
                }
                tmp = tmp->next;
            }

            
        }
        //freeStats(new_node->item);
        //free(patient_age);
        //free(new_node);
    }
    //free(patient_disease);
}

char* concatStats(String_of_stats* string)
{
    char* big_string = malloc( (strlen(string->age_til_20)+1+strlen(string->age_til_40)+1+strlen(string->age_til_60)+1+strlen(string->age_over_60)+1+strlen(string->file)+1+strlen(string->country)+1+strlen(string->virus)+1)); 

    strcpy(big_string, string->age_til_20 );
    strcat(big_string, " ");
    strcat(big_string, string->age_til_40);
    strcat(big_string, " ");
    strcat(big_string, string->age_til_60);
    strcat(big_string, " ");
    strcat(big_string, string->age_over_60);
    strcat(big_string, " ");
    strcat(big_string, string->file);
    strcat(big_string, " ");
    strcat(big_string, string->country);
    strcat(big_string, " ");
    strcat(big_string, string->virus);

    return big_string;
}

void concatPrint(char* big_string)
{
    char* tok = strtok(big_string, " ");
    printf("age_til_20 is %s\n", tok);

    tok = strtok(NULL, " ");
    printf("age_til_40 is %s\n", tok);

    tok = strtok(NULL, " ");
    printf("age_til_60 is %s\n", tok);

    tok = strtok(NULL, " ");
    printf("age_over_60 is %s\n", tok);

    tok = strtok(NULL, " ");
    printf("file is %s\n", tok);

    tok = strtok(NULL, " ");
    printf("country is %s\n", tok);

    tok = strtok(NULL, " ");
    printf("virus is %s\n", tok);

    printf("\n");
}
