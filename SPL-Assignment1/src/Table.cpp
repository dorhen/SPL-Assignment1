#include "../include/Table.h"
#include <iostream>
#include <vector>


Table::Table(int t_capacity): capacity(t_capacity), open(false) {}

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
    return customersList.size();
}

Customer* Table::getCustomer(int id) {
    Customer* ans = nullptr;
    for (size_t i = 0; i < customersList.size(); i++) {
        if ((*customersList[i]).getId() == id)
            ans = customersList[i];
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
    for (size_t i = 0; i < customersList.size(); i++) {
        v = (*customersList[i]).order(menu);
        for (size_t j = 0; j < v.size(); j++) {
            s += customersList[i].getName() + " ordered" + menu[v[j]].getName() + "/n";
            orderList.push_back(OrderPair(customersList[i].getId(), menu[v[j]]));
        }
    }
    std::cout << s << std::endl;
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    orderList.clear();
    customersList.clear();
    open = false;
}

int Table::getBill() {
    int bill = 0;
    for (size_t i = 0; i < orderList.size(); i++) {
        bill += orderList[i].second.getPrice();
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
}

void Table::addOrder(OrderPair p){
    orderList.push_back(p);
}

