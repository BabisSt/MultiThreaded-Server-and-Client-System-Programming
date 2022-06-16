#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h>
#include <arpa/inet.h> //gia byte order
#include <netdb.h>
#include <sys/socket.h> // sockets
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h> //internter addresses
#include <unistd.h> // fork
#include <ctype.h> //toupper
#include <signal.h> //signal
#include <pthread.h>
#include <sys/select.h>

#include "../HEADERS/server.h"
#include "../HEADERS/queue.h"
#include "../../MASTER/HEADERS/pipe.h"
#include "../../MASTER/HEADERS/stats.h"

#define myperror(s,e) fprintf(stderr,"%s: %s\n",s , strerror(e))

void startThread(void *thread_f,int numOfThreads,void*);
int start_server(uint16_t port);

struct sockaddr_in server;
int counter =0;
List* list = NULL;
pthread_t *thread_pool= NULL;

pthread_mutex_t mutexThread = PTHREAD_MUTEX_INITIALIZER;//na dw ligo auta
pthread_mutex_t mutexSock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutexWork = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;    


int main (int argc, char **argv)
{

    fd_set desc,ready;
    uint16_t query_port =atoi(argv[2]);
    uint16_t stat_port = atoi(argv[4]);
    int numOfThreads = atoi(argv[6]);
    int bufferSize = atoi(argv[8]);
    struct sockaddr_in worker,client;

    struct sockaddr *workerptr= (struct sockaddr *)&worker;
    struct sockaddr *clientptr= (struct sockaddr *)&client;

    socklen_t workerlen = 0;
    socklen_t clientlen = 0;
    
    int newquery,newstat;
    int query_sock,stat_sock,maxfd = 0;

    int SockArray[100];
    for(int i=0; i < 100;i++)
    {
        SockArray[i] = 0;
    }    
    
    //DHMIOURGIA LISTS-QUEUE
    if((list = initList(bufferSize)) == NULL)
    {
        perror_exit("initList");
    }

    //MALLOC TA THREADS
    if ((thread_pool = malloc(numOfThreads * sizeof(pthread_t))) == NULL)
    {
        perror_exit("malloc");
    }
    
    //DHMIOYRGIA THREADS ME THREAD_FUN
    for (int i = 0; i < numOfThreads; i++)       
    {
        if(pthread_create(&thread_pool[i],NULL,child_server,NULL) != 0)
            perror_exit("pthread_create");
    }
    

    //KSEKINHMA SERVER GIA QUERIES KAI STATS
    if((query_sock = start_server(query_port)) < 0)
    {
        perror_exit("failed to start server");
    }
    if((stat_sock = start_server(stat_port)) < 0)
    {
        perror_exit("failed to start server");
    }


    if(query_sock > stat_sock)
        maxfd = query_sock+1;
    else
        maxfd = stat_sock+1; 

    FD_ZERO(&desc);
    FD_SET(stat_sock,&desc);
    FD_SET(query_sock,&desc);
   
    while (1)
    {
        ready = desc;
        if(pselect(maxfd ,&ready,NULL,NULL,NULL,NULL) < 0)//mporei na einai o ready
        {
            perror_exit("pselect");
        }
        else
        {
            for (int i = 0; i < maxfd; i++)   //kanoume loop mesa stou fd pou exoyme dwsei sthn select
            { 
                if(FD_ISSET(i,&ready))      //an o fd mesa sto set tou ready einai etoimos mpes
                {
                    if(i == query_sock)     //dexomaste sundesh
                    {
                        printf("\n");
                        if((newquery = accept(query_sock,clientptr,&clientlen)) < 0 )//
                            perror_exit("accept");
                        
                        printf("Accepted Connection\n");
                        if(maxfd > newquery)
                            maxfd = maxfd+1;
                        else
                            maxfd = newquery+1;
                        FD_SET(newquery,&desc);           //bazoume to newquery mesa sto set
                        
                        pthread_mutex_lock(&mutexSock);
                        SockArray[newquery] = 2;
                        pthread_mutex_unlock(&mutexSock);

                    }
                    else if (i== stat_sock) // stats / workers/1
                    {
                        printf("\n");
                        if((newstat = accept(stat_sock,workerptr,&workerlen)) < 0 )//
                            perror_exit("accept");
                            
                        printf("Accepted Connection\n");
                       // maxfd = max(maxfd,newstat) +1;
    
                        pthread_mutex_lock(&mutexSock);
                        if(maxfd > newstat)
                            maxfd = maxfd+1;
                        else
                            maxfd = newstat+1;
                        FD_SET(newstat,&desc);
                        SockArray[newstat] = 1;
                        counter ++;
                        pthread_mutex_unlock(&mutexSock);
                    }
                    else
                    {
                       
                        // memset(&worker,0, sizeof(struct sockaddr_in));
                        // socklen_t sa_len = sizeof(struct sockaddr_in);
                        // int tmpRealPort = getsockname(i, (struct sockaddr *)&worker,&sa_len);
                        // printf("Id %d Port %d\n",getpid(),tmpRealPort);
                        // char* toNtoa = strdup(inet_ntoa(worker.sin_addr));
                        // printf("Id %d toNtoa %s \n",getpid(),toNtoa);
                        // int ntoHs = ntohs(worker.sin_port);
                        // printf("Id %d ntoHs %d \n",getpid(),ntoHs);

                        int *pclient = malloc(sizeof(int));
                        *pclient = i;
                        pthread_mutex_lock(&mutexThread);
                        
                        appendList(list,pclient,SockArray[i]);
                        FD_CLR(i,&desc);                      //bgazoume apo to list to fd giati kaname thn douleia
                        
                        pthread_cond_signal(&condition_var);
                        pthread_mutex_unlock(&mutexThread);
                    }
                    
                }
            }
            
        }
    }
    pthread_cond_broadcast(&condition_var);
    for (int i = 0; i < numOfThreads; i++)
    {
        pthread_join(thread_pool[i], NULL);
    }
    

    
    close(query_sock);
    close(stat_sock);
    free(thread_pool);
 

return 0;
}



void* child_server(void* p_newsock)
{
    ListNode* tmp = NULL;
    while(1)
    {
        pthread_mutex_lock(&mutexThread);
        if((tmp = popList(list)) == NULL)
        {
            pthread_cond_wait(&condition_var,&mutexThread); 
            tmp = popList(list);
        
        }

        pthread_mutex_unlock(&mutexThread);
        if(tmp!=NULL)
        {
            if((tmp->job) == 1)     //stats/workers/1
            {
                int tmpSock = tmp->socket;
                free(tmp->port);
                free(tmp);
                char buf[256] = {0};
                char* readerWork = NULL;
                while(1)
                {
                    char array[100];
                    char* readerStat = RecieveProtocol(tmpSock,array,50);
                    if(strcmp(readerStat, "END") == 0)   //END
                    {
                        printf("\nStats taken by Server\n");
                        free(readerStat);
                        break;
                    }

                   // concatPrint(readerStat);
                    free(readerStat);
                }

                //DIABAZOUME TOUS WORKERS
                readerWork = RecieveProtocol(tmpSock,buf,50);
                printf("Number of Workers %s\n",readerWork);
                char* numOfWorkers = strdup(readerWork);
                free(readerWork);
                readerWork = NULL;

                //PORT APO WORKERS
                readerWork = RecieveProtocol(tmpSock,buf,50);//DEn leITOYREI
                printf("Port of worker %s\n",readerWork);
                char* WorkerPort = strdup(readerWork);
                free(readerWork);
                readerWork = NULL;

                //IP APO WORKERS
                readerWork = RecieveProtocol(tmpSock,buf,50);
                printf("IP %s \n",readerWork);
                char* WorkerIP = strdup(readerWork);
                free(readerWork);

                // pthread_mutex_lock(&mutexWork);
                // ServerWorker* serverworker;
                // serverworker->Workernum = atoi(numOfWorkers);
                // serverworker->WorkerPort = atoi(WorkerPort);
                // serverworker->WorkerIP = WorkerIP;
                
                // // printf("print from worker %d %d %s \n",serverworker->Workernum,serverworker->WorkerPort,serverworker->WorkerIP);

                //  pthread_mutex_unlock(&mutexWork);


                printf("Closing Connection\n");
                //free(reader);
                close(tmpSock);
            }
            else if((tmp->job) == 2)    //Client query
            {
                int tmpSock = tmp->socket;
                free(tmp->port);
                free(tmp);
                while (1)
                {
                    char array[100];
                    char* readerok = RecieveProtocol(tmpSock,array,100);
                    if(strcmp(readerok,"END") == 0)
                    {
                        printf("Queries taken by Server\n");
                        free(readerok);
                        break;
                    }
                    query_fun(readerok);
                    free(readerok);

                }
                //HAVE TO STOP WORKERS
                
                printf("Closing Connection\n");
                close(tmpSock);
                
            }
            
        }
    }
   
    return NULL;
}

void query_fun(char* inputString)
{

    char* query = strdup(inputString);
    char* instruct = strtok(inputString," ");

    if(strcmp(instruct,"/diseaseFrequency") == 0)
    {
        printf("%s Yparxei kwdikas gia auto \n",instruct);
    }
    else if(strcmp(instruct,"/topk-AgeRanges") == 0)
    {
        printf("I cant handle this query \n");
    }
    else if(strcmp(instruct,"/searchPatientRecord") == 0)
    {
        printf("%s \n",instruct);
    }
    else if(strcmp(instruct,"/numPatientAdmissions") == 0)
    {
       printf("I cant handle this query \n");
    }
    else if(strcmp(instruct,"/numPatientDischarges") == 0)
    {
        printf("I cant handle this query\n");
    }
    else
    {
        printf("This is not a query: %s \n",instruct);
    }
    
     free(query);
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


