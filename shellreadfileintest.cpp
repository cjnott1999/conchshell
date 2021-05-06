// read file in command
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string readFileIn(string filename) {
    ifstream file;
    file.open(filename);
    string line;
    string stringedFile = "";
    while(getline(file, line)){
        stringedFile = stringedFile + line + "\n";
    }
    file.close();
    return stringedFile;
}

int main(){
    cout << readFileIn("example.txt");
}