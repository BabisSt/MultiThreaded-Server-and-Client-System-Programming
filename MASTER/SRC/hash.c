#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HEADERS/avl.h"
#include "../HEADERS/hash.h"
#include "../HEADERS/patient.h"
#include "../HEADERS/date.h"
#include "../HEADERS/list.h"


HashTable* initTable (int size,int bucketsize)
{
   HashTable* a = malloc(sizeof(HashTable));
   a->size = size;               //ta entries
   a->bucketsize = bucketsize;   //megethos bucket
   
   int arraysize = a->bucketsize/sizeof(BucketArray);    //gia na doume posa tha dexetai to bucket
   
   a->hashbucket = malloc( (a->size)*sizeof(HashBucket));// malloc tou bucket

   //errors
   if(a->hashbucket == NULL)
   {
      free(a);
      return NULL;
   }
   if(arraysize == 0 )
   {
      return NULL;
   }

   for(int i=0;i<a->size;i++)
   {    
      (a->hashbucket)[i].next = NULL;                 //to epomeno keno
      (a->hashbucket)[i].totalvalues= arraysize;      //   
      (a->hashbucket)[i].isempty = true;
      (a->hashbucket)[i].bucketarray = malloc(arraysize*sizeof(BucketArray));//16 

      for(int j=0; j<(a->hashbucket)[i].totalvalues; j++)   //arxikopoihsh ths domhs bucketarray
      {
         ( (a->hashbucket[i]).bucketarray[j]).string = NULL;
         ( (a->hashbucket[i]).bucketarray[j]).tree = NULL;
      }
   }
   return a;

}

void deleteTable(HashTable* hashtable)
{ 
   if(hashtable!=NULL)
   {
      for(int i=0; i<hashtable->size; i++)
      {
         HashBucket *h = &((hashtable->hashbucket)[i]);
         deleteBucket(&h,false);
      }
      free(hashtable->hashbucket);
      free(hashtable);
   }
}

void deleteBucket(HashBucket** hashbucket,bool is_second)
{
   if(*hashbucket !=NULL)
   {
      if((*hashbucket)->bucketarray != NULL)
      {
         for(int i=0; i < (*hashbucket)->totalvalues; i++)
         {   
            ((*hashbucket)->bucketarray[i]).string = NULL;                 //string se NULL
            if( ((*hashbucket)->bucketarray[i]).tree!=NULL )
            {
               deleteTree(((*hashbucket)->bucketarray[i]).tree->root);     //diagrafh tou dentrou ths domhs
               free(((*hashbucket)->bucketarray[i]).tree);                 //free to dentro
            }
            ((*hashbucket)->bucketarray[i]).tree = NULL;                   //dentro se NULL
         }   
         free((*hashbucket)->bucketarray);
      } 
      deleteBucket(&((*hashbucket)->next),true);                           //diagrafh tou epomenou an uparxei allo bucket
      (*hashbucket)->next = NULL;                                          // next se NULL
      
      if(is_second == true)
         free(*hashbucket);
   }
}

void printTable(HashTable* hashtable,int DorC)
{
   
   if(DorC == 0)
   {
      printf("\t TREE DISEASE\n\n");
   }
   else
   {
      printf("\t TREE COUNTRIES\n\n");
   }
      for(int i=0;i<hashtable->size;i++)
      {
         HashBucket* tmp = &(hashtable->hashbucket[i]);
         if(tmp->isempty == false )
         {
            printf("\t%d. %s \n",i,tmp->bucketarray->string);
            printTree((hashtable->hashbucket[i]).bucketarray->tree->root);
            while(tmp->next !=NULL)
            {
               tmp=tmp->next;
               printf("\t%d. %s \n",i,tmp->bucketarray->string);
               printTree(tmp->bucketarray->tree->root);
               
            }
            printf("\n");
         }
         else
            printf("\t%d. ----\n",i);
      }
}

int hash(char * str,int size) {
	char* temp =str;
   int value =0;

	while (*temp != '\0')
   {
		value+= (int)(*temp);
      temp++;
	}
	return value % size;
}

bool InsertTable(HashTable *hashtable,Patient* patient, int DorC)       //DorC = metablhth gia kathorismo tou hashtable
{                                                                       //D = 0 gia disease
   int index = 0;                                                       //C = 1 gia country

   if(hashtable==NULL)
      return false;

   if(DorC == 0)//disease
   {    
      index = hash(patient->diseaseID,hashtable->size);
   }
   else         //country
   { 
     
      index = hash(patient->country,hashtable->size);
   }
   InsertBucket(&(hashtable->hashbucket)[index],patient,DorC);
   return true;

}

void InsertBucket(HashBucket* hashbucket,Patient* patient,int DorC)
{  
   bool thisbucket = 0;
   for(int i=0;i<hashbucket->totalvalues;i++)
   { 
      if((hashbucket->bucketarray[i]).string==NULL)//an den uparxei astheneia sto string
      {
        hashbucket->isempty = false;   //eimaste se keno bucket
         if(DorC==0)                   //se disease
         {
            (hashbucket->bucketarray[i]).string = patient->diseaseID;
         }
         else                          //se country
         {
            (hashbucket->bucketarray[i]).string = patient->country;
         }
         Tree* tree = initTree();      //dhmiourgia dentrou kai anathesh tou sthn domh
         (hashbucket->bucketarray[i]).tree = tree;
         
         insertTree((hashbucket->bucketarray[i]).tree,patient);   //bazoume ton asthenei mesa sto dentro apo th domh
         
         thisbucket=1;     //flag gia auto to bucket
         
         break;
      }
      else                                        //an uparxei astheneia sto string
      {
         if(DorC == 0 && strcmp((hashbucket->bucketarray[i]).string,patient->diseaseID) == 0 ) //einai to disease to asthenh idio me to strig
         {
            insertTree((hashbucket->bucketarray[i]).tree,patient);
            thisbucket = 1;   //flag gia auto to bucket
            break;
         }
         if(DorC == 1 && strcmp((hashbucket->bucketarray[i]).string,patient->country) == 0 )    //einai h xwra tou asthenh idia me to sting
         {
            insertTree((hashbucket->bucketarray[i]).tree,patient);
            thisbucket = 1;   //flag gia auto to bucket
            break;
         }
      }
   }

   if(thisbucket==0 && hashbucket->next!=NULL)    //eisagwgh se auto to bucket
   {
      InsertBucket(hashbucket->next,patient,DorC);
      thisbucket=1;
   }
   if(thisbucket==0) // dhmiourgoume neo bucket
   {
      
      hashbucket->next = InsertNewBucket(patient,DorC,hashbucket->totalvalues);
      InsertBucket(hashbucket->next,patient,DorC);
   }

}

HashBucket* InsertNewBucket(Patient* patient,int DorC,int totalvalues)
{
   HashBucket* nb= malloc(sizeof(HashBucket));
   if(nb == NULL)
      return NULL;

   nb->totalvalues= totalvalues;
   nb->next=NULL;
   nb->bucketarray = malloc(totalvalues*sizeof(BucketArray));

   for(int i=0;i<nb->totalvalues;i++)
   {
      (nb->bucketarray[i]).string = NULL;
      (nb->bucketarray[i]).tree = NULL;
   }
   return nb;
}


bool addHT(HashTable* table, Patient* patient,int CorD)
{
   int value;
   if(CorD == 0)  //disease
   {
      value = hash(patient->diseaseID,table->size);
   }
   else           //country
   {
      value = hash(patient->country,table->size);

   }
   InsertBucket( &(table->hashbucket)[value], patient, CorD);
   return true;
}

bool InsertListToTable(List* lista, HashTable* table, int CorD)
{
   ListNode* tmp = lista->first;
   while(tmp!=NULL)
   {
      if(!addHT(table,tmp->data, CorD))
      {
         return false;
      }
      tmp= tmp->next;
   }
   return true;
}












//QUERIES 1hs Ergasias
// void globalDiseaseStats(HashTable* hashtable,char* date1,char* date2)
// {
//    int entries =0;
   
//    for(int i=0;i<hashtable->size;i++)
//    {
//       if((hashtable->hashbucket[i]).bucketarray->string == NULL)  //elegxos an einai adeio to string,ara h domh
//          continue;
//       else if(date1 == NULL && date2 == NULL)                     //periptwsh pou den dinoume dates
//       {
//          for(int j=0; j<(hashtable->hashbucket[i]).totalvalues;j++)
//          {
//             printf("%s %d \n",((hashtable->hashbucket[i]).bucketarray[j]).string,searchTree((hashtable->hashbucket[i]).bucketarray->tree->root) );
//          }

//       }
//       else if((date1 != NULL && date2 == NULL) || (date1 == NULL && date2 !=NULL))     //an mia apo ta duo dates einai lathos
//       {
//          printf("ERROR\n");
//          return;
//       }
//       else                                                         //periptwsh pou dinoume dates
//       {
//          for(int j=0; j<(hashtable->hashbucket[i]).totalvalues;j++)
//          {
//             //date1>date2 1 date1<date2 -1 date1=date2 0
//             int compare1 = compare_dates(((hashtable->hashbucket[i]).bucketarray[j]).tree->root->treepatient->entryDate,date1);// 1 h 0
//             int compare2 = compare_dates(((hashtable->hashbucket[i]).bucketarray[j]).tree->root->treepatient->entryDate,date2);// -1 h 0
//             if( (compare1== 1 || compare1==0 ) && (compare2==-1 || compare2==0) )
//             {
//                entries =searchTree(((hashtable->hashbucket[i]).bucketarray[j]).tree->root);
//             }

//          }
//          printf("%s %d \n",(hashtable->hashbucket[i]).bucketarray->string,entries );
//          entries =0;
//       }
//    }
// }

void diseaseFrequency(HashTable* hashtable,char* virusname,char* date1,char* date2,char* country)
{
   int entries=0;
   for(int i=0;i<hashtable->size;i++)
   {
      if(country == NULL)                    //periptwsh pou den dinoume country
      {
         if((hashtable->hashbucket[i]).bucketarray->string == NULL)     //an einai keno mhn elenxeis
            continue;
         else
         {
            if(strcmp(virusname,(hashtable->hashbucket[i]).bucketarray->string) == 0)     //an eimase sto swsto virusname
            {  
            // //date1>date2 1 date1<date2 -1 date1=date2 0
            // int compare1 = compare_dates((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->entryDate,date1);// 1 h 0
            // int compare2 = compare_dates((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->entryDate,date2);// -1 h 0
               entries =searchTree((hashtable->hashbucket[i]).bucketarray->tree->root);

            }
            else
               continue;
         }
      }
      else                                  //periptwsh pou dinoume country
      {
         if((hashtable->hashbucket[i]).bucketarray->string == NULL)
            continue;
         else
         {  
            if(strcmp(virusname,(hashtable->hashbucket[i]).bucketarray->string) == 0)     //an eimaste sto swsto virusname
            {
               if(strcmp(country,(hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->country) == 0)  //an eimaste sto swsto country
               {  
                  // //date1>date2 1 date1<date2 -1 date1=date2 0
                  // int compare1 = compare_dates((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->entryDate,date1);// 1 h 0
                  // int compare2 = compare_dates((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->entryDate,date2);// -1 h 0
                  entries =searchTree((hashtable->hashbucket[i]).bucketarray->tree->root);
               }
            }
         }
      }
      printf("%s %d \n",virusname,entries);
      entries =0;
   }
}

// void insertPatientRecord(HashTable* hashtable_disease,HashTable* hashtable_country,List* lista,char* recordID,char*patientFirstName,char* patientLastName,char* diseaseID,char* country,char* entryDate,char* exitDate)
// {
//    Patient* patient;                                     //dhmiourgia asthenh kai arxikopoihsh
//    patient = malloc(sizeof(Patient));
//    patient->recordID=strdup(recordID);
//    patient->patientFirstName=strdup(patientFirstName);
//    patient->patientLastName=strdup(patientLastName);
//    patient->diseaseID = strdup(diseaseID);
//    patient->country = strdup(country);
//    patient->entryDate=strdup(entryDate);
   
//    if(exitDate == NULL)                               //elegxos gia to exitdate
//       patient->exitDate = strdup("-");
//    else
//       patient->exitDate = strdup(exitDate);

//    //errors
//    if((compare_dates(patient->entryDate,patient->exitDate)) == 1) // 1 gia date1>date2 / -1 gia date1<date2 / 0 gia date1=date2 / -1 gia -
//    {
//          printf("exitdate is later than entry date %s %s \n",patient->entryDate,patient->exitDate);
//          deletePatient(patient);
//    }
//    else
//    {
//       if((searchListID(lista,patient->recordID)) == false)           //eisagwges
//       {
//          appendListPatient(lista,patient);

//          InsertTable(hashtable_disease,patient,0);
//          InsertTable(hashtable_country,patient,1);
//       }
//       else if ((searchListID(lista,patient->recordID)) == true)      //an uparxei to id
//       {
//          printf("Patient with this id %s already exists\n",recordID);
//          deletePatient(patient);
//       }
//    }
// }

// void recordPatientExit(HashTable* hashtable,char* recordID,char* exitDate)
// {
//    for(int i=0;i<hashtable->size;i++)
//    {
//       if((hashtable->hashbucket[i]).bucketarray->string == NULL)
//          continue;
//       else
//       {
//          if( (compare_dates( (hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->entryDate,exitDate)) == 1) // 1 gia date1>date2 / -1 gia date1<date2 / 0 gia date1=date2 / -1 gia -
//          {
//             printf("Wrong dates \n");
//             return;
//          }
//          if(strcmp(recordID,(hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->recordID)==0)
//          {
//             printf("ID's %s exit date was %s\n",(hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->recordID,(hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->exitDate);
//             strcpy((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->exitDate,exitDate);
//             printf("Now its %s\n",(hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->exitDate);
//          }
//          else
//             continue;
//       }
//    }
// }

// void numCurrentPatients(HashTable* hashtable,char* diseaseID)
// {
//    int entries=0;
   
//    for(int i=0; i<hashtable->size;i++)
//    {
//       if(diseaseID==NULL)
//       {
//          if((hashtable->hashbucket[i]).bucketarray->string == NULL)
//             continue;
//          else
//          {
//             //printf("%s EDW\n",(hashtable->hashbucket[i]).bucketarray->string );
//             // if( strcmp((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->exitDate,"-") == 0 )
//             // {
//             //    entries++;
//             // }
//             entries=searchTreeExit((hashtable->hashbucket[i]).bucketarray->tree->root,"-");
//          }
//       }
//       else
//       {
//          if((hashtable->hashbucket[i]).bucketarray->string == NULL)
//             continue;
//          else
//          {
//             if( strcmp((hashtable->hashbucket[i]).bucketarray->string,diseaseID) == 0 )
//                if( strcmp((hashtable->hashbucket[i]).bucketarray->tree->root->treepatient->exitDate,"-") == 0 )
//                   entries=searchTreeExit((hashtable->hashbucket[i]).bucketarray->tree->root,"-");
//          }
//       }
//       if(diseaseID!=NULL)
//       {
//          printf("%s %d \n",diseaseID,entries);
//          return;
//       }
//       else
//          printf("%s %d \n", (hashtable->hashbucket[i]).bucketarray->string,entries);
      
//       entries=0;
   
//    }
   

   
// }