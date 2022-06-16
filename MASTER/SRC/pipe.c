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
#include <time.h>
#include "../HEADERS/pipe.h"


int OpenPipe(char* pipe_name,int flag) // flag = gia read h write
{
   if((mkfifo(pipe_name, 0666)) == -1)
   {
       perror("mkfifo");
   }
  

    int fd = open(pipe_name,flag);
    return fd;
}


int SendProtocol(int fd, char* buf, int buffersize)
{
    char epikefalida[12] = {0}; //stelnoume posa bytes tha steilei
    int stSize = sizeof(epikefalida);
    int bufLen = strlen(buf)+1;

    sprintf(epikefalida, "%u" , bufLen);

    for(int i=0; i<stSize/buffersize ; i++)
    {
        write(fd, epikefalida+(i*buffersize), buffersize);

    }

    if(stSize%buffersize!=0)
    {
        write(fd, epikefalida+(stSize-stSize%buffersize),stSize%buffersize);

    }

    for (int i = 0; i < bufLen/buffersize; i++)
    {
        write(fd, buf+(i*buffersize), buffersize);

    }
    
    if(bufLen%buffersize !=0)
    {
        write(fd, buf+(bufLen-bufLen%buffersize), bufLen%buffersize);
    }
    return 0;
}

char* RecieveProtocol(int fd, char* buf, int buffersize)
{
    char* return_buf= NULL;
    char epikefalida[12] = {0}; //mathenoume posa bytes tha diabasei
    int stSize = sizeof(epikefalida);
    int return_bufSize = 0;

    for(int i=0; i<stSize/buffersize; i++)
    {
        read(fd, buf, buffersize);
        memcpy(epikefalida+(i*buffersize),buf, buffersize);
    }

    if(stSize%buffersize!=0)
    {
        read(fd, buf, stSize%buffersize);
        memcpy(epikefalida+(stSize/buffersize)*buffersize, buf , stSize-(stSize/buffersize)*buffersize);

    }

    return_bufSize = atoi(epikefalida);

    if( (return_buf=malloc(return_bufSize))== NULL)
    {
        printf("malloc ERROR\n");
        return NULL;

    }

    for(int i=0; i<return_bufSize/buffersize; i++)
    {
        read(fd, buf, buffersize);
        memcpy(return_buf+(i*buffersize), buf, buffersize);
    }

    if(return_bufSize%buffersize!=0)
    {
        read(fd, buf, return_bufSize%buffersize);
        memcpy(return_buf+(return_bufSize/buffersize)*buffersize, buf , return_bufSize-(return_bufSize/buffersize)*buffersize);
    }

    return return_buf;
}