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

    // Getters
    int getId() const;
    int getVol() const;

    // Overload == operator to compare if two items are the same
    bool operator ==(const Item& item);
};

// Class for bin
class Bin{
private: 
    int capLeft;
    int cap;
    int id;
    vector<Item> items;
public:
    // Constructor
    Bin(int id, int cap);

    // Getters and setters
    int getCapLeft() const;
    int getCap() const;
    vector<Item> getItems() const;
    void setCapLeft(int capLeft);
    void setCap(int cap);
    void addItem(Item item);


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

    // Getters and setters
    int getBinNum() const;
    vector<Bin> getBins() const;
    void setBinNum(int binNum);
    void setBins(vector<Bin> bins);
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

// Class for best fit



// Class for minimum bin slack(MBS) heuristic
class [[deprecated]] MBSHeuristic: public Heuristic{
public:
    // Function to generate solution with MBS heuristic 
    virtual Solution genSolution(const Problem& problem);
private:
    // Function to find the maximum packing given a restricted capacity and a vector of unpacked items in a descending order of volumn
    map<int, vector<Item>> findMaxPack(int cap, vector<Item> uItems);

    // Function to compare volumns of two items
    // Used to sort the unpacked items
    static bool compareItem(Item item1, Item item2);
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

    // Solution solution = solver.solve(problems[0]);
}
















// Constructor for Item
Item::Item(int id, int vol){
    this->id = id;
    this->vol = vol;
}

// Getters
int Item::getId() const{
    return id;
}
int Item::getVol() const{
    return vol;
}

// Overload == operator to compare if two items are the same
bool Item::operator ==(const Item& item){
        return (this->id == item.id);
}



// Constructor for Bin
Bin::Bin(int id, int cap){
    this->id = id;
    this->cap = cap;
    this->capLeft = cap;
}


// Getters and setters
int Bin::getCapLeft() const{
    return capLeft;
}
int Bin::getCap() const{
    return cap;
}
vector<Item> Bin::getItems() const{
    return items;
}
void Bin::setCapLeft(int capLeft){
    this->capLeft = capLeft;
}
void Bin::setCap(int cap){
    this->cap = cap;
}
void Bin::addItem(Item item){
    this->items.push_back(item);
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

// Getters and setters
int Solution::getBinNum() const{
    return binNum;
}
vector<Bin> Solution::getBins() const{
    return bins;
}
void Solution::setBinNum(int binNum){
    this->binNum = binNum;
}
void Solution::setBins(vector<Bin> bins){
    this->bins = bins;
}

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


// Function to compare volumns of two items
// Used to sort the unpacked items
bool MBSHeuristic::compareItem(Item item1, Item item2){
    return (item1.getVol() > item2.getVol());
}


// Function to generate solution with MBS heuristic 
Solution MBSHeuristic::genSolution(const Problem& problem){
    vector<Bin> bins;

    // Initialize the solution
    Solution solution(problem);

    // Get the items in the problem
    vector<Item> items = problem.getItems();

    // Initialize the unpacked items
    vector<Item> uItems = items;

    // Sort the unpacked items in a descending order of volumn
    sort(uItems.begin(), uItems.end(), compareItem);

    // 
    int binNum = 0;
    while(uItems.size() > 0){
        Bin bin(binNum, problem.getBinCap());
        binNum++;

        map<int, vector<Item>> maxPacking = findMaxPack(bin.getCapLeft(), uItems);
        auto it = maxPacking.begin();
        int maxVol = it->first;
        vector<Item> pItems = it->second;
        bin.setCapLeft(bin.getCapLeft() - maxVol);
        for(Item item : pItems){
            bin.addItem(item);

	        auto it2 = find(uItems.begin(), uItems.end(), item);
	        int index = it2 - uItems.begin();
	        uItems.erase(uItems.begin() + index);
        }

        bins.push_back(bin);
    }

    solution.setBins(bins);
    solution.setBinNum(binNum);
    cout << "gap" << solution.getAbsGap() << endl;

    return solution;
}


// Function to find the maximum packing given a restricted capacity and a vector of unpacked items in a descending order of volumn
// Return the map of the total volumn and the items in the packing
map<int, vector<Item>> MBSHeuristic::findMaxPack(int cap, vector<Item> uItems){
    int i;

    // Find the index of item with the maximum volumn and is smaller than the capacity
    for(i = 0; i < uItems.size(); i++){
        if(uItems[i].getVol() <= cap){
            break;
        }
    }

    // Base case
    if(i == uItems.size()){ // If there is no such item, return the map of volumn 0 and the vector containing no item
        vector<Item> retItem;
        map<int, vector<Item>> ret = {{0, retItem}};
        return ret;
    }else if(i == uItems.size() - 1){ // If the item is the last one in the vector, return the map of its volumn and the vector containing it
        vector<Item> retItem{uItems[i]};
        map<int, vector<Item>> ret = {{uItems[i].getVol(), retItem}};
        return ret;
    }

    // Recursive case
    // If the current item is chosen
    map<int, vector<Item>> map1 = findMaxPack(cap - uItems[i].getVol(), vector<Item>(uItems.begin() + 1, uItems.end()));
    auto it1 = map1.begin();
    int vol1 = it1->first + uItems[i].getVol();
    vector<Item> retItems = it1->second;
    vector<Item> pItems1{uItems[i]};
    pItems1.insert(pItems1.end(), retItems.begin(), retItems.end());
    // If the current item is not chosen
    map<int, vector<Item>> map2 = findMaxPack(cap, vector<Item>(uItems.begin() + 1, uItems.end()));
    auto it2 = map2.begin();
    int vol2 = it2->first;
    vector<Item> pItems2 = it2->second;

    // Return the case with larger total volumn
    if(vol1 >= vol2){
        map<int, vector<Item>> ret = {{vol1, pItems1}};
        return ret;
    }else{
        map<int, vector<Item>> ret = {{vol2, pItems2}};
        return ret;
    }
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