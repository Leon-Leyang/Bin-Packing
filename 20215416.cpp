#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>

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
    int getBinCap() const;
    int getItemNum() const;
    int getBinNum() const;
    vector<Item> getItems() const;
    void setBinCap(int binCap);
    void setItemNum(int itemNum);
    void setBinNum(int binNum);
    void addItem(Item item);
};

// Class for solution
class Solution{
private:
    int binNum;
    int obj;
    vector<Bin> bins;
    const Problem& problem;
public:
    // Constructor
    Solution(const Problem& problem);

    //Function to calculate the absolute gap between the solution's bin number and optimal
    int getAbsGap();
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

// Interface for heuristic
class Heuristic{
public:
    // Pure virtual function that shall be implemented in derived class to generate solution with the heuristic 
    virtual Solution genSolution(const Problem& problem) = 0;    
};

// Class for minimum bin slack(MBS) heuristic
class MBSHeuristic: public Heuristic{
public:
    // Function to generate solution with MBS heuristic 
    virtual Solution genSolution(const Problem& problem);
private:
    // Function to find the maximum packing given a list of unpacked items and a capacity
    map<vector<Item>, int> findMaxPack(vector<Item> uItems, int cap);
};


// Class solving the problem
class Solver{
public:
    // Constructor and destructor
    Solver();
    ~Solver();

    // Function to solve a problem
    Solution solve(const Problem& problem);
private:
    // Pointer to the heuristic
    Heuristic* heuristic;

    // Function to initialize a solution with MBS heuristic
    Solution initSolution(const Problem& problem);
};















// Main function
int main(int argc, char* argv[]){
    // Set the random seed
    srand(20);

    // Initialize an IOManager with the command line arguments
    IOManager ioManager = IOManager(argc, argv);

    int maxTime = ioManager.getMaxTime();

    // Load the problems
    vector<Problem> problems = ioManager.loadProblems();

    // Initialze a solver
    Solver solver;

    // Initialize a vector of solutions
    vector<Solution> solutions;

    // Generate a solution for each problem and push to the solution vector
    for(int i = 0; i < problems.size(); i++){
        Solution solution = solver.solve(problems[i]);
        solutions.push_back(solution);
    }
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
int Problem::getBinCap() const{
    return binCap;
}
int Problem::getItemNum() const{
    return itemNum;
}
int Problem::getBinNum() const{
    return binNum;
}
vector<Item> Problem::getItems() const{
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

// Constructor for Solution
Solution::Solution(const Problem& problem): problem(problem){}

//Function to calculate the absolute gap between the solution's bin number and optimal
int Solution::getAbsGap(){
    return binNum - problem.getBinNum();
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
        if(line.find("u") != string::npos || line.find("HARD") != string::npos){

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

    // Checker for correctness of the loading
    if(problems.size() != problemNum){
        cout << "The number of problems declared: " << problemNum << endl;
        cout << "The real number of problems: " << problems.size() << endl;
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

// Function to generate solution with MBS heuristic 
Solution MBSHeuristic::genSolution(const Problem& problem){
    Solution solution(problem);
    return solution;
}


// Function to find the maximum packing given a list of unpacked items and a capacity
map<vector<Item>, int> MBSHeuristic::findMaxPack(vector<Item> uItems, int cap){
    map<vector<Item>, int> ret;
    return ret;
}


// Constructor and destructor
Solver::Solver(){
    heuristic = new MBSHeuristic();
}
Solver::~Solver(){
    delete heuristic;
}

// Function to solve a problem
Solution Solver::solve(const Problem& problem){
    Solution initSol = initSolution(problem);
    

    return initSol;
}

// Function to initialize a solution with heuristic
Solution Solver::initSolution(const Problem& problem){
    return heuristic->genSolution(problem);
}