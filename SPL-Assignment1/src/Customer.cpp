#include <iostream>
#include "../include/Customer.h"

Customer :: Customer(std :: string c_name, int c_id) : name(c_name), id(c_id){}
std::vector<int> Customer::order(const std::vector<Dish> &menu) {
    std::vector<int> a;
    return a;
}
std::string Customer::toString() const {
    return "";
}
std::string Customer :: getName() const{
    return name;
}
int Customer :: getId() const{
    return id;
}
//we added those
Customer::~Customer(){ //a virtual D'ctor that will be overload
}
Customer* Customer::clone() {//virtual fuction that will be overload
    return nullptr;
}

//static variables declaration
std::vector<int> VegetarianCustomer::strategy;
std::vector<int> VegetarianCustomer::ans;

VegetarianCustomer :: VegetarianCustomer(std::string name, int id): Customer(name, id){
}
std::vector<int> VegetarianCustomer :: order(const std::vector<Dish> &menu){
    if((strategy.size() == 0)){//if startegy didnt initialized yet
        int expBVG = -1;
        strategy.push_back(-1);
        strategy.push_back(-1);//change strategy size that it wont get in the 'if' again
        for(int i=0 ; i <  static_cast<int>(menu.size()) ; i++){
            if ((strategy[0]==-1) && (menu[i].getType() == VEG))//fing first VEG dish
                strategy[0] = i;
            if ((menu[i].getType() == BVG) && (menu[i].getPrice() > expBVG)){//fing most expensive non alcoholic drink
                strategy[1] = i;
                expBVG = menu[i].getPrice();
            }
        }
        if((strategy[1] == -1)  || (strategy[0] == -1))//if one of them is not exist dont order at all
            return ans;
        ans.push_back(strategy[0]);//change ans for furthere order from a veg customer
        ans.push_back(strategy[1]);
    }
    return ans;
}

std::string VegetarianCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
//we added that
Customer* VegetarianCustomer:: clone(){//make a copy of 'this'. will be called only to copy tables, and the table class will be responsible for this memory
    VegetarianCustomer* c=new VegetarianCustomer(getName(), getId());
    return c;
}

//static variable declaration
std::vector<int> CheapCustomer::strategy;
CheapCustomer :: CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false){
}
std::vector<int> CheapCustomer :: order(const std::vector<Dish> &menu){
    if((!ordered) && (!menu.empty())){//this customer orders only once, so need to check if he already ordered and if there is no dish then he wont order
        int cheapest = -1;
        strategy.push_back(-1);
        for(int i=0 ; i < static_cast<int>(menu.size()) ; i++){//find the cheapest dish
            if((menu[i].getPrice() < cheapest) || (cheapest == -1)){
                cheapest = menu[i].getPrice();
                strategy[0] = i;
            }
        }
        ordered=true;
        return strategy;
    }
    strategy.clear();//if he already ordered so he wont order again
    return strategy;
}
std::string CheapCustomer :: toString() const {
    return std::to_string(this->getId()) + " " + this->getName();
}
//we added those
Customer* CheapCustomer:: clone(){//make a copy of 'this'. will be called only to copy tables, and the table class will be responsible for this memory
    CheapCustomer* c=new CheapCustomer(getName(), getId());
    c->setBool(ordered);
    return c;
}
void CheapCustomer:: setBool(bool t){
    ordered=t;
}


//static variable declaration
std::vector<int> SpicyCustomer::strategy;
SpicyCustomer :: SpicyCustomer(std::string name, int id) : Customer(name, id), ordered(false) {
    ans.push_back(-1);
}
std::vector<int> SpicyCustomer :: order(const std::vector<Dish> &menu){
    if(ordered) {//if he already ordered so need to order only BVG
        if (strategy[1]==-1) {//if there is non BVG he wont order
            ans.clear();
            return ans;
        }
        ans[0]=strategy[1];
        return ans;
    }
    if(ans.size()==0)//if ans been cleared so there is no option for another order
        return ans;
    if(strategy.size()==0) {// if strategy didnt initialized need to find possible orders
        strategy.push_back(-1);
        strategy.push_back(-1);
        int exp = 0;
        int cheapest = -1;
        for (int i = 0; i < static_cast<int>(menu.size()); i++) {
            if ((menu[i].getType() == SPC) && (menu[i].getPrice() > exp)) {//find most expensive SPC dish
                exp = menu[i].getPrice();
                strategy[0] = i;
            }
            if (((menu[i].getPrice() < cheapest) || (cheapest == -1)) && (menu[i].getType() == BVG)) {//fing cheapest BVG
                cheapest = menu[i].getPrice();
                strategy[1] = i;
            }
        }
        if (exp == 0) {//if no SPC dish he wont order
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
//we added those
Customer* SpicyCustomer:: clone(){//make a copy of 'this'. will be called only to copy tables, and the table class will be responsible for this memory
    SpicyCustomer* c=new SpicyCustomer(getName(), getId());
    c->setBool(ordered);
    return c;
}
void SpicyCustomer:: setBool(bool t){
    ordered=t;
}


//static variable declaration
std::vector<int> AlchoholicCustomer::strategy;
AlchoholicCustomer :: AlchoholicCustomer(std::string name, int id) : Customer(name, id), current(0) {
    ans.push_back(1);
}
std::vector<int> AlchoholicCustomer :: order(const std::vector<Dish> &menu){
    if(strategy.size()==0){// if strategy didnt initialized need to find possible orders
        for (int i = 0; i < static_cast<int>(menu.size()); i++) {
            if (menu[i].getType() == ALC) {//find all the ALC dishes
                Alcs.push_back(menu[i]);
                strategy.push_back(i);
            }
        }
        Sort();//sorting them
    }
    if(strategy.size()==0){//if there is no ALC drinks
        strategy.push_back(-1);//to dont get in the loop again
        ans.clear();
        return ans;//no order
    }
    if(static_cast<int>(strategy.size())>current){//while there is more drinks to order, order and point to the next one
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

//we added those
void AlchoholicCustomer :: Sort(){//sort the alc dishes by price by selection sort
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
Customer* AlchoholicCustomer:: clone(){//make a copy of 'this'. will be called only to copy tables, and the table class will be responsible for this memory
    Customer* c=new AlchoholicCustomer(getName(), getId());
    return c;
}
void AlchoholicCustomer::erase(std::vector<Dish> &listRef, int index) {//compiler didnt know how to compare dishes, and therefor vector.erase didnt work
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
