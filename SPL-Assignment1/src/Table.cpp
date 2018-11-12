#include "../include/Table.h"
#include <iostream>
#include <vector>

//constructor
Table::Table(int t_capacity): capacity(t_capacity), open(false), customersList(), orderList() {}
//methods
int Table::getCapacity() const {
    return capacity;
}
void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Table::removeCustomer(int id) {
    for (size_t i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id)
            customersList.erase(customersList.begin()+i);
    }
}
Customer* Table::getCustomer(int id) {
    Customer* ans = nullptr;//if there is no customer with the same id
    for (auto &i : customersList) {
        if ((*i).getId() == id)
            ans = i;
    }
    return ans;
}

std::vector<Customer *>& Table::getCustomers() {
    std::vector < Customer * > &ans = customersList;//need to return a reference
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
        v = (*i).order(menu);//take order from each customer
        for (int j : v) {
            s += i->getName() + " ordered " + menu[j].getName() + '\n';
            orderList.push_back(OrderPair(i->getId(), menu[j]));//save the orders
        }
    }
    if(!s.empty()) {
        s.erase(s.length() - 1);//removing the'/n' from the ens of s
        std::cout << s << std::endl;
    }
}
void Table::openTable() {
    open = true;
}
void Table::closeTable() {
    orderList.clear();
    Customer *c;
    for(auto &customer : customersList){
        c = customer;
        delete c;//free the memory of each customer and then closing the table.
        //we decided that class table will be responsible of the customers memory
    }
    customersList.clear();
    open = false;
}
int Table::getBill() {
    int bill = 0;
    for (auto &i : orderList) {
        bill += i.second.getPrice();//counting each order
    }
    return bill;
}
bool Table::isOpen() {
    return open;
}
//rule of 5
void Table:: copy(const Table& rhs){
    capacity=rhs.getCapacity();
    open = rhs.status();
    for(int i=0;i<rhs.getCurrentSize();i++){
        customersList.push_back(rhs.customersList[i]->clone());//making a copy of each customer, using inheritence of clone
    }
    for(auto &i : rhs.orderList)
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
Table::Table(const Table& rhs): capacity(), open(), customersList(), orderList(){
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
Table::Table(Table&& other): capacity(), open(), customersList(), orderList(){
    steal(other);
}
//move assignment operator
Table& Table:: operator=(Table&& other){
    clear();
    steal(other);
    return *this;
}
std::vector<OrderPair> Table::removeOrders(int id){
    std::vector<OrderPair> idOrders;
    for(size_t i = 0; i< orderList.size(); i++){
        if(orderList[i].first == id){//take all the orders from that id
            idOrders.emplace_back(id,orderList[i].second);
        }
    }
    std::vector<OrderPair> &ref = orderList;
    erase(ref,id);
    return idOrders;
}
int Table::getCurrentSize() const {
    return static_cast<int>(customersList.size());
}
void Table::addOrder(OrderPair p){
    orderList.push_back(p);
}
bool Table::status() const{
    return open;
}
void Table::erase(std::vector<OrderPair> &listRef, int id) {
    std::vector<OrderPair> temp;
    for(auto &i : listRef){
        if(i.first == id)//coping only the pairs we want
            continue;
        temp.emplace_back(i.first,i.second);
    }
    listRef.clear();
    for(auto &i : temp)//put them back in the vactor
        listRef.emplace_back(i.first,i.second);
}
