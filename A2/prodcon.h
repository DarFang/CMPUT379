#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <chrono>
#include <queue>
#include <pthread.h>
using namespace std;
extern chrono::system_clock::time_point start;
extern chrono::system_clock::time_point current;
extern pthread_cond_t CONS;
extern pthread_cond_t FINISHED;
extern pthread_cond_t PROD;
extern pthread_mutex_t QUEUE;
extern pthread_mutex_t OUTPUT;
extern string fileName;
extern string line;
extern int threads;
extern string outputString;
extern char temp[50];
extern int ask ;
extern int work;
extern int completed ;
extern int Sleepn ;
extern int recieved ;
extern bool working ;
extern int *threadWork;


void printline1(int ID, int Q, string type, int n);
void printline2(int ID, string type);
void printline3(string type, int val);
void printSummary();
void writeFile();
void *worker(void *arg);