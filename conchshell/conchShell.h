#include <iostream>
#include <fstream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <filesystem>
#include <vector>

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

    //History conditions
    vector<string> history;

    //Redirect conditions
    int redirect = 0;
    char *redirectFileName;

    //Run the Loop
    void loop();

    //Shell builtins
    int checkForBuiltIns(Token *tokens);
    void execute(Token *tokens);
    void conch_cd(string directory);

    //Functions to actually execute the system command
    void WaitFor(int pid);
    void execute_sys_command(char *cmd, char *argv[]);

    //Parse and execute the command
    void parse(string c);
    
};

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
    while(true){
        cout << greeting;
        //Gets the entire line off of the command line
        getline(cin, command);
        parse(command);
    }
}

int conchShell::checkForBuiltIns(Token *tokens){
    //if the first token is exit, we gotta exit
    if (tokens->type == TOKEN_EXIT){
        exit(0);
    }
    //Check for the cd command, so we can change directory
    else if (tokens->type == TOKEN_CD){
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
        string historic_command = history[history.size() -2];
        parse(historic_command); 
        return 0;    
    }
    
    //Check for listing history
    else if(tokens->type == TOKEN_HISTORY){
        for(int i = 0; i < history.size(); i++){
            cout << history[i] << endl;
        }
        return 1;
    }
    else return 0;
}

//Parse the tokens as they come in and execute commands as necessary
void conchShell::parse(string command){
    
    //Add the command to history
    history.push_back(command);

    //convert the command from a C++ string to a c-style string
    char *command_cstring = &command[0];

    //Do some lexing
    Token *tokens= lex(command_cstring);

    //First we gotta check for builtins 
    if (checkForBuiltIns(tokens)){
        return;
    }

    //If we aren't a builtin, we are a system command
    execute(tokens);
}


void conchShell::execute(Token *tokens){
    int tracker = 0;
    int fileOutput = 0;

    vector<string>  inputTokens;
    while(tokens!=NULL)
    {
        //Check is file redirect is present
        if (tokens->type == TOKEN_FILE_OUTPUT){
            fileOutput = tracker;
            redirect = 1;
        }
        //Push the indentifer, the string itself, into the vector
        inputTokens.push_back(tokens->identifier);
        tokens=tokens->next;
        tracker++;
    }

    int num_tokens;
    if (fileOutput == 0){
        num_tokens = inputTokens.size();
    }
    else{
        num_tokens = fileOutput;
        redirectFileName = &inputTokens[fileOutput + 1][0];
    }
    
    char *cmd = &inputTokens[0][0];
    char *argv[num_tokens];

    for(int i = 0; i < num_tokens; i++) {
            argv[i] = &inputTokens[i][0];
    }

    //Termiate the argv for the execvp command
    argv[num_tokens] = NULL;
    execute_sys_command(cmd,argv);

}

void conchShell::WaitFor(int pid){
  int gotpid, status;
  while( (gotpid = wait(&status)) != pid);
}

void conchShell::execute_sys_command(char *cmd, char *argv[]){
    //Pipe up
    int pipefd[2];
    pipe(pipefd);

    //Fork stuff up
    int id = fork();
    
    if(id == 0){ 
        if (redirect){
            // we are in the child process
            close(pipefd[0]); 
            //Dup the pipe to stdout
            dup2(pipefd[1], 1); 
        }
        //Run the command through a system call
        execvp(cmd, argv);
    }
    else{

        if (redirect){
            //Ofstream will redirect output to a file
            ofstream out(redirectFileName);
            //Close the write end of the pipe
            close(pipefd[1]);
            //Open the read end of the pipe as a file
            FILE *readP = fdopen(pipefd[0], "r");
            char c[1000];
            
            while (fgets(c,999,readP) != NULL){
                out << c;
            }

            //Clean up and close
            close(pipefd[0]);
            out.close();
        }
        //Reset the file redirect
        redirect = 0;
        WaitFor(id);
    }

}

