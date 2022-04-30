#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>

using namespace std;


// Declaration of the classes used in main funciton

// Class of a problem instance
class Problem{
private:
    int id;
    int itemNum;
    int binNum;
};


// Class used to manage io related operations
class IOManager{
public:
    // Function to get command line arguments
    static char* getCmdArg(char ** begin, char ** end, const string & arg);

    // Function to load problem from the given file and initialize
    static vector<Problem> loadProblem();
    
};






// Main function
int main(int argc, char* argv[]){

    char* dataFile = IOManager::getCmdArg(argv, argv + argc, "-s");
    char* solutionFile = IOManager::getCmdArg(argv, argv + argc, "-o");
    int maxTime = atoi(IOManager::getCmdArg(argv, argv + argc, "-t"));

    cout << "Data: " << dataFile << endl;
    cout << "Solution: " << solutionFile << endl;
    cout << "Max time: " << maxTime << endl;
}


// Function to get command line arguments
char* IOManager::getCmdArg(char ** begin, char ** end, const string & arg){
        char ** val = find(begin, end, arg);
        if (val != end && ++val != end)
        {
            return *val;
        }
        return 0;
}


// Function to load problem from the given file and initialize
vector<Problem> IOManager::loadProblem(){
    vector<Problem> problems;
    return problems;
}










// class Problem{
// private:
//     int id;
//     int itemNum;
//     int binNum;
// };



// class Solution{
// private:
//     vector<Bin> bins;
//     int binNum;
//     int obj;
// };


// class Bin{
// private:
//     vector<Item> items;
//     int capLeft;
//     int cap;
// };


// class Item{
// private: 
//     int id;
//     int vol;
// };