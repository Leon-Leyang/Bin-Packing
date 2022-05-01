#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

// class A{
// public:    
//     A(int a): a(a){}
//     const int a;
//     int getNum(){
//         return a;
//     }
// };

map<vector<int>, int> findMaxVol(vector<int> items, int cap){
    // cout << "cap: "<< cap << endl;
    // cout << "items len: " << items.size() << endl;
    int i;
    for(i = 0; i < items.size(); i++){
        if(items[i] <= cap){
            break;
        }
    }
    if(i == items.size()){
        vector<int> noItem;
        map<vector<int>, int> ret = {{noItem, 0}};
        return ret;
    }else if(i == items.size() - 1){
        vector<int> vec{items[i]};
        map<vector<int>, int> ret = {{vec, items[i]}};
        return ret;
    }

    map<vector<int>, int> map1 = findMaxVol(vector<int>(items.begin() + 1, items.end()), cap - items[i]);
    auto it1 = map1.begin();
    int vol1 = it1->second + items[i];
    vector<int> items1 = it1->first;
    vector<int> itemVec{items[i]};
    itemVec.insert(itemVec.end(), items1.begin(), items1.end());

    map<vector<int>, int> map2 = findMaxVol(vector<int>(items.begin() + 1, items.end()), cap);
    auto it2 = map2.begin();
    int vol2 = it2->second;
    vector<int> items2 = it2->first;

    if(vol1 > vol2){
        map<vector<int>, int> ret = {{itemVec, vol1}};
        return ret;
    }else{
        map<vector<int>, int> ret = {{items2, vol2}};
        return ret;
    }
}






int main(){
    // for(int i = 1; i < 3; i++){
    //     A a(i);
    //     int n = a.getNum();
    //     cout << n << endl;
    // }

    srand(20);
    // for (int i = 0; i < 10; i++) printf("%d\n", rand()%100+1);   

    vector<int> test{11, 6, 5, 4, 1};
    map<vector<int>, int> tmap = findMaxVol(test, 13);
    auto t = tmap.begin();
    cout << "Max vol: " << t->second <<endl; 

    vector<int> a = t->first;

    for(int i = 0; i<a.size(); i++){
        cout << a[i] << endl;
    }

    return 0;
}