// include all important libraries
// Pipe() and dup2() code function referenced from sirs code in gitlab (https://git.uwaterloo.ca/ece650-f23/cpp)
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// The main function
int main (int argc, char **argv) {
    vector <int> cid;
    int rgentoa1[2];
    int a1toa2[2];
    
    // creating two pipes to facilitate the transfer from rgen to a1 and from a1 to a2
    int a= pipe(rgentoa1);
    int b = pipe(a1toa2);

    // checking if pipe creation is failed or not 
    if(a == -1 || b == -1){
        cerr << "Error: pipe failed to create" << endl;

    }

    //forking first process
    int id_1 = fork();
    if(id_1 == -1){
        cerr << "Error: fork 1 failed to create"<< endl;
    }
    else if(id_1 == 0){
        // redirecting the output of first to stdout
        dup2(rgentoa1[1], STDOUT_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);

        // executing rgen
        argv[0] = (char *)"rgen";
        //passing the required argumenents
        execv("rgen",argv);
        return 1;

    }
    cid.push_back(id_1);

    // forking second process
    int id_2 = fork();
    if(id_2 == -1){
        cerr << "Error: fork 1 failed to create"<< endl;
    }
    else if(id_2 == 0){
        // reading the input from stdin
        dup2(rgentoa1[0], STDIN_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);

        // redirecting the output to stdout
        dup2(a1toa2[1], STDOUT_FILENO);
        close(a1toa2[0]);
        close(a1toa2[1]);
        // executing the python code of a1
        argv[0] = (char *)"python3";
	    argv[1] = (char *)"ece650-a1.py";
	    argv[2] = nullptr;
        // passing the required arguments
        execvp("python3",argv);
        return 1;
    }
    cid.push_back(id_2);
    
    // forking the third process
    int id_3 = fork();
    if(id_3 == -1){
        cerr << "Error: fork 1 failed to create"<< endl;
    }
    else if(id_3 == 0){
        // redirecting the input to stdin
        dup2(a1toa2[0], STDIN_FILENO);
        close(a1toa2[1]);
        close(a1toa2[0]);
        // executing the c++ code of a2
        argv[0] = (char*)"ece650-a2";
	    argv[1] = nullptr;
        // passing the required arguments
        execv("ece650-a2",argv);
        return 1;
    }
     cid.push_back(id_3);


     dup2(a1toa2[1],STDOUT_FILENO);
         close(a1toa2[0]);
        close(a1toa2[1]);
        // taking the s command from the user
    int id_4 = fork();
     if(id_4 == -1){
        cerr << "Error: fork 1 failed to create"<< endl;
    }
    else if(id_4 == 0){

        while (!cin.eof()) {
        string line;
        getline(cin, line);
        cout << line << endl;

        }}

        
         cid.push_back(id_4);



   int status;
waitpid(id_1, &status, 0);

// Terminate other processes
for (int k : cid) {
    kill(k, SIGTERM);
}


    

return 0;

}


