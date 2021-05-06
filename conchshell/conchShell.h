#include <iostream>
#include <fstream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <filesystem>
#include <vector>
#include <filesystem>

#include "lexer.h"
#include "tokens.h"



using namespace std;

class conchShell {
private:

public: 

    //Set the current shell greeting
    string greeting = "$ ";
    //Store the working command
    string command;


    //Set up and destruction of that mf shell
    void setUpShell();
    void exitShell();
    int breakOut = 1;
    //History conditions
    vector<string> history;

    //Redirect conditions
    int outputRedirect = 0;
    int inputRedirect = 0;
    char *inputRedirectFileName;
    char *outputRedirectFileName;

    //Run the Loop
    void loop();

    //Shell builtins
    int checkForBuiltIns(Token *tokens);
    void parse(Token *tokens);
    void conch_cd(string directory);

    //Functions to actually execute the system command
    void WaitFor(int pid);
    void execute_sys_command(char *cmd, char *argv[]);

    //Parse and execute the command
    void lexing(string c);
    
};




string readFileIn(string filename) {
    ifstream file;
    file.open(filename);
    string line;
    string stringedFile = "";
    while(getline(file, line)){
        stringedFile = stringedFile + line;
    }
    file.close();
    return stringedFile;
}

void conchShell::exitShell(){
    ofstream out;
    out.open(".history");
    for(int i = 0; i < history.size(); i++){
        out << history[i] << endl;
    }

}

void conchShell::setUpShell(){
    ifstream in;
    in.open(".history");
    string line;
    while(getline(in, line)){
        history.push_back(line);
    }
    in.close();

}

void conchShell::conch_cd(string directory){
    if (directory == "~"){
        chdir(getenv("HOME"));
    }
    else {
        chdir(directory.c_str());
    }
}

//The execute loop, this will run indefinitely while the shell is running
void conchShell::loop(){
    while(breakOut){
        cout << greeting;
        //Gets the entire line off of the command line
        getline(cin, command);
        
        if (command.empty()){
            continue;
        }
        lexing(command);
    }


    
}

int conchShell::checkForBuiltIns(Token *tokens){
    //if the first token is exit, we gotta exit

    if (tokens->type == TOKEN_EXIT){
        
        breakOut = 0;
        return 1;
    }

    //Check for the cd command, so we can change directory
    if (tokens->type == TOKEN_CD){
        history.push_back(tokens->identifier);
        string directory = "~";
        if (tokens->next != NULL){
            tokens = tokens->next;
            directory = tokens->identifier;
        }

        conch_cd(directory);
        return 1;
    }
    //Check for bang
    else if (tokens->type == TOKEN_BANG){

        char *bangString = (char *)malloc(strlen(tokens->identifier)+1);
        memset(bangString, 0, strlen(tokens->identifier)+1);
        strcpy(bangString, tokens->identifier);
        memmove(bangString, bangString+1, strlen(bangString));

        int back = atoi(bangString);
        if (back == 0){
            back = 1;
        }
        if (history.size() >= (1 + back)){
            string historic_command = history[history.size() - (1 + back)];
            lexing(historic_command);
        } 
        return 0;    
    }
    
    //Check for listing history
    else if(tokens->type == TOKEN_HISTORY){
        for(int i = 0; i < history.size(); i++){
            cout << history[i] << endl;
        }
        history.push_back(tokens->identifier);
        return 1;
        
    }
    else return 0;
}

//Parse the tokens as they come in and execute commands as necessary
void conchShell::lexing(string command){
    
    

    //convert the command from a C++ string to a c-style string
    char *command_cstring = &command[0];

    //Do some lexing
    Token *tokens= lex(command_cstring);

    //If we aren't a builtin, we are a system command
    parse(tokens);
}


void conchShell::parse(Token *tokens){

    //First we gotta check for builtins 
    if (checkForBuiltIns(tokens)){
        return;
    }
    //Add the command to history
    history.push_back(tokens->identifier);


    int outputTracker = 0;
    int inputTracker = 0;
    int fileOutput = 0;
    int fileInput = 0;

    vector<string>  inputTokens;
    while(tokens!=NULL)
    {
        //Check is file output redirect is present
        if (tokens->type == TOKEN_FILE_OUTPUT){
            fileOutput = outputTracker;
            
            outputRedirect = 1;
        }
        //Check is file input redirect is present
        if (tokens->type==TOKEN_FILE_INPUT){
            fileInput = inputTracker;
            inputRedirect = 1;
        }
        //Push the indentifer, the string itself, into the vector
        inputTokens.push_back(tokens->identifier);
        tokens=tokens->next;
        //Increment
        outputTracker++;
        inputTracker++;
    }

    
    int num_tokens;

    if (inputRedirect == 0 && outputRedirect == 0){
        num_tokens = inputTokens.size();
    }
    else if (inputRedirect == 1 && outputRedirect == 1){
        num_tokens = fileInput;

        inputRedirectFileName = &inputTokens[fileInput + 1][0];
        outputRedirectFileName = &inputTokens[fileOutput + 1][0]; 
    }
    else if (outputRedirect == 1){
        num_tokens = fileOutput;
        outputRedirectFileName = &inputTokens[fileOutput + 1][0];
    }
    else if (inputRedirect == 1){
        num_tokens = fileInput;
        inputRedirectFileName = &inputTokens[fileInput + 1][0]; 
    }

    char *cmd = &inputTokens[0][0];
    char *argv[num_tokens];

    for(int i = 0; i < num_tokens; i++) {
            argv[i] = &inputTokens[i][0];
    }

    //Terminate the argv for the execvp command
    argv[num_tokens] = NULL;
    execute_sys_command(cmd,argv);

}

void conchShell::WaitFor(int pid){
  int gotpid, status;
  while( (gotpid = wait(&status)) != pid);
}

void conchShell::execute_sys_command(char *cmd, char *argv[]){
    //Pipe up
    int pipefdout[2];
    int pipefdin[2];
    pipe(pipefdout);
    pipe(pipefdin);
    signal(SIGINT, SIG_IGN);
    //Fork stuff up
    int id = fork();
    
    if(id == 0){
        signal(SIGINT, SIG_DFL);
        if (outputRedirect){
            // we do be in the child process doh
            close(pipefdout[0]); 
            //Dup the pipe to stdout
            dup2(pipefdout[1], 1); 
        }
        if (inputRedirect){
            int fd_in = open(inputRedirectFileName, O_RDONLY);
            close(0);
            dup(fd_in);
            close(fd_in);
            
        }
        //Run the command through a system call and check that the command exits
        if (execvp(cmd, argv) == -1) {
            exit(EXIT_FAILURE);
        }
    
    }
    else{

        if (outputRedirect){
            //Ofstream will redirect output to a file
            ofstream out(outputRedirectFileName);
            //Close the write end of the pipe
            close(pipefdout[1]);
            //Open the read end of the pipe as a file and yeet
            FILE *readP = fdopen(pipefdout[0], "r");
            char c[1000];
            
            while (fgets(c,999,readP) != NULL){
                out << c;
            }

            //Clean up and close
            close(pipefdout[0]);
            out.close();
        }
        outputRedirect = 0;
        inputRedirect = 0;
        WaitFor(id);
    }

}

