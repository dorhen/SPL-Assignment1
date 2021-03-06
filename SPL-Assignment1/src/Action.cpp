#include "../include/Action.h"
#include "../include/Table.h"
#include <iostream>

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


//OpenTable

//Constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id), customers(customersList){}
//Methods
void OpenTable::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(this->tableId);
    if (!t || !(t->isOpen()))
        this->error("Table does not exist or is already open");
    else{
        t->openTable();
        for(size_t i = 0; i < t->getCustomers().size() ; i++)
            t->addCustomer(t->getCustomers()[i]);
        complete();
    }
}
std::string OpenTable::toString() const {
    if(this->getStatus() == 2)
        std::cout << "open " <<  this->getArgs() << " Error: " << this->getErrorMsg() << std::endl;
    else
        std::cout << "open " << this->getArgs() << "Completed" << std::endl;
}


//Order

//Constructor
Order::Order(int id) : BaseAction(), tableId(id){}
//Methods
void Order::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(this->tableId);
        if (!t || !(t->isOpen()))
            this->error("Table does not exist or is already open");
        else {
            t->order(restaurant.getMenu());
            complete();
        }

}
std::string Order::toString() const {
    if(this->getStatus() == 2)
        std::cout << "order " <<  this->getArgs() << " Error: " << this->getErrorMsg() << std::endl;
    else
        std::cout << "order " << this->getArgs() << "Completed" << std::endl;
}


//MoveCustomer

//Constructort 
MoveCustomer::MoveCustomer(int src, int dst, int customerId) :BaseAction(), srcTable(src), dstTable(dst), id(customerId){}
//Methods
void MoveCustomer::act(Restaurant &restaurant) {
    Table *t1 = restaurant.getTable(this->srcTable);
    Table *t2 = restaurant.getTable(this->dstTable);
    if(!t1 || !(t1->isOpen()) || !t2 || !(t2->isOpen()) || t2->getCapacity() == t2->getCurrentSize() || !t1->getCustomer(this->id))
        this->error("Cannot move customer");
    else {
        t2.addCustomer(t1.getCustomer(this->id));
        std::vector<OrderPair> toMove = t1->removeOrders(this->id);
        for (const auto &i : toMove)
            t2->addOrder(i);
        t1->removeCustomer(this->id);
        complete();
    }
}
std::string MoveCustomer::toString() const {
    if(this->getStatus() == 2)
        std::cout << "move " <<  this->getArgs() << " Error: " << this->getErrorMsg() << std::endl;
    else
        std::cout << "move " << this->getArgs() << "Completed" << std::endl;
}


//Close

//Constructor
Close::Close (int id): BaseAction(), tableId(id) {}
//Methods
void Close::act(Restaurant &restaurant){
    Table *t = restaurant.getTable(this->tableId);
    if (!t || !(t->isOpen()))
        this->error("Table does not exist or is not open");
    else{

        std::cout << "Table " << this->tableId << "was closed. " << "Bill " << t->getBill() << "NIS" << std::endl;
        t->closeTable();
        complete();
    }
}
std::string Close::toString() const {
    if(this->getStatus() == 2)
        std::cout << "close " <<  this->getArgs() << " Error: " << this->getErrorMsg() << std::endl;
    else
        std::cout << "close " << this->getArgs() << "Completed" << std::endl;
}


//CloseAll

//Constructor
CloseAll::CloseAll(): BaseAction() {}
//Methods
void CloseAll::act(Restaurant &restaurant) {
    Table *t = restaurant.getTable(0);
    int size = restaurant.getNumOfTables();
    Close *c;
    for (size_t i = 0; i < size ; i++){
        if(!(t->isOpen()))continue;
        else{
            c = new Close((int)i);
            c->act(restaurant);
            delete c;
        }
    }
    complete();
}
std::string CloseAll::toString() const {
    std::cout << "closeall" << " Completed" << std::endl;
}


//PrintMenu

//Constructor
PrintMenu::PrintMenu(): BaseAction() {}
//Methods
void PrintMenu::act(Restaurant &restaurant) {
    std::vector<Dish> menu = restaurant.getMenu();
    for (auto &i : menu)
        i.toString();
    complete();
}
std::string PrintMenu::toString() const {
    std::cout << "menu" << " Completed" <<  std::endl;
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
        std::cout << "Table " << tableId << " status: open" << std::endl;
        for (size_t i = 0; i < t->getCurrentSize(); i++)
            (t->getCustomer((int) i))->toString();
        std::vector<OrderPair> orders = t->getOrders();
        for (auto &order : orders)
            std::cout << order.second.getName() << " " << order.second.getPrice() << "NIS " << order.first << std::endl;
        complete();
    }
}
std::string PrintTableStatus::toString() const {
    std::cout << "menu" << this->getArgs() << " Completed" <<  std::endl;
}


//PrintActionsLog

//Constructor
PrintActionsLog::PrintActionsLog(): BaseAction(){}
//Methods
void PrintActionsLog::act(Restaurant &restaurant) {
    std::vector<BaseAction*> acts = restaurant.getActionsLog();
    for (auto &act : acts) {
        act->toString();
    }
    complete();
}
std::string PrintActionsLog::toString() const {
    std::cout << "log" << " Completed" <<  std::endl;
}
