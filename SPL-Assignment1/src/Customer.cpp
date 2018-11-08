#include <iostream>
#include "../include/Customer.h"

Customer :: Customer(std :: string c_name, int c_id) : name(c_name), id(c_id){}


std::string Customer :: getName() const{
    return name;
}
int Customer :: getId() const{
    return id;
}
Customer::~Customer() {
}
std::string Customer::toString() const {
    return "";
}
std::vector<int> Customer::order(const std::vector<Dish> &menu) {
    std::vector<int> a;
    return a;
}
Customer* Customer::clone() const {
    return nullptr;
}


VegetarianCustomer :: VegetarianCustomer(std::string name, int id): Customer(name, id){
    strategy.push_back(-1);
    strategy.push_back(-1);
}
std::vector<int> VegetarianCustomer :: order(const std::vector<Dish> &menu){
    if((strategy.size() == 2) && (strategy[0] == -1)){
        int expBVG = -1;
        for(int i=0 ; i <  static_cast<int>(menu.size()) ; i++){
            if ((strategy[0]==-1) && (menu[i].getType() == VEG))
                strategy[0] = i;
            if ((menu[i].getType() == BVG) && (menu[i].getPrice() > expBVG)){
                strategy[1] = i;
                expBVG = menu[i].getPrice();
            }
        }
        if((strategy[1] == -1)  || (strategy[0] == -1))
            strategy.clear();
    }
    return strategy;
}

std::string VegetarianCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
Customer* VegetarianCustomer:: clone(){
    Customer* c=new VegetarianCustomer(getName(), getId());
    return c;
}



CheapCustomer :: CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false){
    strategy.push_back(-1);
}
std::vector<int> CheapCustomer :: order(const std::vector<Dish> &menu){
    if((!ordered) && (!menu.empty())){
        int cheapest = -1;
        for(int i=0 ; i < static_cast<int>(menu.size()) ; i++){
            if((menu[i].getPrice() < cheapest) || (cheapest == -1)){
                cheapest = menu[i].getPrice();
                strategy[0] = i;
            }
        }
        ordered=true;
        return strategy;
    }
    strategy.clear();
    return strategy;
}
std::string CheapCustomer :: toString() const {
    return std::to_string(this->getId()) + " " + this->getName();
}
Customer* CheapCustomer:: clone(){
    Customer* c=new CheapCustomer(getName(), getId());
    return c;
}




SpicyCustomer :: SpicyCustomer(std::string name, int id) : Customer(name, id), ordered(false) {
    strategy.push_back(-1);
}
std::vector<int> SpicyCustomer :: order(const std::vector<Dish> &menu){
    if((ordered) && (menu[strategy[0]].getType() == BVG))
        return strategy;
    else if(ordered){
        int cheapest=-1;
        for(int i=0 ; i <  static_cast<int>(menu.size()) ; i++){
            if(menu[i].getType() == BVG){
                if((menu[i].getPrice() < cheapest) || (cheapest == -1)){
                    cheapest = menu[i].getPrice();
                    strategy[0] = i;
                }
            }
        }
        if(cheapest == -1){
            strategy.clear();
            return strategy;
        }
    }
    int exp=0;
    for(int i=0;i< static_cast<int>(menu.size());i++){
        if((menu[i].getType()==SPC) && (menu[i].getPrice()>exp)){
            exp = menu[i].getPrice();
            strategy[0]=i;
        }
    }
    if(exp == 0)
        strategy.clear();
    ordered = true;
    return strategy;
}
std::string SpicyCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
Customer* SpicyCustomer:: clone(){
    Customer* c=new SpicyCustomer(getName(), getId());
    return c;
}




AlchoholicCustomer :: AlchoholicCustomer(std::string name, int id) : Customer(name, id), current(0) {}
void AlchoholicCustomer :: Sort(){
    std::vector<Dish> Sorted;
    int min;
    std::vector<Dish> &ref = Drinks;
    while(!Drinks.empty()){
        min=findMinimum(ref);
        Sorted.push_back(Drinks[min]);
        erase(Drinks,min);
    }
    for(size_t i=0 ; i<Sorted.size();i++){
        Drinks.push_back(Sorted[i]);
    }
}
int AlchoholicCustomer:: findMinimum(std::vector<Dish> &v){
    int min=v[0].getPrice();
    int id=0;
    for(int i=1;i< static_cast<int>(v.size());i++){
        if(v[i].getPrice()<min){
            min=v[i].getPrice();
            id=i;
        }
    }
    return id;
}
std::vector<int> AlchoholicCustomer :: order(const std::vector<Dish> &menu){
    if(current>0){
        if(current< static_cast<int>(Drinks.size())){
            strategy[0]=Drinks[current].getId();
            current++;
        }
        else strategy.clear();
        return strategy;
    }
    for(int i=0 ; i <  static_cast<int>(menu.size()) ; i++){
        if(menu[i].getType()==ALC)
            Drinks.push_back(menu[i]);
    }
    Sort();
    strategy.push_back(Drinks[current].getId());
    current++;
    return strategy;
}
std::string AlchoholicCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
Customer* AlchoholicCustomer:: clone(){
    Customer* c=new AlchoholicCustomer(getName(), getId());
    return c;
}
void AlchoholicCustomer::erase(std::vector<Dish> &listRef, int index) {
    std::vector<Dish> temp;
    for(int i = 0; i <  static_cast<int>(listRef.size()); i++){
        if(i == index)
            continue;
        temp.emplace_back(listRef[i].getId(),listRef[i].getName(),listRef[i].getPrice(),listRef[i].getType());
    }
    listRef.clear();
    for(auto &i : temp)
        listRef.emplace_back(i.getId(),i.getName(),i.getPrice(),i.getType());
}

