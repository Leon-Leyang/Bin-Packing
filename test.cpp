#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

class A{
public:    
    A(int a){
        this->a = a;
    }
    int a;
    int getNum(){
        return a;
    }
};

int main(){
    // for(int i = 1; i < 3; i++){
    //     A a(i);
    //     int n = a.getNum();
    //     cout << n << endl;
    // }

    string line = " 150 120 48";

    string buf;
    stringstream ss(line);

    while (ss >> buf){
        cout << "pass" << buf << endl;
    }
       

    return 0;
}