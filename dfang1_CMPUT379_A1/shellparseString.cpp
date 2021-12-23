#include "shell379.h"
int parseString(string input){
    string inFile;
    string outFile;
    if (input.length() > LINE_LENGTH) {
        cout << "Command line cannot be bigger than" << LINE_LENGTH << " characters\n";
    }
    else if (input.length() !=0) {
        string arg;
         tms endCPU;
        clock_t endTime = times(&endCPU);
        vector<string> args;
        // split string
        stringstream ss(input);
        while(getline(ss, arg, ' ')){
            args.push_back(arg);
        }
        if (args.size() > MAX_ARGS){
            cout << "Command line cannot be bigger than "<< MAX_ARGS << " arguments\n";
        }
        else{
            bool max = false;
            for (auto i = args.begin(); i != args.end(); i++){
                if ((*i).size() >MAX_LENGTH){
                    max = true;
                }
            }
            if (max){
                cout << "One or more argument is bigger than " << MAX_LENGTH << "characters\n";
            }
            else{
                if (args[0].compare("exit") == 0){
                    if (int(args.size()) == 1){
                        //exit
                        running = false;
                        // this is not complete
                        exitProcess();
                    }
                    else{
                        cout << "exit has no arguments\n";
                    }
                }
                else if(args[0].compare("jobs") == 0){
                    if (args.size() == 1){
                        //jobs
                       jobProcess();
                    }
                    else{
                        cout << "jobs has no arguments\n";
                    }
                }
                else if(args[0].compare("kill") == 0){
                    if (args.size() == 2){
                        //kill one of the processes
                        killProcess(stoi(args[1]));
                    }
                    else{
                        cout << "kill can only have 1 argument\n";
                    }
                    running = true;
                }
                else if(args[0].compare("resume") == 0){
                    if (args.size() == 2){
                        //resume
                        resumeProcess(stoi(args[1]));
                        running = true;
                        //cout << "resume";
                    }
                    else{
                        cout << "resume can only have 1 argument\n";
                    }
                    running = true;
                }
                else if(args[0].compare("sleep") == 0){
                    if (args.size() == 2){
                        //sleep the shell
                        sleepProcess(stoi(args[1]));
                        running = true;
                        //cout << "sleep";
                    }
                    else{
                        cout << "sleep can only have 1 argument\n";
                    }
                }
                else if(args[0].compare("suspend") == 0){
                    if (args.size() == 2){
                        //suspend one of the process
                        suspendProcess(stoi(args[1]));
                        running = true;
                        // cout << "suspend";
                    }
                    else{
                        cout << "suspend can only have 1 argument\n";
                    }
                }
                else if(args[0].compare("wait") == 0){
                    if (args.size() == 2){
                        //wait
                        waitProcess(stoi(args[1]));
                        running = true;
                        // cout << "wait";
                    }
                    else{
                        cout << "wait can only have 1 argument\n";
                    }
                }
                else{
                    pid_t pid = fork();

                    if (pid < 0) {
                        perror("fork problem");
                        _exit(0);
                    }
                    else if (pid == 0){
                        
                        //Child process
                        if (args.back() == "&"){
                            args.pop_back();
                        }
                        
                        //cout << "child process\n";
                        char * inputcommand[args.size()+1];
                        for (uint i = 0; i < args.size(); i++){
                            inputcommand[i] = (char *)&args[i][0];
                            if(args[i][0] == '<'){
                                inFile = args[i].substr(1, args[i].size());
                                int ffd;
                                if((ffd = open(inFile.c_str(), O_RDONLY)) < 0)
                                    perror("open failed!");
                                dup2(ffd, STDIN_FILENO);    // stdin = ffd
                                close(ffd);                 // stdin is still open 
                                args.erase(args.begin()+ i);
                                
                            }
                            
                            if(args[i][0] == '>'){
                                outFile = args[i].substr(1, args[i].size());
                                int ffd;
                                if((ffd = open(outFile.c_str(),  O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR )) < 0)
                                    perror("open failed!");
                                dup2(ffd, STDOUT_FILENO);    // stdin = ffd
                                close(ffd);                 // stdin is still open 
                                args.erase(args.begin()+ i);
                            }
                        }
                        // cout << inFile << endl;
                        inputcommand[args.size()] = NULL;
                        // for (uint i = 0; i < args.size()+1; i++){
                           // cout << inputcommand[i] << "\n";
                        // }   
                        
                        if (execvp(inputcommand[0], inputcommand ) <0 ){
                            perror( "EXEC Problem");
                        }
                        running = false;
						_exit(0);
                    }
                    else{
                        //parent process
                        if (args.back() != "&"){
                            wait(NULL);
                        }
                        
                        else{
                            process newProcess = {.pid = pid,
                                                .spawntime = endTime,
                                                .cmd = input,
                                                .isSuspend = false};
                            // add to table
                            processTable.push_back(newProcess);
                        }
                        running = true;
                    }
                    // cout << "<cmd><arg>*\n";
                }
            }
        }
    }
    return 0;
}