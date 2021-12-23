#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <signal.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <fcntl.h>
using namespace std;
#define LINE_LENGTH 100    // Max # of characters in an input line 
#define MAX_ARGS 7   // Max number of arguments to a command 
#define MAX_LENGTH 20    // Max # of characters in an argument 
#define MAX_PT_ENTRIES 32    // Max entries in the Process Table

extern bool running;
extern int completed;
#ifndef PROCESS_H_
#define PROCESS_H_
class process{
    public:
        pid_t pid;
        clock_t spawntime;
        string cmd;
        bool isSuspend;
};
#endif

extern tms startCPU;
extern clock_t startTime;
extern vector <process> processTable;
void sleepProcess(int time);
void updateTable(int signal);
void jobProcess();
void killProcess(int pid);
void exitProcess();
void callexit(int signal);
void resumeProcess(int pid);
void suspendProcess(int pid);
void waitProcess(pid_t pid);


int parseString(string input);