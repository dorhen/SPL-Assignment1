#include "../include/Restaurant.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Restaurant::Restaurant(): open(true), tables(), menu(), actionsLog() {}

Restaurant::Restaurant(const string &configFilePath):Restaurant(){
    ifstream inFile;
    inFile.open(configFilePath);
    if(!inFile.is_open()){
        exit(1);
    }
    string line;
    int count = 0;
    int dishID = 0;
    while(!inFile.eof()){
        getline(inFile,line);
        if(line.empty() || line[0] == '\r')
            continue;
        else if(line[0] == '#'){
            count++;
            continue;
        }
        if(count == 1) {

            stringstream num(line);
            int numOfTables = 0;
            num >> numOfTables;
            vector<int> t(numOfTables);
        }
        else if(count == 2) {
            while (line.size()) {
                if (static_cast<int>(line.find(',')) != -1) {
                    string numWord = line.substr(0, line.find(','));
                    line = line.substr(line.find(',') + 1);
                    stringstream num(numWord);
                    int tSize = 0;
                    num >> tSize;
                    this->tables.push_back(new Table(tSize));
                } else {
                    stringstream num(line);
                    int tSize = 0;
                    num >> tSize;
                    this->tables.push_back(new Table(tSize));
                    line = "";
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
    getline(cin,command);
    action = command.substr(0,command.find_first_of(' '));
    command = command.substr(command.find_first_of(' ')+1,command.length());
    args = command;
    int c_id=0;
    std::string &cRef = command;
    std::string &aRef = action;
    std::string &argsRef = args;
    while (action !="closeall"){
        if(action == "open"){
            int t_id = std::stoi(command.substr(0,command.find_first_of(' ')));
            std::vector<Customer *> customersList;
            int ocs=0;
            for (char i : command)
                if(i == ',') ocs++;
            if(t_id < getNumOfTables() && !getTable(t_id)->isOpen() && getTable(t_id)->getCapacity() >= ocs ) {
                std::string name;
                std::string strategy;
                command = command.substr(command.find_first_of(' ') + 1, command.length());
                while (command.length() > 3) {
                    name = command.substr(0, command.find_first_of(','));
                    command = command.substr(command.find_first_of(',') + 1, command.length());
                    strategy = command.substr(0, command.find_first_of(' '));
                    command = command.substr(command.find_first_of(' ') + 1, command.length());
                    if (strategy == "veg")
                        customersList.push_back(new VegetarianCustomer(name, c_id));
                    else if (strategy == "chp")
                        customersList.push_back(new CheapCustomer(name, c_id));
                    else if (strategy == "spc")
                        customersList.push_back(new SpicyCustomer(name, c_id));
                    else if (strategy == "alc")
                        customersList.push_back(new AlchoholicCustomer(name, c_id));
                    c_id++;
                }
            }
            BaseAction *ot = new OpenTable(t_id,customersList);
            update(cRef,ot,argsRef,aRef);
            /*
            OT->updateArgs(args);
            OT->act(me);
            actionsLog.push_back(OT);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;
             */
            continue;
        }
        else if(action == "order"){
            int t_id = std::stoi(command.substr(0,command.length()));
            BaseAction *o=new Order(t_id);
            update(cRef,o,argsRef,aRef);
            /*
            O->updateArgs(args);
            O->act(me);
            actionsLog.push_back(O);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;
             */
            continue;
        }
        else if(action == "move"){
            int ori =std::stoi(command.substr(0,command.find_first_of(' ')));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            int des =std::stoi(command.substr(0,command.find_first_of(' ')));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            int cus =std::stoi(command.substr(0,command.length()));
            BaseAction *mc=new MoveCustomer(ori,des,cus);
            update(cRef,mc,argsRef,aRef);
            /*
            MC->updateArgs(args);
            MC->act(me);
            actionsLog.push_back(MC);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;
             */
            continue;
        }
        else if(action == "close"){
            int t_id =std::stoi(command.substr(0,command.length()));
            BaseAction *c=new Close(t_id);
            update(cRef,c,argsRef,aRef);
            /*
            C->updateArgs(args);
            C->act(me);
            actionsLog.push_back(C);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;
            continue;
             */
        }
        else if(action == "menu"){
            BaseAction *pm=new PrintMenu();
            update(cRef,pm,argsRef,aRef);

            /*
            PM->act(me);
            actionsLog.push_back(PM);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;*/
            continue;
        }
        else if(action == "status"){
            int t_id =std::stoi(command.substr(0,command.length()));
            BaseAction *stat=new PrintTableStatus(t_id);
            update(cRef,stat,argsRef,aRef);
            /*
            Stat->updateArgs(args);
            Stat->act(me);
            actionsLog.push_back(Stat);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;*/
            continue;
        }
        else if(action == "log"){
            BaseAction *log=new PrintActionsLog();
            update(cRef,log,argsRef,aRef);
            /*
            log->act(me);
            actionsLog.push_back(log);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;
            continue;
             */
        }
        else if(action == "backup"){
            BaseAction *bu= new BackupRestaurant();
            update(cRef,bu,argsRef,aRef);
            /*
            BU->act(me);
            actionsLog.push_back(BU);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;*/
            continue;
        }
        if(action == "restore"){
            BaseAction *rr= new RestoreResturant();
            update(cRef,rr,argsRef,aRef);
            /*RR->act(me);
            getline(cin,command);
            action = command.substr(0,command.find_first_of(' '));
            command = command.substr(command.find_first_of(' ')+1,command.length());
            args = command;*/
            continue;
        }
    }
    BaseAction *CA= new CloseAll();
    CA->act(me);
}

void Restaurant::update(std::string &command, BaseAction *ba, std::string &args, std::string &action){
    Restaurant &restRef = *this;
    ba->updateArgs(args);
    this->actionsLog.push_back(ba);
    ba->act(restRef);
    getline(cin,command);
    action = command.substr(0,command.find_first_of(' '));
    command = command.substr(command.find_first_of(' ')+1,command.length());
    args = command;
}


int Restaurant::getNumOfTables() const{
    return (int)tables.size();
}

vector<Dish>& Restaurant::getMenu(){
    vector<Dish> &dishRef = menu;
    return dishRef;
}

Table* Restaurant::getTable(int ind){
    if(ind>= static_cast<int>(tables.size()))return nullptr;
    return tables[ind];
}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const{
    const std::vector<BaseAction*>& ref = actionsLog;
    return ref;
}

DishType Restaurant::convert(string str){
    if(str == "VEG") return VEG;
    else if(str == "SPC") return SPC;
    else if(str == "BVG") return BVG;
    else return ALC;
}


//Rule of 5
//Copy
void Restaurant::copy(const Restaurant& other){
    open = other.open;
    for(int i = 0; i< static_cast<int>(other.tables.size()); i++)
        tables.push_back(new Table(*other.tables[i]));
    for (const auto &i : other.menu)
        menu.emplace_back(i.getId(), i.getName(), i.getPrice(), i.getType());
    for(int i =0; i<static_cast<int>(other.actionsLog.size()); i++)
        actionsLog.push_back(other.actionsLog[i]->clone());
}
//Steal
void Restaurant::steal(Restaurant& other){
    open = other.open;
    tables = other.tables;
    for (const auto &i : other.menu)
        menu.emplace_back(i.getId(), i.getName(), i.getPrice(), i.getType());
    actionsLog = other.actionsLog;
}
//Clean
void Restaurant::clean(){
    for (auto &table : tables) {
        delete table;
    }
    tables.clear();
    menu.clear();
    for (auto &i : actionsLog) {
        delete i;
    }
    actionsLog.clear();
    open = false;
}
//D'tor
Restaurant::~Restaurant(){
    for (auto &table : tables) {
        delete table;
    }
    tables.clear();
    for (auto &i : actionsLog) {
        delete i;
    }
    actionsLog.clear();
}
//Deep copy c'tor
Restaurant::Restaurant(const Restaurant& rhs):open(), tables(), menu(), actionsLog(){
    this->copy(rhs);
}
//Copy assignment op'.
Restaurant& Restaurant::operator=(const Restaurant& rhs){
    if(&rhs != this)
    {
        this->clean();
        copy(rhs);
    }
    return *this;

}
//Move
Restaurant::Restaurant(Restaurant&& rhs):open(), tables(), menu(), actionsLog(){
    steal(rhs);
}
//Move assignment op'.
Restaurant& Restaurant::operator=(Restaurant&& rhs) {
    clean();
    steal(rhs);
    return *this;
}
