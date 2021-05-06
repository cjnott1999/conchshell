#include "conchShell.h"


using namespace std;

int main(int argc, char* argv[]){
    //Inititate a shell object
    conchShell shell;
    //Begin execution
    shell.setUpShell();
    shell.loop();
    shell.exitShell();
    return 0;
}
