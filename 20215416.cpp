#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>

using namespace std;


// Declaration of the classes used in main funciton

// Class for item
class Item{
private: 
    int id;
    int vol;
};

// Class for bin
class Bin{
private:
    vector<Item> items;
    int capLeft;
    int cap;
};

// Class for solution
class Solution{
private:
    vector<Bin> bins;
    int binNum;
    int obj;
};


// Class for problem
class Problem{
private:
    int id;
    int itemNum;
    int binNum;
};

// Class managing io related operations
class IOManager{
public:
    // Constructor
    IOManager(int argc, char* argv[]);

    // Function to load problems from the data file
    vector<Problem> loadProblem();
    
    // Getter for maximum time
    int getMaxTime();
    
private:
    char* dataFile;
    char* solutionFile;
    int maxTime;

    // Function to read command line argument
    char* getCmdArg(char ** begin, char ** end, const string & arg);
};






// Main function
int main(int argc, char* argv[]){

    IOManager ioManager = IOManager(argc, argv);

    int maxTime = ioManager.getMaxTime();

    cout << "Max time: " << maxTime << endl;
}







// Constructor for IOManager
IOManager::IOManager(int argc, char* argv[]){
    dataFile = getCmdArg(argv, argv + argc, "-s");
    solutionFile = getCmdArg(argv, argv + argc, "-o");
    maxTime = atoi(getCmdArg(argv, argv + argc, "-t"));
}

// Function to load problems from the data file
vector<Problem> IOManager::loadProblem(){
    vector<Problem> problems;
    return problems;
}

// Getter for maximum time
int IOManager::getMaxTime(){
    return maxTime;
}

// Function to read command line argument
char* IOManager::getCmdArg(char ** begin, char ** end, const string & arg){
        char ** val = find(begin, end, arg);
        if (val != end && ++val != end)
        {
            return *val;
        }
        return 0;
}
