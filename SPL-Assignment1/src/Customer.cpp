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
Customer* Customer::clone() {
    return nullptr;
}

std::vector<int> VegetarianCustomer::strategy;
std::vector<int> VegetarianCustomer::ans;

VegetarianCustomer :: VegetarianCustomer(std::string name, int id): Customer(name, id){
}
std::vector<int> VegetarianCustomer :: order(const std::vector<Dish> &menu){
    if((strategy.size() == 0)){
        int expBVG = -1;
        strategy.push_back(-1);
        strategy.push_back(-1);
        for(int i=0 ; i <  static_cast<int>(menu.size()) ; i++){
            if ((strategy[0]==-1) && (menu[i].getType() == VEG))
                strategy[0] = i;
            if ((menu[i].getType() == BVG) && (menu[i].getPrice() > expBVG)){
                strategy[1] = i;
                expBVG = menu[i].getPrice();
            }
        }
        if((strategy[1] == -1)  || (strategy[0] == -1))
            return ans;
        ans.push_back(strategy[0]);
        ans.push_back(strategy[1]);
    }
    return ans;
}

std::string VegetarianCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
Customer* VegetarianCustomer:: clone(){
    VegetarianCustomer* c=new VegetarianCustomer(getName(), getId());
    return c;
}


std::vector<int> CheapCustomer::strategy;
CheapCustomer :: CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false){
}
void CheapCustomer:: setBool(bool t){
    ordered=t;
}
std::vector<int> CheapCustomer :: order(const std::vector<Dish> &menu){
    if((!ordered) && (!menu.empty())){
        int cheapest = -1;
        strategy.push_back(-1);
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
    CheapCustomer* c=new CheapCustomer(getName(), getId());
    c->setBool(ordered);
    return c;
}



std::vector<int> SpicyCustomer::strategy;
SpicyCustomer :: SpicyCustomer(std::string name, int id) : Customer(name, id), ordered(false) {
    ans.push_back(-1);
}
void SpicyCustomer:: setBool(bool t){
    ordered=t;
}
std::vector<int> SpicyCustomer :: order(const std::vector<Dish> &menu){
    if(ordered) {
        ans[0]=strategy[1];
        return ans;
    }
    if(ans.size()==0)
        return ans;
    if(strategy.size()==0) {
        strategy.push_back(-1);
        strategy.push_back(-1);
        int exp = 0;
        int cheapest = -1;
        for (int i = 0; i < static_cast<int>(menu.size()); i++) {
            if ((menu[i].getType() == SPC) && (menu[i].getPrice() > exp)) {
                exp = menu[i].getPrice();
                strategy[0] = i;
            }
            if ((menu[i].getPrice() < cheapest) || (cheapest == -1)) {
                cheapest = menu[i].getPrice();
                strategy[1] = i;
            }
        }
        if (cheapest == -1 || exp == 0) {
            ans.clear();
            return ans;
        }
    }
    ans[0]=strategy[0];
    ordered = true;
    return ans;
}
std::string SpicyCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
Customer* SpicyCustomer:: clone(){
    SpicyCustomer* c=new SpicyCustomer(getName(), getId());
    c->setBool(ordered);
    return c;
}



std::vector<int> AlchoholicCustomer::strategy;
AlchoholicCustomer :: AlchoholicCustomer(std::string name, int id) : Customer(name, id), current(0) {
    ans.push_back(1);
}
void AlchoholicCustomer :: Sort(){
    std::vector<int> Sorted;
    int min;
    std::vector<Dish> &ref = Alcs;
    while(!Alcs.empty()){
        min=findMinimum(ref);
        Sorted.push_back(strategy[min]);
        erase(Alcs,min);
        strategy.erase(strategy.begin()+min);
    }
    for(size_t i=0 ; i<Sorted.size();i++){
        strategy.push_back(Sorted[i]);
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
    if(strategy.size()==0){
        for (int i = 0; i < static_cast<int>(menu.size()); i++) {
            if (menu[i].getType() == ALC) {
                Alcs.push_back(menu[i]);
                strategy.push_back(i);
            }
        }
        Sort();
    }
    if(strategy.size()==0){
        strategy.push_back(-1);
        ans.clear();
        return ans;
    }
    if(static_cast<int>(strategy.size())>current){
        ans[0]=strategy[current];
        current++;
        return ans;
    }
    ans.clear();
    return ans;
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
