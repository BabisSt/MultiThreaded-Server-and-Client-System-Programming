#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <arpa/inet.h> //gia byte order
#include <netdb.h>
#include <sys/socket.h> // sockets
#include <netinet/in.h> //internter addresses
#include <ctype.h> //toupper
#include <signal.h> //signal
#include <pthread.h>
#include <sys/select.h>

#include "../HEADERS/worker.h"
#include "../HEADERS/pipe.h"
#include "../HEADERS/list.h"
#include "../HEADERS/stats.h"
#include "../HEADERS/hash.h"

struct sockaddr_in server;

ListNodeWorker* initArray(int size)
{
    ListNodeWorker* array = malloc(size*sizeof(ListNodeWorker));
    for(int i=0;i <size;i++)
    {
        array[i].countries = NULL;
        array[i].fd_read = -1;
        array[i].fd_write = -1;
    }
    return array;
}

int RunWorker(char* dir,int buffersize,int pipe_read,int pipe_write,int numofworkers)
{

    char arr[buffersize];
    struct dirent* dent;
    char* readerServer, *reader;
    char* IP;
    uint16_t Port; 

    //PAIRNOUME PRWTA IP KAI PORT
    readerServer = RecieveProtocol(pipe_read,arr,50);
    IP = strdup(readerServer);
    free(readerServer);

    readerServer= RecieveProtocol(pipe_read,arr,50);
    Port = atoi(readerServer);
    free(readerServer);

    
   // String_of_stats* chars = initOfChar();
    CountryList* countries = initCountry();
    List* PatientList= initList();
    List* PatientEXITList = initList();

    
     int sock;
    //char buf[256];
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct hostent *rem;

    if((sock = socket(AF_INET,SOCK_STREAM, 0 )) < 0)
    {
        perror_exit("socket");
    }

    if((rem = gethostbyname(IP))== NULL)
    {
        perror_exit("gethost");
    }

    server.sin_family= AF_INET;
    memcpy(&server.sin_addr,rem->h_addr,rem->h_length);
    server.sin_port = htons(Port);

    if(connect(sock,serverptr,sizeof(server))< 0)   //STAMATAEI EDW
    {
        perror_exit("connect");
    }

    printf("Connection with IP %s Port %d \n",IP,Port);

    //PAIRNOUME XWRES
    while(1)
    {
        reader = RecieveProtocol(pipe_read,arr,buffersize);
        if(strcmp(reader,"END") == 0)
        {
            free(reader);
            break;
        }
        appendListCountry(countries,reader);
        free(reader);
    }

    CountryList* tmp = countries;
    List_of_Stats* charForDateFile = initListOfStats();

    while(tmp->first!=NULL)
    {
        char* countryDir = malloc((strlen(dir)+strlen(tmp->first->data)+2)*sizeof(char));
        strcpy(countryDir,dir);
        strcat(countryDir, "/");
        strcat(countryDir,tmp->first->data);

        DIR* dirr = opendir(countryDir);
        
        if(ENOENT == errno)
        {
            fprintf(stderr,"DIR not exists\n");
            return -1;
        }
        else if(!dirr)
        {
            fprintf(stderr,"opendir\n");
            return -1;
        }
        
        CountryList* country_list_date = initCountry();
        while( (dent = readdir(dirr))!=NULL)
        {
            if(strcmp(dent->d_name, ".")!=0 && strcmp(dent->d_name, "..")!=0)
            {
                char* countryDateFile = malloc((strlen(countryDir)+strlen(dent->d_name)+2)*sizeof(char));
                strcpy(countryDateFile,countryDir);
                strcat(countryDateFile, "/");
                strcat(countryDateFile,dent->d_name);
                appendListCountry(country_list_date,countryDateFile);

                FILE* fp = fopen(countryDateFile, "r");
                if(fp==NULL)
                {
                    perror("open file");
                }

                char* buffer =NULL;
                size_t size = 0;
                while(getline(&buffer,&size,fp)>=0)
                {
                    PutPatientsIntoStructs(buffer,PatientList,dent->d_name,tmp->first->data,charForDateFile,PatientEXITList );
                    free(buffer);
                    buffer = NULL;
                }
                
                Node_of_stats* tmpStats = charForDateFile->first;

                while(tmpStats!=NULL)
                {
                    char* stringStats = concatStats(tmpStats->item);
                    SendProtocol(sock,stringStats,50);
                    free(stringStats);
                    tmpStats = tmpStats->next;
                }

                emptyStats(charForDateFile);
                //charForDateFile->first = NULL;
                free(buffer);
                fclose(fp);
                free(countryDateFile);
            }
        }
        freeListCountry(country_list_date);
        free(country_list_date);
    
        free(countryDir);
        closedir(dirr);
        tmp->first = tmp->first->next;
    }
    //free(tmp);
    SendProtocol(sock,"END",buffersize);

    ListNode* looking = PatientEXITList->first;
    while(looking!=NULL)
    {
        if(updateExitDate(PatientList,looking->data)==false)
        {
            //printf("ERROR in exit date\n");
            //osoi exoun lathos sto exit date
        }
        looking= looking->next;
    }

   // printf("EXW ftasei edw\n");
    //SUNDESH ME TON SERVER

    //STELNOUME ARITHMO WORKERS
    char* tmpWorkers = malloc(20);
    sprintf(tmpWorkers, "%d", numofworkers);
    SendProtocol(sock,tmpWorkers,50);
    //free(tmpWorkers);

    //STELNOUME PORT
    char* portWorkers = malloc(20);
    sprintf(portWorkers, "%d", getpid() );
    SendProtocol(sock,portWorkers,50);
    //free(portWorkers);

    portWorkers = malloc(20);
    sprintf(portWorkers, "%d", getpid());
    int portOfServer = atoi(portWorkers);
    free(portWorkers);
    
    //STELNOUME IP
    SendProtocol(sock,IP,50);
    close(sock);


    int diseaseHTentries = 10;
    int countryHTentries = 10;
    int bucketSize = 16;

    HashTable *countryTable,*diseaseTable;
    if( (countryTable = initTable(countryHTentries,bucketSize)) == NULL)
    {
        printf("ERROR on Hashtable\n");
        return false;
    }

    InsertListToTable(PatientList, countryTable,1);

    if((diseaseTable = initTable(diseaseHTentries,bucketSize)) == NULL)
    {
        printf("ERROR Hashtable\n");
        return false;
    }

    InsertListToTable(PatientList,diseaseTable,0);


    //SYNDESH WORKER ME SERVER
    int contoserver = start_server(portOfServer);
    int newCon;
    struct sockaddr_in worker;
    socklen_t workerlen = 0;
    struct sockaddr *workerPtr = (struct sockaddr*)&worker;
    if((newCon = accept(contoserver,workerPtr,&workerlen)) < 0)
    {
        perror_exit("accept worker");
    }

    DoTheQueries(newCon,diseaseTable,countryTable);

    close(newCon);
    close(contoserver);

    free(IP);
    emptyStats(charForDateFile);
    freeListCountry(countries);
    freeListPatient(PatientList);
    freeListPatient(PatientEXITList);
    free(countries);
    free(charForDateFile);
    deleteTable(countryTable);
    deleteTable(diseaseTable);


    return 0;
}

void appendListCountries(ListNodeWorker* head, char* country) 
{ 
    CountryListNode* new_node = malloc(sizeof(CountryListNode));
    new_node->data = malloc(sizeof(country)+1);
    strcpy(new_node->data,country);
    new_node->next = head->countries;
    head->countries = new_node;

    free(new_node);
} 

void emptyworkerNode(ListNodeWorker* workerNode)
{
    workerNode->fd_read= 0;
    workerNode->fd_write=0;
    ListNodeWorker* tmp = workerNode;
    while(tmp != NULL)
    {
        free(tmp->countries->data);
        tmp->countries = tmp->countries->next;
    }

    free(tmp->countries);
    free(tmp);
    free(workerNode);
}

int PutPatientsIntoStructs(char* line,List* PatientsList, char* date, char* country, List_of_Stats* charsForDateFile, List* PatientEXITList)
{

    char* tmp = strdup(line);
    char* tok = strtok(tmp, " ");
    tok = strtok(NULL, " ");

    Patient* patient = initPatient(line,date,country);
    //free(line);

    

    if(strcmp(tok, "ENTER")==0) //O astheneis exei eisagwgh
    {
        appendListPatient(PatientsList,patient);//balton se lista
        informCharList(charsForDateFile, patient);//enhmerwse ta stoixeia tou
    }
    else if(strcmp(tok, "EXIT")==0)
    {
        appendListPatient(PatientEXITList,patient);//balton sthn lista pou exou bgei
       
    }
    free(tmp);
    //free(tok);
    return 0;
}

void perror_exit(char* message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int start_server(uint16_t port)
{
    int sockfd = 0;
    struct sockaddr *serverptr= (struct sockaddr *)&server;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if(bind(sockfd, serverptr, sizeof(server)) < 0)
        perror_exit("bind");
    
    if(listen(sockfd, 5) < 0)
        perror_exit("listen");
    
    printf("Listening for connnections to port %d\n",port);

    

    return sockfd;

}

void DoTheQueries(int sock, HashTable* diseaseTable, HashTable* countyTable)
{
    while(1)
    {
        char arr[100];
        char* reader = RecieveProtocol(sock,arr,50);
        //if()
    }
}
