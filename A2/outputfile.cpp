#include "prodcon.h"
void printline1(int ID, int Q, string type, int n){
    // one of the print formats types are complete, receive, work
    current = chrono::system_clock::now();
    chrono::duration<double> time = current - start;
    pthread_mutex_lock(&OUTPUT);
    sprintf (temp, "%.3f ID= %2d Q= %2d %-10s %d\n",time.count(), ID, Q, type.c_str(), n);
    // cout << temp;
    outputString.append(temp);
    pthread_mutex_unlock(&OUTPUT);
}
void printline2(int ID, string type){
    // another print format types are ask, sleep
    current = chrono::system_clock::now();
    chrono::duration<double> time = current - start;
    pthread_mutex_lock(&OUTPUT);

    sprintf (temp,"%.3f ID= %2d       %-10s\n",time.count(), ID, type.c_str());
    // cout << temp;
    outputString.append(temp);
    pthread_mutex_unlock(&OUTPUT);
}
void printline3(string type, int val){
    // print format for summary
    sprintf(temp, "    %-10s    %4d\n", type.c_str(), val);
    // cout << temp;
    outputString.append(temp);
}
void printSummary(){
    // prints the summary
    outputString.append("Summary:\n");
    printline3("Work", work);
    printline3("Ask", work);
    printline3("Receive", work);
    printline3("Complete", work);
    printline3("Sleep", work);
    for (int number = 0; number < threads; number++){
        string temp = "Thread  ";
        temp += to_string(number+1);
        printline3(temp, threadWork[number]);
    }
    chrono::duration<double> time = current - start;
    sprintf (temp,"Transactions per second: %.2f\n", work/time.count());
    outputString.append(temp);
}
void writeFile(){
    // write to file
    ofstream myfile(fileName);
    if (myfile.is_open()){
        myfile << outputString;

        myfile.close();
    }
    else cout << "file failed " ;
}