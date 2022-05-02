#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <time.h>

using namespace std;


// Declaration of the classes used in main funciton

// Class for item
class Item{
private: 
    int id;
    int vol;
public:
    // Constructor
    Item();
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
    Bin();
    Bin(int id, int cap);

    // Getters and setters
    int getCapLeft() const;
    int getCap() const;
    vector<Item> getItems() const;
    void setCapLeft(int capLeft);
    void setCap(int cap);
    void addItem(Item item);
    void removeItem(Item item);

    // Overload == operator to compare if two bins are the same
    bool operator ==(const Bin& bin);
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
    int getBinNumLB() const;    // Funtion to return the lower bound of the bin number
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
    // Unfilled bins
    vector<Bin> ufBins;
    // Filled bins
    vector<Bin> fBins;
    const Problem& problem;
public:
    // Constructor
    Solution(const Problem& problem);
    Solution(const Solution &soluton);

    //Function to calculate the absolute gap between the solution's bin number and optimal
    int getAbsGap();

    // Getters and setters
    int getBinNum() const;
    int getBinNumLB() const;    // Funtion to return the lower bound of the bin number
    int getObj() const;
    vector<Item> getItems() const;
    vector<Bin> getUFBins() const;
    vector<Bin> getFBins() const;
    void setBinNum(int binNum);
    void setObj(int obj);
    void setUFBins(vector<Bin> ufBins);
    void setFBins(vector<Bin> fBins);

    // Overload = operator to assign a solution to the current solution
    Solution& operator=(const Solution& solution);
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

    // Function to compare volumns of two items
    // Used to sort the unpacked items in a descending order
    static bool compareItem(Item item1, Item item2);   

    // Function to compare capacity left of two bins
    // Used to sort the unfilled bins in an ascending order
    static bool compareBin(Bin bin1, Bin bin2);   
};

// Class for best fit
class BFHeuristic: public Heuristic{
public:
    // Function to generate solution with Best Fit heuristic 
    virtual Solution genSolution(const Problem& problem);
};


// Class for minimum bin slack(MBS) heuristic
// class MBSHeuristic: public Heuristic{
// public:
//     // Function to generate solution with MBS heuristic 
//     virtual Solution genSolution(const Problem& problem);
// private:
//     // Function to find the maximum packing given a restricted capacity and a vector of unpacked items in a descending order of volumn
//     map<int, vector<Item>> findMaxPack(int cap, vector<Item> uItems);
// };


// Class solving the problem
class Solver{
public:
    // Constructor and destructor
    Solver(int maxTime);
    ~Solver();

    // Function to solve a problem
    Solution solve(const Problem& problem);
private:
    // Maximum computation time for one run
    int maxTime;

    // Pointer to the heuristic
    Heuristic* heuristic;

    // Function to initialize a solution with MBS heuristic
    Solution initSolution(const Problem& problem);

    // Function to carry out Variable Neighbourhood Search(VNS)
    Solution VNS(Solution initSol);

    // Funtion to evaluate the objective of a solution
    int eval(Solution solution);

    // Funtion to compare two solutions
    bool compareSols(Solution solution1, Solution solution2);

    // Function to search the nth neighborhood of the given solution with best descent and get the local optimal solution
    Solution searchNthNB(Solution solution, int count);

    // Funtion to shake the solution
    Solution shaking(Solution solution);
};
























// Main function
int main(int argc, char* argv[]){
    // Set the random seed
    srand(20);

    // Initialize an IOManager with the command line arguments
    IOManager ioManager = IOManager(argc, argv);

    // Load the problems
    vector<Problem> problems = ioManager.loadProblems();

    // Initialze a solver
    int maxTime = ioManager.getMaxTime();
    Solver solver(maxTime);

    // Initialize a vector of solutions
    vector<Solution> solutions;

    // Generate a solution for each problem and push to the solution vector
    for(int i = 0; i < problems.size(); i++){
        Solution solution = solver.solve(problems[i]);
        solutions.push_back(solution);
        cout << "gap: " << solution.getAbsGap() << endl << endl;
    }

    // Solution solution = solver.solve(problems[0]);
    // cout << "gap: " << solution.getAbsGap() << endl;
}
































// Constructor for Item
Item::Item(){}
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
Bin::Bin(){}
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
void Bin::setCap(int cap){
    this->cap = cap;
}
void Bin::addItem(Item item){
    this->items.push_back(item);
    capLeft -= item.getVol();
}
void Bin::removeItem(Item item){
    auto it = find(items.begin(), items.end(), item);
    if(it != items.end()){  // If the item is in items
        int index = it - items.begin();
        items.erase(items.begin() + index);
        capLeft += item.getVol();
    }else{
        cout << "Fail to find the item to delete!" << endl;
    }
}

// Overload == operator to compare if two bins are the same
bool Bin::operator ==(const Bin& bin){
        return (this->id == bin.id);
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
int Problem::getBinNumLB() const{    // Funtion to return the lower bound of the bin number
    int sumVol = 0;
    for(Item item : items){
        sumVol += item.getVol();
    }
    float num = static_cast< float > (sumVol) / binCap;
    int binNumLB = ceil(num);
    return binNumLB;
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
Solution::Solution(const Solution& solution): problem(solution.problem){
        binNum = solution.binNum;
        obj = solution.obj;
        ufBins = solution.ufBins;
        fBins = solution.fBins;
}

// Getters and setters
int Solution::getBinNum() const{
    return binNum;
}
int Solution::getBinNumLB() const{    // Funtion to return the lower bound of the bin number
    return problem.getBinNumLB();
}
int Solution::getObj() const{
    return obj;
}
vector<Item> Solution::getItems() const{
    return problem.getItems();
}
vector<Bin> Solution::getUFBins() const{
    return ufBins;
}
vector<Bin> Solution::getFBins() const{
    return fBins;
}
void Solution::setObj(int obj){
    this->obj = obj;
}
void Solution::setUFBins(vector<Bin> ufBins){
    this->ufBins = ufBins;
    binNum = ufBins.size() + fBins.size();
}
void Solution::setFBins(vector<Bin> fBins){
    this->fBins = fBins;
    binNum = ufBins.size() + fBins.size();
}

//Function to calculate the absolute gap between the solution's bin number and optimal
int Solution::getAbsGap(){
    return binNum - problem.getBinNum();
}

// Overload = operator to assign a solution to the current solution
Solution& Solution::operator=(const Solution& solution){
    this->binNum = solution.binNum;
    this->obj = solution.obj;
    this->ufBins = solution.ufBins;
    this->fBins = solution.fBins;

    return *this;
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
// Used to sort the unpacked items in an descending order
bool Heuristic::compareItem(Item item1, Item item2){
    return (item1.getVol() > item2.getVol());
}

// Function to compare capacity left of two bins
// Used to sort the unfilled bins in an ascending order
bool Heuristic::compareBin(Bin bin1, Bin bin2){
    return (bin1.getCapLeft() < bin2.getCapLeft());
}

// Function to generate solution with Best Fit heuristic 
Solution BFHeuristic::genSolution(const Problem& problem){
    // Initialize vector that stores unfilled bins in the ascending order of left capacity
    vector<Bin> ufBins;
    vector<Bin> fBins;

    // Initialize the solution
    Solution solution(problem);
    
    // Get the items in the problem
    vector<Item> items = problem.getItems();

    vector<Item> uItems = items;

    // Sort the unpacked items in a descending order of volumn
    sort(uItems.begin(), uItems.end(), Heuristic::compareItem);

    int binNum = 0;
    // Pack the items according to BF until all items are packed
    while(uItems.size() > 0){
        Item item = uItems[0];

        int i = 0;
        Bin bin;

        if(ufBins.size() == 0){ // If currently no unfilled bin available
            Bin newBin(binNum, problem.getBinCap());
            bin = newBin;
            binNum++;
            bin.addItem(item);
            ufBins.push_back(bin);
            i = ufBins.size() - 1;

        }else{  // Else find the bin with the smallest left capacity that can pack the item
            for(i = 0; i < ufBins.size(); i++){
                bin = ufBins[i];
                if(bin.getCapLeft() >= item.getVol()){  // If there is unfilled bin that can pack the item
                    bin.addItem(item);
                    ufBins[i] = bin;
                    break;
                }
            }
            if(i == ufBins.size()){ // If there is no unfilled bin that is big enough to pack the item
                Bin newBin(binNum, problem.getBinCap());
                bin = newBin;
                binNum++;
                bin.addItem(item);
                ufBins.push_back(bin);
                i = ufBins.size() - 1;
            }
        }

        // Remove the item from unpacked items
        uItems.erase(uItems.begin() + 0);

        // If the unfilled bin is now filled
        // Move it to filled bins
        if(bin.getCapLeft() == 0){
            fBins.push_back(bin);
	        ufBins.erase(ufBins.begin() + i);
        }else{// Else sort the unfilled items in an ascending order of capacity left
            sort(ufBins.begin(), ufBins.end(), Heuristic::compareBin);
        } 
    }

    // cout << "Bin num: " << binNum << endl;
    solution.setUFBins(ufBins);
    solution.setFBins(fBins);

    return solution;
}








// Constructor and destructor
Solver::Solver(int maxTime){
    heuristic = new BFHeuristic();
    this->maxTime = maxTime;
}
Solver::~Solver(){
    delete heuristic;
}

// Function to solve a problem
Solution Solver::solve(const Problem& problem){
    Solution initSol(initSolution(problem));
    
    cout << "init gap: " << initSol.getAbsGap() << endl;

    Solution bestSol(VNS(initSol));

    return bestSol;
}

// Function to initialize a solution with heuristic
Solution Solver::initSolution(const Problem& problem){
    Solution initSol = heuristic->genSolution(problem);
    initSol.setObj(eval(initSol));

    return initSol;
}

// Funtion to evaluate the objective of a solution
int Solver::eval(Solution solution){
    int obj = 0;

    // Get the filled bins and unfilled bins in the solution
    vector<Bin> fBins = solution.getFBins();
    vector<Bin> ufBins = solution.getUFBins();

    // For filled bins, add the square of its used capacity(i.e. its capacity)
    for(Bin bin : fBins){
        obj += bin.getCap() * bin.getCap();
    }

    // For unfilled bins, add the square of its used capacity
    for(Bin bin : ufBins){
        obj += (bin.getCap() - bin.getCapLeft()) * (bin.getCap() - bin.getCapLeft());
    }

    return obj;
}

// Funtion to compare two solutions
// Return true if solution1 is better than solution2
bool Solver::compareSols(Solution solution1, Solution solution2){
    // First compare the bin numbers of two solutions
    if(solution1.getBinNum() < solution2.getBinNum()){
        return true;
    }else if(solution1.getBinNum() > solution2.getBinNum()){
        return false;
    }

    // If the two solutions have the same bin number
    // Compare the objective values
    if(solution1.getObj() > solution2.getObj()){
        return true;
    }else{
        return false;
    }
}

// Function to search the nth neighborhood of the given solution with best descent and get the local optimal solution
// Nth neighborhood is generated from (n-1)_swap(swap n items with 1 item)
Solution Solver::searchNthNB(Solution solution, int n){
    // cout << "\nSearch " << n << "th neighborhood" << endl;

    // Initialize the local optimal solution
    Solution lOptSol = solution;

    // Get the unfilled bins and filled bins
    vector<Bin> ufBins = solution.getUFBins();
    vector<Bin> fBins = solution.getFBins();

    // 
    for(Bin nBin : ufBins){
        // Get the items in the bin
        vector<Item> items = nBin.getItems();

        // Vector to store the (n - 1) items
        vector<Item> nItems;

        // Make sure the nItems has no item inside
        nItems.clear();

        // If the number of items in the bin is less than n - 1(i.e. no n-1 items can be selected)
        // Skip the bin
        if(items.size() < n - 1){
            continue;
        }

        // Select n - 1 items from the bin if it is not 0_swap
        if(n - 1 != 0){
            // Generate a vector of indexes
            vector<int> indexes;
            for(int i = 0; i < items.size(); i++){
                indexes.push_back(i);
            }

            // Generate random index and push the item at the index to nItems
            for(int i = 0; i < n - 1; i++){
                int vecSize = indexes.size();
	            int randNum = rand() % vecSize;
                int index = indexes[randNum];
                indexes.erase(indexes.begin() + randNum);

                Item item = items[index];
                nItems.push_back(item);
            }
        }

        // Int to record the total volumn of the (n - 1) items
        int nItemsVol = 0;

        // If there is item in the nItems
        // Calculate the total volumn
        if(n - 1 != 0){
            for(Item nItem : nItems){
                nItemsVol += nItem.getVol();
            }
        }


        
        for(Bin oBin : ufBins){
            // If the oBin is the same as the nBin, skip it
            if(nBin == oBin){
                continue;
            }

            // For each item in the oBin
            // Try to swap it with the (n - 1) items
            for(Item oItem : oBin.getItems()){
                int oItemVol = oItem.getVol();

                // If the capacity of both bins will not be violated after the swap, perform the swap and generate a new solution
                // Otherwise skip it
                if(oItemVol - nItemsVol <= nBin.getCapLeft() && nItemsVol - oItemVol <= oBin.getCapLeft()){
                    Solution curSol(solution);
                    vector<Bin> newUFBins = curSol.getUFBins();
                    vector<Bin> newFBins = curSol.getFBins();

                    Bin oBinCopy = oBin;
                    Bin nBinCopy = nBin;

                    // Erase the nBin and oBin from newUFBins
                    for(int i = 0; i < newUFBins.size(); i++){
                        if(newUFBins[i] == nBinCopy || newUFBins[i] == oBinCopy){
                            newUFBins.erase(newUFBins.begin() + i);
                        }
                    }
                    
                    // Remove the oItem from oBin
                    oBinCopy.removeItem(oItem);

                    // If nItems is not empty, remove the items from nBin add them to oBin
                    if(n - 1 != 0){
                        for(Item nItem : nItems){
                            nBinCopy.removeItem(nItem);
                            oBinCopy.addItem(nItem);
                        }        
                    }

                    // Add the oItem to nBin
                    nBinCopy.addItem(oItem);

                    // Push nBin to the corresponding vector
                    if(nBinCopy.getCapLeft() == 0){
                        newFBins.push_back(nBinCopy);
                    }else if(nBinCopy.getCapLeft() != nBinCopy.getCap()){
                        newUFBins.push_back(nBinCopy);
                    }

                    // Push oBin to the corresponding vector
                    if(oBinCopy.getCapLeft() == 0){
                        newFBins.push_back(oBinCopy);
                    }else if(oBinCopy.getCapLeft() != oBinCopy.getCap()){
                        newUFBins.push_back(oBinCopy);
                    }

                    curSol.setUFBins(newUFBins);
                    curSol.setFBins(newFBins);

                    // Evaluate the current solution and set its objective value
                    curSol.setObj(eval(curSol));

                    // Update the local optimal solution
                    if(compareSols(curSol, lOptSol)){
                        // cout << "Update the local optimal" << endl;
                        // cout << "pre bin num: " << lOptSol.getBinNum() << "; now bin num: " << curSol.getBinNum() << endl;
                        // cout << "pre obj: " << lOptSol.getObj() << "; now bin num: " << curSol.getObj() << endl;
                        lOptSol = curSol;
                    }
                }
            }
        }
    }
    
    return lOptSol;
}

// Funtion to shake the solution
Solution Solver::shaking(Solution solution){

    // Get the unfilled bins and filled bins
    vector<Bin> ufBins = solution.getUFBins();
    vector<Bin> fBins = solution.getFBins();

    // Initialize the vectors of fBins indexes and ufBins indexes
    vector<int> fIndexes;
    vector<int> ufIndexes;
    for(int i = 0; i < fBins.size(); i++){
        fIndexes.push_back(i);
    }
    for(int i = 0; i < ufBins.size(); i++){
        ufIndexes.push_back(i);
    }

    bool findTrans = false;

    Bin fBin;
    Bin ufBin;
    Item fItem;

    do{
        // Get a random filled bin
        int randNum = rand() % fIndexes.size();
        int fIndex = fIndexes[randNum];
        fBin = fBins[fIndex];
        fIndexes.erase(fIndexes.begin() + randNum);

        // Initialize a vector of items indexes in the filled bin
        vector<int> fItemsIndexes;
        for(int i = 0; i < fBin.getItems().size(); i++){
            fItemsIndexes.push_back(i);
        }

        do{
            // Get a random item from the filled bin 
            randNum = rand() % fItemsIndexes.size();
            int fItemIndex = fItemsIndexes[randNum];
            fItem = fBin.getItems()[fItemIndex];
            fItemsIndexes.erase(fItemsIndexes.begin() + randNum);

            
            do{
                // Get a random unfilled bin
                randNum = rand() % ufIndexes.size();
                int ufIndex = ufIndexes[randNum];
                ufBin = ufBins[ufIndex];
                ufIndexes.erase(ufIndexes.begin() + randNum);
            }while(fItem.getVol() > ufBin.getCapLeft() && ufIndexes.size() > 0);

            // If the unfilled bin is able to pack the item from the filled bin
            if(fItem.getVol() <= ufBin.getCapLeft()){
                findTrans = true;
            }

        }while(findTrans == false && fItemsIndexes.size() > 0);

    }while(findTrans == false && fIndexes.size() > 0);

    if(findTrans == true){
        fBin.removeItem(fItem);
        ufBin.addItem(fItem);

        // Erase the fBin from fBins
        for(int i = 0; i < fBins.size(); i++){
            if(fBins[i] == fBin){
                fBins.erase(fBins.begin() + i);
            }
        }

        ufBins.push_back(fBin);

        // Erase the ufBin from ufBins
        for(int i = 0; i < ufBins.size(); i++){
            if(ufBins[i] == ufBin){
                ufBins.erase(ufBins.begin() + i);
            }
        }

        if(ufBin.getCapLeft() == 0){
            fBins.push_back(ufBin);
        }else{
            ufBins.push_back(ufBin);
        }

        // cout << "\nShake the solution!" << endl;
        // cout << "pre obj: " << solution.getObj(); 

        solution.setFBins(fBins);
        solution.setUFBins(ufBins);

        solution.setObj(eval(solution));

        // cout << "; now obj: " << solution.getObj() << endl;

    }else{
        // cout << "\nFail to shake the solution!" << endl;
    }
    
    return solution;
}

// Function to carry out Variable Neighbourhood Search(VNS)
Solution Solver::VNS(Solution initSol){
    // Const for the number of neighborhood that VNS use
    const int NB_NUM = 5;

    // Initialize the starting time
    clock_t start;
    start = clock();

    // Initialize the current solution and the best solution
    Solution curSol = initSol;
    Solution bestSol = initSol;

    // Initialize the neighborhood counter
    int nbCount = 1;

    int i = 0;

    // Stopping criteria
    while((double(clock() - start)/CLOCKS_PER_SEC) < maxTime && bestSol.getBinNum() != bestSol.getBinNumLB()){
        // Neighborhood search for intensification
        while(nbCount < NB_NUM){
            // Search the current nbCount th neighborhood with best descent and get the local optimal solution
            Solution lOptSol = searchNthNB(curSol, nbCount);

            // If the local optimal solution is better than the current solution
            // Replace the current solution with the local optimal solution and start again from the new first neighborhood
            if(compareSols(lOptSol, curSol)){
                curSol = lOptSol;
                nbCount = 1;
                continue;
            }else{
                nbCount++;
            }
        }
        // Update the best solution
        if(compareSols(curSol, bestSol)){
            // cout << "Update the global optimal" << endl;
            // cout << "pre bin num: " << bestSol.getBinNum() << "; now bin num: " << curSol.getBinNum() << endl;
            // cout << "pre obj: " << bestSol.getObj() << "; now bin num: " << curSol.getObj() << endl;
            bestSol = curSol; 
        }

        // Shaking the current solution for diversification
        // curSol = shaking(curSol);
        
        // nbCount = 1;
    }
    
    return bestSol;
}







































// Function to generate solution with MBS heuristic 
// Solution MBSHeuristic::genSolution(const Problem& problem){
//     vector<Bin> unfilledBins;
//     vector<Bin> filledBins;


//     // Initialize the solution
//     Solution solution(problem);

//     // Get the items in the problem
//     vector<Item> items = problem.getItems();

//     // Initialize the unpacked items
//     vector<Item> uItems = items;

//     // Sort the unpacked items in a descending order of volumn
//     sort(uItems.begin(), uItems.end(), Heuristic::compareItem);

//     // Pack the items according to MBS until all items are packed
//     int binNum = 0;
//     while(uItems.size() > 0){
//         Bin bin(binNum, problem.getBinCap());
//         binNum++;

//         map<int, vector<Item>> maxPacking = findMaxPack(bin.getCapLeft(), uItems);
//         auto it = maxPacking.begin();
//         int maxVol = it->first;
//         vector<Item> pItems = it->second;
//         bin.setCapLeft(bin.getCapLeft() - maxVol);
//         for(Item item : pItems){
//             bin.addItem(item);

// 	        auto it2 = find(uItems.begin(), uItems.end(), item);
// 	        int index = it2 - uItems.begin();
// 	        uItems.erase(uItems.begin() + index);
//         }

//         bins.push_back(bin);
//     }

//     // Set the solution
//     solution.setBins(bins);
//     solution.setBinNum(binNum);
//     cout << "gap" << solution.getAbsGap() << endl;

//     return solution;
// }


// Function to find the maximum packing given a restricted capacity and a vector of unpacked items in a descending order of volumn
// Return the map of the total volumn and the items in the packing
// map<int, vector<Item>> MBSHeuristic::findMaxPack(int cap, vector<Item> uItems){
//     int i;

//     // Find the index of item with the maximum volumn and is smaller than the capacity
//     for(i = 0; i < uItems.size(); i++){
//         if(uItems[i].getVol() <= cap){
//             break;
//         }
//     }

//     // Base case
//     if(i == uItems.size()){ // If there is no such item, return the map of volumn 0 and the vector containing no item
//         vector<Item> retItem;
//         map<int, vector<Item>> ret = {{0, retItem}};
//         return ret;
//     }else if(i == uItems.size() - 1){ // If the item is the last one in the vector, return the map of its volumn and the vector containing it
//         vector<Item> retItem{uItems[i]};
//         map<int, vector<Item>> ret = {{uItems[i].getVol(), retItem}};
//         return ret;
//     }

//     // Recursive case
//     // If the current item is chosen
//     map<int, vector<Item>> map1 = findMaxPack(cap - uItems[i].getVol(), vector<Item>(uItems.begin() + 1, uItems.end()));
//     auto it1 = map1.begin();
//     int vol1 = it1->first + uItems[i].getVol();
//     vector<Item> retItems = it1->second;
//     vector<Item> pItems1{uItems[i]};
//     pItems1.insert(pItems1.end(), retItems.begin(), retItems.end());
//     // If the current item is not chosen
//     map<int, vector<Item>> map2 = findMaxPack(cap, vector<Item>(uItems.begin() + 1, uItems.end()));
//     auto it2 = map2.begin();
//     int vol2 = it2->first;
//     vector<Item> pItems2 = it2->second;

//     // Return the case with larger total volumn
//     if(vol1 >= vol2){
//         map<int, vector<Item>> ret = {{vol1, pItems1}};
//         return ret;
//     }else{
//         map<int, vector<Item>> ret = {{vol2, pItems2}};
//         return ret;
//     }
// }