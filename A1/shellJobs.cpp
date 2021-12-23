#include "shell379.h"
void sleepProcess(int time){
    sleep(time);
}
void updateTable(int signal){
    while(waitpid(-1, 0, WNOHANG) > 0) {};
    if (processTable.size() != 0){
        for (uint i = 0; i<processTable.size(); i++){
            if (kill(processTable[i].pid,0) != 0){
                kill(processTable[i].pid,SIGKILL);
                processTable.erase(processTable.begin()+i);
                
                completed ++;
            }
        }
    }
}
void jobProcess(){
    
	running = true;
    int active = 0;
    tms endCPU;
    times(&endCPU);
	printf("Running processes: ");
    // check if the table is empty
    if (processTable.size() != 0){
        // check for active processes
        for (uint i = 0; i < processTable.size(); i++ ){
            if (!processTable[i].isSuspend){
                active ++;
            }
        }
        // print table of processes
        cout << "\n #       PID S SEC COMMAND";
        
        for(uint i = 0; i < processTable.size(); i++){
            char P_running;
            if (!processTable[i].isSuspend){
                P_running = 'R';
            }
            else{
                P_running = 'S';
            }
            
            string command = "ps -o times -p " + to_string(processTable[i].pid);

            FILE *fp;
            int runtime;
            int LINESIZE = 20;
            char buf[LINESIZE];
            fp = popen(command.c_str(), "r");
            while(fgets(buf, LINESIZE, fp) != NULL)
                runtime = atoi(buf);
            pclose(fp);
            printf("\n%2i: %8d %c %3d %s", i, processTable[i].pid, P_running, runtime, processTable[i].cmd.c_str());
            // printf("\n%2i: %8d %c %s", i, processTable[i].pid, P_running, processTable[i].cmd.c_str());
        }
    }
    printf("\nProcesses = %6d active", active);
    // printf("\nCompleted processes: %d", completed);
    printf("\nCompleted processes:");
    printf("\nUser time = %6ld seconds", (endCPU.tms_cutime)/ sysconf(_SC_CLK_TCK));
    printf("\nSys  time = %6ld seconds\n", (endCPU.tms_cstime)/ sysconf(_SC_CLK_TCK));
}

void killProcess(int pid){
    running = true;
    // kill the process
    //remove it in the vector
    if(kill(pid, SIGKILL) == 0){
        printf("Killed pid: %d\n", pid);
    }
    else{
        printf("pid %d does not exist\n", pid);
    }
}
void exitProcess(){
    cout <<endl;
	for (uint i = 0; i < processTable.size(); i++){
		// kill each process
        
		kill(processTable[i].pid, SIGKILL);
        printf("Killed pid: %d\n", processTable[i].pid);
	}
    tms endCPU;
    times(&endCPU);
    printf("Resources used:");
    printf("\nUser time = %6ld seconds", (endCPU.tms_cutime)/ sysconf(_SC_CLK_TCK));
    printf("\nSys  time = %6ld seconds\n", (endCPU.tms_cstime)/ sysconf(_SC_CLK_TCK));
    running = false;
}
void callexit(int signal){
    exitProcess();
    _exit(0);
}
void resumeProcess(int pid){
    running = true;
    if (kill(pid, SIGCONT) == 0){
        for (uint i = 0; i<processTable.size(); i++){
            if (processTable[i].pid == pid){
                if (processTable[i].isSuspend == true){
                    processTable[i].isSuspend = false;
                    // change the process to resume
            
                    printf("Resumed pid: %d\n", pid);
                    }
                else{
                    cout << "pid " << pid <<" is already running\n";
                }
                break;
            }
        }
    }
    else{
         cout << "The pid "<< pid << " does not exist\n";
    }
 
}
void suspendProcess(int pid){
    running = true;
    if (kill(pid, SIGSTOP) == 0){
        for (uint i = 0; i<processTable.size(); i++){
            if (processTable[i].pid == pid){
                if (processTable[i].isSuspend == false){
                    processTable[i].isSuspend = true;
                    // change the process to resume
            
                    printf("Suspended pid: %d\n", pid);
                    }
                else{
                    cout << "pid " << pid <<" is already suspended\n";
                }
                break;
            }
        }
    }
    else{
         cout << "The pid "<< pid << " does not exist\n";
    }
 
}
void waitProcess(pid_t pid){

    running = true;
    kill(pid,SIGCONT);
    waitpid(pid,0,0);
}