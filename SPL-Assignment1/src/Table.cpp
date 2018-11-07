#include "../include/Table.h"
#include <iostream>
#include <vector>

//constructor
Table::Table(int t_capacity): capacity(t_capacity), open(false) {}

//rule of 5
void Table:: copy(const Table& rhs){
    capacity=rhs.getCapacity();
    open = rhs.status();
    for(size_t i=0;i<rhs.getCurrentSize();i++){
        customersList.push_back(rhs.customersList[i]->clone());
    }
    for(auto &i : orderList)
        orderList.emplace_back(i.first,i.second);
}
void Table:: clear(){
    for (auto &i : customersList) {
        delete i;
    }
    customersList.clear();
    orderList.clear();
}
void Table::steal(Table& other){
    capacity = other.capacity;
    open=other.status();
    customersList=other.customersList;
    for(auto &i : orderList)
        orderList.emplace_back(i.first,i.second);
    other.customersList.clear();
    other.getOrders().clear();
}
//distractor
Table::~Table(){
    clear();
}
//copy constructor
Table::Table(const Table& rhs){
    copy(rhs);
}
//assignment operator
Table& Table::operator=(const Table& rhs) {
    if (&rhs != this) {
        clear();
        copy(rhs);
    }
    return *this;
}
//move copy constructor
Table::Table(Table&& other){
    steal(other);
}
//move assignment operator
Table& Table:: operator=(Table&& other){
    clear();
    steal(other);
    return *this;
}
//methods
int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Table::removeCustomer(int id) {
    for (size_t i = 0; i < customersList.size(); i++) {
        if ((*customersList[i]).getId() == id)
            customersList.erase(customersList.begin()+i);
    }
}

int Table::getCurrentSize() const {
    return static_cast<int>(customersList.size());
}

Customer* Table::getCustomer(int id) {
    Customer* ans = nullptr;
    for (auto &i : customersList) {
        if ((*i).getId() == id)
            ans = i;
    }
    return ans;
}

std::vector<Customer *>& Table::getCustomers() {
    std::vector < Customer * > &ans = customersList;
    return ans;
}

std::vector<OrderPair>& Table::getOrders() {
    std::vector <OrderPair> &ans = orderList;
    return ans;
}

void Table::order(const std::vector<Dish> &menu) {
    std::vector<int> v;
    std::string s;
    for (auto &i : customersList) {
        v = (*i).order(menu);
        for (int j : v) {
            s += i->getName() + " ordered" + menu[j].getName() + "/n";
            orderList.push_back(OrderPair(i->getId(), menu[j]));
        }
    }
    std::cout << s << std::endl;
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    orderList.clear();
    Customer *c;
    for(auto &customer : customersList){
        c = customer;
        delete c;
    }
    customersList.clear();
    open = false;
}

int Table::getBill() {
    int bill = 0;
    for (auto &i : orderList) {
        bill += i.second.getPrice();
    }
    return bill;
}

bool Table::isOpen() {
    return open;
}

std::vector<OrderPair> Table::removeOrders(int id){
    std::vector<OrderPair> idOrders;
    for(size_t i = 0; i< orderList.size(); i++){
        if(orderList[i].first == id){
            idOrders.push_back(OrderPair(id,orderList[i].second));
            orderList.erase(orderList.begin()+i);
            i--;
        }
    }
    return idOrders;
}

void Table::addOrder(OrderPair p){
    orderList.push_back(p);
}
bool Table::status() const{
    return open;
}
