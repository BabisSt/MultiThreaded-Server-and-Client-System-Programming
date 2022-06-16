#pragma once


typedef struct ServerWorker
{
    int Workernum;
    int WorkerPort;
    char* WorkerIP;

}ServerWorker;


void sigchld_handler (int );

void perror_exit(char*);

void* thread_fun(void*);

void* child_server(void*);

void query_fun(char*);

int stat_fun(int);