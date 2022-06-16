#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
#include <time.h>
#include <arpa/inet.h> //gia byte order
#include <netdb.h>
#include <sys/socket.h> // sockets
#include <netinet/in.h> //internter addresses
#include <ctype.h> //toupper
#include <pthread.h>
#include <sys/select.h>

#include "../HEADERS/country_list.h"
#include "../HEADERS/worker.h"
#include "../HEADERS/pipe.h"


int main(int argc, char **argv)
{
    int numWorkers = atoi(argv[2]);
    int bufferSize = atoi(argv[4]);
    char* servIP = malloc(strlen(argv[6])+1) ;
    strcpy(servIP,argv[6]);
    u_int16_t Port = atoi(argv[8]);
    char* input_dir = malloc(strlen(argv[10])+1);
    strcpy(input_dir,argv[10]);
    pid_t childpid;
    int dir_count = 0;

    CountryList* countriesList = initCountry();
    ListNodeWorker* array= initArray(numWorkers);
    
    struct dirent* dent;
    DIR* srcdir = opendir(input_dir);
    
    if (srcdir == NULL)
    {
        perror("opendir");
        return -1;
    }

    
    while((dent = readdir(srcdir)) != NULL)//anoigoume ton fakelo kai pairnoume tis xwres
    {
        struct stat st;

        if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0)
        {
            perror(dent->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            appendListCountry(countriesList,dent->d_name); //epistrefoume ena antigrafo ths listas sthn pragmatikh lista
            dir_count++;
            
        }
    }
    closedir(srcdir);
    //printListCountry(countriesList->first);


    //Kanoume fork ta paidia
    for(int i=0; i< numWorkers; i++)
    {
        
        childpid = fork();
        if(childpid == -1)
        {
            perror("Failed to fork");
            return -1;
        }

        if(childpid == 0)   //paidi
        {
            int fd_read,fd_write;
            char* read = malloc(strlen("./files/pipes_read")+13);
            sprintf(read, "./files/pipes_read%d", getpid());
            char* write = malloc(strlen("./files/pipes_write")+13);
            sprintf(write, "./files/pipes_write%d", getpid());
            
            
            if((fd_write = open(read,O_WRONLY)) == -1)
            {
                perror("open 2");
            }
            if((fd_read = open(write,O_RDONLY)) == -1)
            {
                perror("open 1");
                exit(-1);
            }
            RunWorker(input_dir,bufferSize,fd_read,fd_write,numWorkers); // bufferSize to 2
            //kanoume free oti exei desmeutei apo panw apo ton patera
            free(array);
            freeListCountry(countriesList);
            free(countriesList);
            free(input_dir);
            free(servIP);
            free(read);
            free(write);
            exit(-1);
        }
        else    //pateras
        {
            char* pipe_read = malloc(strlen("./files/pipes_read")+12);
            sprintf(pipe_read, "./files/pipes_read%d", childpid);
            mkfifo(pipe_read, 0666);
            
            
            char* pipe_write = malloc(strlen("./files/pipes_write")+12);
            sprintf(pipe_write, "./files/pipes_write%d", childpid);
            mkfifo(pipe_write, 0666);

            if((array[i].fd_read = open(pipe_read,O_RDONLY)) == -1)
            {
                perror("open");
            }
            if((array[i].fd_write = open(pipe_write, O_WRONLY)) == -1)
            {
                perror("open");
            }

            free(pipe_read);
            free(pipe_write);
        }
        
    }

    //STELNOUME PRWTA PORT KAI IP
    char* servPort = malloc(10);
    sprintf(servPort, "%d", Port);
    for (int i = 0; i < numWorkers; i++)
    {
        SendProtocol(array[i].fd_write,servIP, 50);
        SendProtocol(array[i].fd_write,servPort,50);
    }
    

    //STELNOUME XWRES
    CountryListNode* tmp = countriesList->first;
    int i=0;
    while( tmp != NULL )        //stelnoume tis xwres stous workers
    {
        SendProtocol(array[i].fd_write, tmp->data, bufferSize);
        i++;
        if(i == numWorkers)
        {  
            i = 0;
        }
        tmp = tmp->next;
    }

    for(int i=0;i<numWorkers;i++)
    {
        SendProtocol(array[i].fd_write,"END",bufferSize);
        
    }
    
    //PAIRNOUME STATS
    // for (int i = 0; i < numWorkers; i++)
    // {
    //     while(1)
    //     {
    //         char arr[bufferSize];
    //         char* reader = RecieveProtocol(array[i].fd_read, arr,bufferSize);
    //         if(strcmp(reader, "END")==0)
    //         {

    //             free(reader);
    //             break;
    //         }
            
    //         //ektupwsh stats
    //         concatPrint(reader);
    //         printf("proxwra\n");
    //         free(reader);
    //     }
    // }


    //perimenoume na teleiwsoun
    for (int i = 0; i < numWorkers; i++)
    {
        sleep(1);
    }

    for(int i=0;i< numWorkers; i++)
    {

        close(array[i].fd_write);
        close(array[i].fd_read);
    }


    free(tmp);
    freeListCountry(countriesList);
    free(countriesList);
    free(array);
    free(input_dir);
    free(servPort); 
    free(servIP);
    return 0;
}

