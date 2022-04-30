#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;


// Declaration of the classes used in main funciton

// Class for item
class Item{
private: 
    int id;
    int vol;
public:
    // Constructor
    Item(int id, int vol);
};

// Class for bin
class Bin{
private:
    vector<Item> items;
    int capLeft;
    int cap;
    int id;
public:
    // Constructor
    Bin(int id, int cap);
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
    string id;
    int itemNum;
    int binNum;
    int binCap;
    vector<Item> items;

public:
    // Constructor
    Problem(string id);

    // Getters and Setters
    int getBinCap();
    int getItemNum();
    int getBinNum();
    vector<Item> getItems();
    void setBinCap(int binCap);
    void setItemNum(int itemNum);
    void setBinNum(int binNum);
    void addItem(Item item);
};

// Class managing io related operations
class IOManager{
public:
    // Constructor
    IOManager(int argc, char* argv[]);

    // Function to load problems from the data file
    vector<Problem> loadProblems();
    
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
    // Initialize an IOManager with the command line arguments
    IOManager ioManager = IOManager(argc, argv);

    int maxTime = ioManager.getMaxTime();

    // Load the problems
    vector<Problem> problems = ioManager.loadProblems();

    
}










// Constructor for Item
Item::Item(int id, int vol){
    this->id = id;
    this->vol = vol;
}

// Constructor for Bin
Bin::Bin(int id, int cap){
    this->id = id;
    this->cap = cap;
}




// Constructor for Problem
Problem::Problem(string id){
    this->id = id;
}

// Getters and Setters
int Problem::getBinCap(){
    return binCap;
}
int Problem::getItemNum(){
    return itemNum;
}
int Problem::getBinNum(){
    return binNum;
}
vector<Item> Problem::getItems(){
    return items;
}
void Problem::setBinCap(int binCap){
    this->binCap = binCap;
}
void Problem::setItemNum(int itemNum){
    this->itemNum = itemNum;
}
void Problem::setBinNum(int binNum){
    this->binNum = binNum;
}
void Problem::addItem(Item item){
    this->items.push_back(item);
}


// Constructor for IOManager
IOManager::IOManager(int argc, char* argv[]){
    dataFile = getCmdArg(argv, argv + argc, "-s");
    solutionFile = getCmdArg(argv, argv + argc, "-o");
    maxTime = atoi(getCmdArg(argv, argv + argc, "-t"));
}

// Function to load problems from the data file
vector<Problem> IOManager::loadProblems(){
    // Open the data file
    ifstream file(dataFile);

    vector<Problem> problems;
    string line;

    getline(file, line);
    int problemNum = stoi(line);

    while(getline(file, line)){
        // If the problem id is read
        if(line.find("u") != string::npos){

            // Remove blank space in the problem id
            line.erase(remove(line.begin(), line.end(),' '), line.end());
            string id = line;

            // Initialize a new problem
            Problem problem(id);

            // Get the line after it and set the capacity of bin, item number and objective value of the problem
            getline(file, line);
            stringstream ss(line);
            string buf;
            int count = 0;
            int itemNum = 0;
            while (ss >> buf){
                switch(count){
                    case 0:
                        problem.setBinCap(stoi(buf));
                        break;
                    case 1:
                        itemNum = stoi(buf);
                        problem.setItemNum(itemNum);
                        break;
                    case 2:
                        problem.setBinNum(stoi(buf));
                        break;
                }
                count++;
            }

            // Get items' number of lines after it, initialize items accordingly and adding them to the problem
            for(int i = 0; i < itemNum; i++){
                getline(file, line);
                Item item(i, stoi(line));
                problem.addItem(item);
            }

            problems.push_back(problem);
        }
    }

    // Close the file
    file.close();

    if(problems.size() != problemNum){
        cout << "The number of problems declared doesn't match the real number of problems" << endl;
        exit(1);
    }
    
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


