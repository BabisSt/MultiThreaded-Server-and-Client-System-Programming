#pragma once

typedef struct Patient
{
    char* recordID;
    char* patientFirstName;
    char* patientLastName;
    char* diseaseID;
    char* country;
    char* entryDate;
    char* exitDate;
    char* age;      //prosthesame thn hlikia

}Patient;


Patient* initPatient (char*,char*,char*);   //Arxikopoihsh Asthenh

void printPatient(Patient*);    //Ektupwsh Asthenh

void deletePatient(Patient*);   //Diagrafh Asthenh

void changeExitDateForPatient(Patient*, char*); // Allagh tou exit gia ton asthenh