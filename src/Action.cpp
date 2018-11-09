#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"
#include <iostream>
extern Restaurant* backup;

//BaseAction

//Constructor
BaseAction::BaseAction() : errorMsg(""), status(PENDING), argsDelivered(""){}
//Methods
ActionStatus BaseAction::getStatus() const{
    return this->status;
}
void BaseAction::updateArgs(std::string args) {
    this->argsDelivered = args;
}
void BaseAction::complete() {
    this->status = COMPLETED;
}
void BaseAction::error(std::string errorMsg){
    this->errorMsg = errorMsg;
    this->status = ERROR;
}
std::string BaseAction::getArgs() const{
    return this->argsDelivered;
}
std::string BaseAction::getErrorMsg() const {
    return this->errorMsg;
}
BaseAction* BaseAction::clone() const{
    return nullptr;
}
std::string BaseAction::toString() const {
    return "";
}
BaseAction::~BaseAction(){
}
void BaseAction::act(Restaurant &restaurant) {
}



//OpenTable

//Constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id), customers(customersList){}
//Methods
void OpenTable::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(this->tableId);
    int ocs=0;
    for (char i : getArgs())
        if(i == ',') ocs++;
    if (!t || t->isOpen() || ocs > t->getCapacity()) {
        this->error("Table does not exist or is already open");
        std::cout << "Error: Table does not exist or is already open" << std::endl;
    }
    else{
        t->openTable();
        for(size_t i = 0; i < customers.size() ; i++)
            t->addCustomer(customers[i]);
        complete();
    }
}
std::string OpenTable::toString() const {
    if(this->getStatus() == 2)
        return "open " +  this->getArgs() + " Error: " + this->getErrorMsg();
    else
        return "open " + this->getArgs() + " Completed";
}
BaseAction* OpenTable::clone() const{
    std::vector<Customer *> temp = customers;
    std::vector<Customer *> &ref = temp;
    BaseAction *a = new OpenTable(tableId,ref);
    return a;
}


//Order

//Constructor
Order::Order(int id) : BaseAction(), tableId(id){}
//Methods
void Order::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(this->tableId);
        if (!t || !(t->isOpen())) {
            this->error("Table does not exist or is already open");
            std::cout << "Error: Table does not exist or is already open" << std::endl;
        }
        else {
            t->order(restaurant.getMenu());
            complete();
        }

}
std::string Order::toString() const {
    if(this->getStatus() == 2)
        return "order " +  this->getArgs() + " Error: " + this->getErrorMsg();
    else
        return "order " + this->getArgs() + " Completed" ;
}
BaseAction* Order::clone() const{
    BaseAction *a = new Order(tableId);
    return a;
}


//MoveCustomer

//Constructort 
MoveCustomer::MoveCustomer(int src, int dst, int customerId) :BaseAction(), srcTable(src), dstTable(dst), id(customerId){}
//Methods
void MoveCustomer::act(Restaurant &restaurant) {
    Table *t1 = restaurant.getTable(this->srcTable);
    Table *t2 = restaurant.getTable(this->dstTable);
    if(!t1 || !(t1->isOpen()) || !t2 || !(t2->isOpen()) || t2->getCapacity() == t2->getCurrentSize() || !t1->getCustomer(this->id)) {
        this->error("Cannot move customer");
        std::cout << "Error: Cannot move customer" << std::endl;

    }
    else {
        t2->addCustomer(t1->getCustomer(this->id));
        std::vector<OrderPair> toMove = t1->removeOrders(this->id);
        for (const auto &i : toMove)
            t2->addOrder(i);
        t1->removeCustomer(this->id);
        complete();
    }
}
std::string MoveCustomer::toString() const {
    if(this->getStatus() == 2)
        return "move " +  this->getArgs() + " Error: " + this->getErrorMsg();
    else
        return "move " + this->getArgs() + " Completed" ;
}
BaseAction* MoveCustomer::clone() const{
    BaseAction *a = new MoveCustomer(srcTable,dstTable,id);
    return a;
}


//Close

//Constructor
Close::Close (int id): BaseAction(), tableId(id) {}
//Methods
void Close::act(Restaurant &restaurant){
    Table *t = restaurant.getTable(this->tableId);
    if (!t || !(t->isOpen())) {
        this->error("Table does not exist or is not open");
        std::cout << "Error: Table does not exist or is already open" << std::endl;

    }
    else{

        std::cout << "Table " << this->tableId << " was closed. " << "Bill " << t->getBill() << "NIS" << std::endl;
        t->closeTable();
        complete();
    }
}
std::string Close::toString() const {
    if(this->getStatus() == 2)
        return "close " +  this->getArgs() + " Error: " + this->getErrorMsg();
    else
        return "close " + this->getArgs() + " Completed";
}
BaseAction* Close::clone() const{
    BaseAction *a = new Close(tableId);
    return a;
}


//CloseAll

//Constructor
CloseAll::CloseAll(): BaseAction() {}
//Methods
void CloseAll::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(0);
    int size = restaurant.getNumOfTables();
    for (int i = 0; i < size ; i++){
        if(t->isOpen()){
            Close c(i);
            c.act(restaurant);
        }
        t = restaurant.getTable(i+1);
    }
    complete();
}
std::string CloseAll::toString() const {
    return "closeall Completed";
}
BaseAction* CloseAll::clone() const{
    BaseAction *a = new CloseAll();
    return a;
}


//PrintMenu

//Constructor
PrintMenu::PrintMenu(): BaseAction() {}
//Methods
void PrintMenu::act(Restaurant &restaurant) {
    std::vector<Dish> menu = restaurant.getMenu();
    for (auto &i : menu)
        std::cout << i.toString() << std::endl;
    complete();
}
std::string PrintMenu::toString() const {
    return "menu Completed";
}
BaseAction* PrintMenu::clone() const{
    BaseAction *a = new PrintMenu();
    return a;
}


//PrintTableStatus

//Constructor
PrintTableStatus::PrintTableStatus(int id): BaseAction(), tableId(id) {}
//Methods
void PrintTableStatus::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(this->tableId);
    if((!t->isOpen())){
        std::cout << "Table " << tableId << " status: close" << std::endl;
        complete();
    }
    else {
        std::cout << "Table " << tableId << " status: open" <<  std::endl;
        std::cout << "Customers:" <<  std::endl;
        std::vector<Customer *> ref = t->getCustomers();
        for (const auto &i : t->getCustomers())
            std::cout << i->toString() << std::endl;
        std::vector<OrderPair> orders = t->getOrders();
        std::cout << "Orders:" <<  std::endl;
        for (auto &order : orders)
            std::cout << order.second.getName() << " " << order.second.getPrice() << "NIS " << order.first << std::endl;
        std::cout << "Current Bill: " << std::to_string(t->getBill()) << "NIS" << std::endl;
        complete();
    }
}
std::string PrintTableStatus::toString() const {
    return "status " + this->getArgs() + " Completed";
}
BaseAction* PrintTableStatus::clone() const{
    BaseAction *a = new PrintTableStatus(tableId);
    return a;
}


//PrintActionsLog

//Constructor
PrintActionsLog::PrintActionsLog(): BaseAction(){}
//Methods
void PrintActionsLog::act(Restaurant &restaurant) {
    std::vector<BaseAction*> acts = restaurant.getActionsLog();
    for (auto &act : acts) {
        std::cout << act->toString() << std::endl;
    }
    complete();
}
std::string PrintActionsLog::toString() const {
    return "log Completed";
}
BaseAction* PrintActionsLog::clone() const{
    BaseAction *a = new PrintActionsLog();
    return a;
}

//Backup

//Constructor
BackupRestaurant::BackupRestaurant(): BaseAction() {}
//Methods
void BackupRestaurant::act(Restaurant &restaurant) {
    backup = new Restaurant(restaurant);
    complete();
}
std::string BackupRestaurant::toString() const {
    return "backup Completed";
}
BaseAction* BackupRestaurant::clone() const{
    BaseAction *a = new BackupRestaurant();
    return a;
}

//Restore
//Constructor
RestoreResturant::RestoreResturant(): BaseAction(){}
//Methods
void RestoreResturant::act(Restaurant &restaurant) {
    if(backup == nullptr){
        this->error("No backup avilable");
        std::cout << "No backup avilable" << std::endl;
    }
    else {
        restaurant = *backup;
        complete();
    }
}
std::string RestoreResturant::toString() const {
    return "restore Completed";
}
BaseAction* RestoreResturant::clone() const{
    return nullptr;
}
