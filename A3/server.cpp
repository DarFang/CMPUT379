#include "comm.h"
string outputString;
vector <string> pids;
vector <int> tran;
double start;
void printline0(int port){
    char temp[1000];
    sprintf(temp,"Using port %d \n", port);
    cout << temp;
    outputString.append(temp);
}
void printline1(int job, int n, string pid){
    char temp[1000];
    double time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    time = time/1000;
    sprintf(temp,"%.2f #%3d (T%3d) from %s\n", time, job, n, pid.c_str());
    cout << temp;
    outputString.append(temp);
}
void printline2(int job, string pid){
    char temp[1000];
    double time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    time = time/1000;
    sprintf(temp,"%.2f #%3d (Done) from %s\n", time, job, pid.c_str());
    cout << temp;
    outputString.append(temp);
}
void printline3(int job){
    char temp[1000];
    outputString.append("SUMMARY\n");
    for (uint i = 0; i<pids.size(); i++){
        sprintf(temp,"%4d transactions from %s\n", tran[i], pids[i].c_str());
        cout << temp;
        outputString.append(temp);
    }
    double time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    time = time/1000;
    sprintf(temp,"%3.1f transactions/sec (%d/%.2f)\n", job/(time-start-30),job, time-start-30);
    cout << temp;
    outputString.append(temp);
}
void writeFile(){
    // write to file
    char id[1000];
    gethostname(id, 1000);
    int pid = getpid();
    string filename = string(id) + "." + to_string(pid) + ".server";
    ofstream myfile(filename);
    if (myfile.is_open()){
        myfile << outputString;

        myfile.close();
    }
    else cout << "file failed " ;
}
int main(int argc, char** argv){
    start = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    start = start /1000;
    struct timeval timeout;
	vector <string> readline;
    string line;
	int connfd;
    int port = atoi(argv[1]);
	int job = 0;
    int clientSocket [10];
    int sd, max_sd;
    for (int i = 0; i < 10; i++)  {  
        clientSocket[i] = 0;  
    }  
    
    fd_set readfds;
    socklen_t clnt_len;
	struct sockaddr clnt_addr;
    struct sockaddr_in serv_addr;
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    
    
	if (listenfd<0){
		perror("socket");
		exit(-1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	//bind

	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0){
		perror("bind");
		exit(-1);
	}
    //listen
	if(listen(listenfd,10)<0){
		perror("listen");
		exit(-1);
	}
    printline0(port);
    bool initial = true;
	while (true){
        FD_ZERO(&readfds);  
        FD_SET(listenfd, &readfds);  
        max_sd = listenfd;  
        for (int i = 0 ; i < 5 ; i++){  
            sd = clientSocket[i];  
            if(sd > 0)  
                FD_SET( sd , &readfds);  
            if(sd > max_sd)  
                max_sd = sd;  
        }
        timeout.tv_sec = 30; 
        timeout.tv_usec = 0; 
        int activity = select( max_sd+1 , &readfds , NULL , NULL , &timeout); 
        if(activity == 0 ){
            cout << "timeout" <<endl;
            printline3(job);
            writeFile();
            break;
        }
        if (FD_ISSET(listenfd, &readfds)) {
                if((connfd = accept(listenfd, &clnt_addr, &clnt_len))<0){
                    perror("accept");
                    exit(-1);
                }
               if (initial){
                     start = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
                     start = start /1000;
                     initial = false;
               }
                for (int i = 0; i < 10; i++)  {
                    if( clientSocket[i] == 0 ){  
                        clientSocket[i] = connfd;   
                        break;  
                    }  
                }  
                cout << "connected" << endl;
        }
        for (int i = 0; i < 10; i++) {
            sd =  clientSocket[i];
            if (FD_ISSET(sd, &readfds)) {
                char msg[1000];
                string pid;
                int nread;
                if ((nread = read(sd, &msg, sizeof(msg)-1))<0){
                    perror("read");
                    exit(-1);
                  }
                else if (nread == 0){
                    cout << "end connection" << endl;
                    close(sd);
                    clientSocket[i] = 0;
                }
                else{
                    msg[nread]='\0';
                    line = string(msg);
                    stringstream ss(line);
                    readline.clear();
                    while(getline(ss, line, '-')){
                        readline.push_back(line);
                    }
                    line = 	readline[1];
                    pid = readline[0];
                    int n = stoi(line.substr(1, (int)line.size()));
                    job++;
                    if (job == 1){
                        start = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
                        start = start /1000;
                    }
                    printline1(job, n, pid);
                    Trans(n);
                    line = "D" + to_string(job);
                    char in[line.length()]; 
                    strcpy(in, line.c_str());
                    printline2(job, pid);
                    if(write(sd, &in, sizeof(in))<0){
                        perror ("write");
                        exit(-1);
                        }
                    bool found = false;
                    for (uint i = 0; i<pids.size(); i++){
                        if (pids[i] == pid){
                            found =true;
                            tran[i] ++;
                        }
                    }
                    if (!found){
                       pids.push_back (pid);
                       tran.push_back(1);
                    }

                        
                    
                }
            }     
        }
    }
		return 0;
}
    
