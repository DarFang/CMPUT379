
#include "shell379.h"
bool running = true;
int completed = 0;
tms startCPU;
clock_t startTime = times(&startCPU);
vector <process> processTable;
int main(){
    signal (SIGCHLD, updateTable);
    signal (SIGINT, callexit);
    string input;
    vector<process> processTable;
    // getrusage(RUSAGE_SELF, &usage);
    // startUser = usage.ru_stime;
    // startSystem = usage.ru_stime;
    
    // cout << startSystem.tv_usec << startUser.tv_usec <<endl;
    while(running){
        cout << "SHELL379: ";
        getline(cin,input);
        parseString(input);
    }
    

  return 0;
}