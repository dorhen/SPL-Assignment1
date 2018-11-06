#include "../include/Restaurant.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


Restaurant::Restaurant(const string &configFilePath): open(true){
    ifstream inFile;
    inFile.open(configFilePath);
    if(!inFile.is_open()){
        exit(1);
    }
    string line;
    int count = 0;
    while(!inFile.eof()){
        getline(inFile,line);
        if(line.size() == 0 )
            continue;
        else if(line[0] == '#'){
            count++;
            continue;
        }
        int dishID = 0;
        if(count == 1) {

            stringstream num(line);
            int numOfTables = 0;
            num >> numOfTables;
            vector<int> t(numOfTables);
        }
        else if(count == 2) {
            while (!line.empty()) {
                if (line.find(',')) {
                    string numWord = line.substr(0, line.find(','));
                    line = line.substr(line.find(',') + 1);
                    stringstream num(numWord);
                    int tSize = 0;
                    num >> tSize;
                    this.tables.push_back(new Table(tSize));
                    break;
                } else {
                    stringstream num(line);
                    int tSize = 0;
                    num >> tSize;
                    this.tables.push_back(new Table(tSize));
                    continue;
                }
            }
        }
        else{
             string name = line.substr(0,line.find(','));
             line = line.substr(line.find(',')+1);
             string dishType = line.substr(0,line.find(','));
             string numWord = line.substr(line.find(',')+1);
             stringstream num1(numWord);
             int price = 0;
             num1 >> price;
             menu.push_back(Dish(dishID,name,price,convert(dishType)));
             dishID++;
        }
    }
    inFile.close();
}

void start(){
    std::cout << "Restaurant is now open!" << std::endl;
    std::string command;
    std::string action;
    cin >> command;
    action = command.substr(command.find_first_of(' '),0);
    while (command !="closeall"){
        switch(action) {
            case "Open Table" :

        }
    }

}

int Restaurant::getNumOfTables() const{
    return tables.size();
}

vector<Dish>& Restaurant::getMenu(){
    vector<Dish> &dishRef = menu;
    return dishRef;
}

Table* Restaurant::getTable(int ind){
    if(ind>=tables.size())return nullptr;
    return tables[ind];
}

DishType Restaurant::convert(string str){
    if(str == "VEG") return VEG;
    else if(str == "SPC") return SPC;
    else if(str == "BVG") return BVG;
    else return ALC;
}
