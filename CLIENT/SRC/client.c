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


#include "../HEADERS/client.h"
#include "../../MASTER/HEADERS/pipe.h"
#include "../../SERVER/HEADERS/queue.h"

pthread_t *thread_pool= NULL;
List* list = NULL;

struct sockaddr_in server;
int servPort;
char* servIP =NULL;
int sockfd = 0;
char* buffer =  NULL;

pthread_mutex_t mutexThread = PTHREAD_MUTEX_INITIALIZER;//na dw ligo auta
pthread_mutex_t mutexSock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

int main (int argc, char **argv)
{

    if(argc !=9)
    {
        printf("Please give query file,num of threads ,port and ip\n");
        exit(1);
    }

    char* FileOfQueries = malloc(strlen(argv[2]) +1 );
    strcpy(FileOfQueries,argv[2]);
    int numOfThreads = atoi(argv[4]);
    servPort = atoi(argv[6]);
    servIP = malloc(strlen(argv[8])+1);
    strcpy(servIP,argv[8]);

    struct sockaddr_in server;
    struct sockaddr * serverptr = (struct sockaddr *)&server;
    struct hostent *rem;
    int sock,port;
    //int query_sock;

    size_t size;
    FILE* fp = fopen(FileOfQueries, "r");

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror_exit("socket");
    }

    if((rem = gethostbyname(servIP)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }

    port = servPort;
    server.sin_family = AF_INET;
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);

    if( connect(sock,serverptr, sizeof(server)) < 0)
    {
        perror_exit("connect");
    }
    printf("connect to %d port %d \n",servPort, port);

    int counter =0;
    char* buf= NULL;
    while( getline(&buf,&size,fp) >= 0)
    {
   
        pthread_mutex_lock(&mutexThread);
        if(SendProtocol(sock,buf,50) < 0)
        {
            perror_exit("write");
        }
        counter++;
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutexThread);
    }   
    

    free(buf);
    buf = NULL;
    SendProtocol(sock,"END",20);

    fclose(fp);

    // // pthread_cond_broadcast(&condition_var);
    // // SendProtocol(sock,"END",50);
    // // close(sock);

    // //DHMIOYRGIA QUEUE
    // if((list = initList(100)) == NULL)
    // {
    //     perror_exit("initList");
    // }

    // // //MALLOC TA THREADS
    // if ((thread_pool = malloc(counter * sizeof(pthread_t))) == NULL)
    // {
    //     perror_exit("malloc");
    // }

    // //DHMIOURGIA LISTS-QUEUE
    // if((buffer = malloc(counter* sizeof(char))) == NULL)
    // {
    //     perror_exit("malloc");
    // }

    // if((query_sock = start_server(servPort)) < 0)
    // {
    //     perror_exit("failed to start server");
    // }
    // for (int i = 0; i < counter; i++)
    // {
    //     buffer[i] = NULL;
    // }

    // FILE *fpp = fopen(FileOfQueries, "r"); 
    // size_t sizet;   

    // while (1)
    // {
        
    //     for (int i = 0; i < counter; i++)
    //     {
    //         //printf("eimai mesa\n");
    //         if(getline(&buffer,&sizet,fpp) == -1)
    //         {
    //             break;
    //         }
    //         else
    //         {
    //             //printf("buffer %s\n",buffer);
                
    //             if(pthread_create(&thread_pool[i],NULL,child_server,buffer) != 0)
    //                 perror_exit("pthread_create");
                
               
    //                     int *pclient = malloc(sizeof(int));
    //                     *pclient = i;
    //                     pthread_mutex_lock(&mutexThread);

    //                     appendList(list,pclient,1);
                        
    //                     pthread_cond_signal(&condition_var);
    //                     pthread_mutex_unlock(&mutexThread);
               
    //         }
    //     }
    //     pthread_cond_broadcast(&condition_var);
    //     for (int i = 0; i < counter; i++)
    //     {
    //         pthread_join(thread_pool[i],NULL);
    //     }
    // }


    free(buffer);
    free(thread_pool);
    //fclose(fpp);
    free(servIP);
    free(FileOfQueries);
    return 0;
}




int start_server(uint16_t port)
{
    int sockfd = 0;
    struct sockaddr *serverptr= (struct sockaddr *)&server;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");
    server.sin_family = AF_INET;
    inet_pton(AF_INET,servIP, &server.sin_addr); //apo string se ipv4
    server.sin_port = htons(servPort);

    if(connect(sockfd,serverptr,sizeof(server)) < 0)
    {
        perror_exit("connect");
    }

    return sockfd;

}

void perror_exit(char* message)
{
    perror(message);
    exit(EXIT_FAILURE);
}



void* child_server(void* p_newsock)
{
   
    char* query = NULL;
    query = (char*) p_newsock;
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
            if(SendProtocol(sockfd,query,50) == -1)
            {
                perror_exit("sendprotocol");
            }
            if((buffer = RecieveProtocol(sockfd,query,50)) == NULL)
            {
                perror_exit("recieveprotocol");
            }
        //printf("query %s %s\n",query,buffer2);
        }
    
    }
    return NULL;
}
