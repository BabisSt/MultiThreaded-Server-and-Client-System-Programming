#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../HEADERS/patient.h"



Patient* initPatient (char* line,char* EntryDate, char* Country)
{
//H morgh tou string einia pleon id/ENTER,EXIT(gia auot kai to keno)/FN/LN/Dis/Coun/Entry/Age/Exit
//Diabazoume mia gramh apo to arxeio kai kovoume to string katallhla.
    Patient* patient;
    patient = malloc(sizeof(Patient));
    char *token;
            token = strtok(line, " ");
            patient->recordID = strdup(token);
            if(patient->recordID == NULL)
            {
                printf("invalid id\n");
                return NULL;
            }
            token = strtok(NULL, " ");
             
            token = strtok(NULL, " ");
            patient->patientFirstName = strdup(token);
            if(patient->patientFirstName== NULL)
            {
                printf("invalid firist name\n");
                return NULL;
            } 

            token = strtok(NULL, " ");
            patient->patientLastName = strdup(token); 
            if(patient->patientLastName== NULL)
            {
                printf("invalid last name\n");
                return NULL;
            }
            
            token = strtok(NULL, " ");
            patient->diseaseID = strdup(token); 
            if(patient->diseaseID== NULL)
            {
                printf("invalid diseaseID\n");
                return NULL;
            }            

            patient->country = strdup(Country);
            if(patient->country==NULL)
            {
                printf("invalid country\n");
                return NULL;
            }
            
            patient->entryDate = strdup(EntryDate);
            if(patient->entryDate==NULL)
            {
                printf("invalid entrydate\n");
                return NULL;
            }

            token = strtok(NULL, " \n");
            patient->age = strdup(token);
            if(patient->age== NULL)
            {
                printf("invalid age\n");
                return NULL;
            }
        
            patient->exitDate = strdup("-");
            if(patient->exitDate == NULL)
            {
                printf("invalid exitdate\n");
                return NULL;
            }

       
       return patient;     
}

void printPatient(Patient* patient)
{
    printf("%s \n",patient->recordID);
    printf("%s \n",patient->patientFirstName);
    printf("%s \n",patient->patientLastName);
    printf("%s \n",patient->diseaseID);
    printf("%s \n",patient->country);
    printf("%s \n",patient->entryDate);
    printf("%s \n",patient->exitDate);
    printf("%s \n",patient->age);

}

void deletePatient(Patient* patient)
{
    free((*patient).recordID);  //theloume * giati einai dieuthinsh ston patient
    free((*patient).patientFirstName);
    free((*patient).patientLastName);
    free((*patient).diseaseID);
    free((*patient).country);
    free((*patient).entryDate);
    free((*patient).exitDate);
    free((*patient).age);
    free(patient);
}

void changeExitDateForPatient(Patient* patient, char* date)
{
    free((*patient).exitDate);  //diagrafh ths palias
    (*patient).exitDate = strdup(date);
}