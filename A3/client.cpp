#include "comm.h"
string outputString;
void printline0(int port, string ip, string host){
    char temp[1000];
    sprintf(temp,"Using port %d \nUsing server address %s \nHost %s \n", port, ip.c_str(), host.c_str());
    cout << temp;
    outputString.append(temp);
}
void printline1(string type, string type1, int n){
    char temp[1000];
    double time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    time = time/1000;
    sprintf(temp,"%.2f %s (%s%3d)\n", time, type.c_str(), type1.c_str(), n);
    cout << temp;
    outputString.append(temp);
}
void printline2(int n){
    char temp[1000];
    sprintf(temp,"Sleep %d units\n", n);
    cout << temp;
    outputString.append(temp);
}
void printline3(int n){
    char temp[1000];
    sprintf(temp,"Sent %d transactions \n", n);
    cout << temp;
    outputString.append(temp);
}
void writeFile(string filename){
    // write to file
    ofstream myfile(filename);
    if (myfile.is_open()){
        myfile << outputString;

        myfile.close();
    }
    else cout << "file failed " ;
}

int main(int argc, char** argv){
    string line;
    char id[1000];
    string done;
    int port = atoi(argv[1]);
    char* ip = argv[2];
    int tran = 0;
	int pid = getpid();
	//socket
	int connfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( connfd<0){
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in serv_addr;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

  
	gethostname(id, 1000);
	string filename = string(id)  + "." +  to_string(pid);
    printline0(port, ip, filename);
	if(connect(connfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
		perror("connect");
		return 1;
	}
    while ( getline(cin, line)){
        if (line[0] == 'T'){
            //tran
            tran++;
            
            printline1("Send", "T", stoi(line.substr(1, (int)line.size())));
            			
			line = filename + "-" + line; 
            char in[line.length()]; 
            strcpy(in, line.c_str());
            if(write(connfd, &in, line.length())<0){
                perror ("write");
                exit(-1);
            }
            int nread;
            char msg[100];
            if ((nread = read(connfd, &msg, sizeof(msg)-1))<0){
              perror("read");
              exit(-1);
            }
            msg[nread]='\0';
			line = string(msg);
            if (line[0] == 'D'){
                printline1("Recv", "D", stoi(line.substr(1, (int)line.size())));
            }
        }
        if (line[0] == 'S'){
            // sleep
            printline2(stoi(line.substr(1, (int)line.size())));
           
        }
       	
    }
    close(connfd); 
	// shutdown(connfd, how);
    printline3(tran);
    writeFile(filename);
}