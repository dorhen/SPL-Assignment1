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

void Restaurant::start(){
    std::cout << "Restaurant is now open!" << std::endl;
    std::string command;
    std::string action;
    std::string args;
    Restaurant &me=*this;
    cin >> command;
    action = command.substr(0,command.find_first_of(" "));
    command = command.substr(command.find_first_of(" ")+1,command.length());
    args = command;
    while (action !="closeall"){
        if(action == "open"){
            int t_id = std::stoi(command.substr(0,command.find_first_of(" ")));
            int c_id=0;
            std::vector<Customer *> customersList;
            std::string name;
            std::string strategy;
            command = command.substr(command.find_first_of(" ")+1, command.length());
            while(command.length()>0){
                name = command.substr(0,command.find_first_of(","));
                command = command.substr(command.find_first_of(",")+1,command.length());
                strategy = command.substr(0,command.find_first_of(' '));
                command = command.substr(command.find_first_of(" ")+1,command.length());
                if(strategy == "veg")
                    customersList.push_back(new VegetarianCustomer(name,c_id));
                else if(strategy == "chp")
                    customersList.push_back(new CheapCustomer(name,c_id));
                else if(strategy == "spc")
                    customersList.push_back(new SpicyCustomer(name,c_id));
                else if(strategy == "alc")
                    customersList.push_back(new AlchoholicCustomer(name,c_id));
                c_id++;
            }
            OpenTable *OT = new OpenTable(t_id,customersList);
            OT->updateArgs(args);
            OT->act(me);
            actionsLog.push_back(OT);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
        }
        else if(action == "order"){
            int t_id = std::stoi(command.substr(0,command.length()));
            Order *O=new Order(t_id);
            O->updateArgs(args);
            O->act(me);
            actionsLog.push_back(O);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
        }
        else if(action == "move"){
            int ori =std::stoi(command.substr(0,command.find_first_of(" ")));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            int des =std::stoi(command.substr(0,command.find_first_of(" ")));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            int cus =std::stoi(command.substr(0,command.length()));
            MoveCustomer *MC=new MoveCustomer(ori,des,cus);
            MC->updateArgs(args);
            MC->act(me);
            actionsLog.push_back(MC);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
        }
        else if(action == "close"){
            int t_id =std::stoi(command.substr(0,command.length()));
            Close *C=new Close(t_id);
            C->updateArgs(args);
            C->act(me);
            actionsLog.push_back(C);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
        }
        else if(action == "menu"){
            PrintMenu *PM=new PrintMenu();
            PM->act(me);
            actionsLog.push_back(PM);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
        }
        else if(action == "status"){
            int t_id =std::stoi(command.substr(0,command.length()));
            PrintTableStatus *Stat=new PrintTableStatus(t_id);
            Stat->updateArgs(args);
            Stat->act(me);
            actionsLog.push_back(Stat);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
        }
        else if(action == "log"){
            PrintActionsLog *log=new PrintActionsLog();
            log->act(me);
            actionsLog.push_back(log);
            cin >> command;
            action = command.substr(0,command.find_first_of(" "));
            command = command.substr(command.find_first_of(" ")+1,command.length());
            args = command;
            continue;
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
