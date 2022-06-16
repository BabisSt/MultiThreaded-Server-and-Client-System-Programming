#pragma once

#include "list.h"
#include "avl.h"

typedef struct BucketArray             //domh pou exei sting gia thn astheneia kai dexetai to dentro ths astheneias
{
   char* string;
   Tree* tree;
}BucketArray;


typedef struct HashBucket              //To bucket me sunolika stoixeia,an einai adeios,deikth ston epomeno kai deikth sthn domh bucketarray
{
   int totalvalues;
   bool isempty;  
   BucketArray *bucketarray;
   struct HashBucket *next;
}HashBucket;


typedef struct HashTable               //Hashtable me plhthos stoixeiwn,megethos bucket,kai diekth se bucket
{
   int size;
   int  bucketsize;
   struct HashBucket* hashbucket;
}HashTable;


HashTable* initTable(int,int);                     //arxikopoihsh

void deleteTable(HashTable*);                      //diagrafh table
void deleteBucket(HashBucket**,bool);              //diagrafhbucket

void printTable(HashTable*,int);                   //ektupwsh table

int hash(char*,int);                               //hash function

bool InsertTable(HashTable*,Patient*,int);         //eisagwgh sto table
void InsertBucket(HashBucket* ,Patient*,int);      //eisagwgh bucket(authn kalei to table gia na eisagei bucket)
HashBucket* InsertNewBucket(Patient*,int,int);     //neo bucket an einai aparaithto
bool addHT(HashTable*,Patient*,int);               //prosthikh asthenh se Hashtable
bool InsertListToTable(List*,HashTable*,int);      //prosthikh listas se hashtable

///////QUERIES/////
// void globalDiseaseStats(HashTable*,char*,char*);
// void diseaseFrequency(HashTable*,char*,char*,char*,char*);
// void insertPatientRecord(HashTable*,HashTable*,List*,char*,char*,char*,char*,char*,char*,char*);
// void recordPatientExit(HashTable*,char*,char*);
// void numCurrentPatients(HashTable*,char*);