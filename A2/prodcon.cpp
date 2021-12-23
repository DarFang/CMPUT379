#include "prodcon.h"
#include "TranSleep.cpp"
queue<string> test;
chrono::system_clock::time_point start;
chrono::system_clock::time_point current;
pthread_cond_t CONS;
pthread_cond_t FINISHED;
pthread_cond_t PROD;
pthread_mutex_t QUEUE;
pthread_mutex_t OUTPUT;
string fileName;
string line;
int threads;
string outputString;
int ask = 0;
int work = 0;
int completed = 0;
int Sleepn = 0;
int recieved = 0;
bool working = true;
char temp[50];
int *threadWork;

void *worker(void *arg){
    // thread
    int ID = *((int*)arg );
    while(working){
        printline2(ID, "Ask");
        ask++;
        // tests if queue is empty
        while(test.empty()){
            if (!working and test.empty()){
                // end thread if no more
                return NULL;
            }
            else{
                // lock thread
                pthread_mutex_lock(&QUEUE);
                pthread_cond_wait(&CONS, &QUEUE);
                pthread_mutex_unlock(&QUEUE);
            }
        }
        bool success = false;
        int n = 0;
        pthread_mutex_lock(&QUEUE);
        // check if queue is empty
        if (!test.empty()){
            string work = test.front();
            test.pop();
            pthread_mutex_unlock(&QUEUE);
            n = stoi(work.substr(1, (int)work.size()));
            success = true;
        }
        else{
            pthread_mutex_unlock(&QUEUE);
        }
        if (success){
            recieved ++;
            printline1(ID, (int)test.size(), "Receive", n);
            if (!((int)test.size()+1 < 2 * threads)){
                pthread_cond_signal(&PROD);
            }
            Trans(n);
            completed ++;
            threadWork[ID-1] ++;
            printline1(ID, (int)test.size(), "Complete", n);
        }
        // exit if no more
        if (!working and test.empty()){
            return NULL;
        }
    }
    return NULL;
}
int main(int argc, char** argv){
    // main, calls in threads, then becomes producer
    start = chrono::system_clock::now();
    if (argc == 2){
            fileName = "prodcon.log";
        }
        else if (argc == 3){
            fileName.append("prodcon."). append(argv[2]).append(".log");
            
        }
    threads = atoi(argv[1]);
    pthread_t c[threads];
    // thread initialization
    int id [threads];
    threadWork = new int[threads];
    for (int number = 0; number < threads; number ++){
        id[number] = number+1;
    }
    for (int number = 0; number < threads; number++){
        
        pthread_create(&c[number], NULL, worker, &id[number]);
    }    
    // producer role
    while ( getline(cin, line)){
        int n = stoi(line.substr(1, (int)line.size()));
        if (line[0] == 'T'){
            // trans
            if (!((int)test.size() < 2 * threads)){
                // check if size of queue is double than thread spawn count
                pthread_cond_wait(&PROD, &QUEUE);
            }
            pthread_mutex_lock(&QUEUE);
            test.push(line);
            pthread_mutex_unlock(&QUEUE);
            work ++;
            printline1(0, (int)test.size(), "Work", n);
            pthread_cond_signal(&CONS);
            
        }
        if (line[0] == 'S'){
            // sleep
            Sleepn++;
            printline2(0, "Sleep");
            Sleep(n);
        }
    }
    // flush out every thread to finish
    while (!test.empty()){
        pthread_cond_signal(&CONS);
    }
    working = false;
    for (int number = 0; number < threads; number++){
        pthread_cond_signal(&CONS);
    }
   for (int number = 0; number < threads; number++){
        pthread_join(c[number], NULL);
    }
    // summary print
    printSummary();
    // output
    writeFile();
  return 0;
}