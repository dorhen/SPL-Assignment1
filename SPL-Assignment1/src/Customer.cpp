#include <iostream>
#include "../include/Customer.h"

Customer :: Customer(std :: string c_name, int c_id) : name(c_name), id(c_id){}


std::string Customer :: getName() const{
    return name;
}
int Customer :: getId() const{
    return id;
}



VegetarianCustomer :: VegetarianCustomer(std::string name, int id): Customer(name, id),  Strategy(-1,-1){
}
std::vector<int> VegetarianCustomer :: order(const std::vector<Dish> &menu){
    if((Strategy.size() == 2) && (Strategy[0] == -1)){
        int expBVG = -1;
        for(size_t i=0 ; i < menu.size() ; i++){
            if ((Strategy[0]==-1) && (menu[i].getType() == VEG))
                Strategy[0] = i;
            if ((menu[i].getType() == BVG) && (menu[i].getPrice() > expBVG)){
                Strategy[1] = i;
                expBVG = menu[i].getPrice();
            }
        }
        if((Strategy[1] == -1)  || (Strategy[0] == -1))
            Strategy.clear();
    }
    return Strategy;
}

std::string VegetarianCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}



CheapCustomer :: CheapCustomer(std::string name, int id) : Customer(name, id), Strategy(-1), ordered(false){}
std::vector<int> CheapCustomer :: order(const std::vector<Dish> &menu){
    if((!ordered) && (menu.size() > 0)){
        int cheapest = -1;
        for(size_t i=0 ; i < menu.size() ; i++){
            if((menu[i].getPrice() < cheapest) || (cheapest == -1)){
                cheapest = menu[i].getPrice();
                Strategy[0] = i;
            }
        }
        ordered=true;
        return Strategy;
    }
    Strategy.clear();
    return Strategy;
}
std::string CheapCustomer :: toString() const {
    return std::to_string(this->getId()) + " " + this->getName();
}




SpicyCustomer :: SpicyCustomer(std::string name, int id) : Customer(name, id), Strategy(-1), ordered(false) {}
std::vector<int> SpicyCustomer :: order(const std::vector<Dish> &menu){
    if((ordered) && (menu[Strategy[0]].getType() == BVG))
        return Strategy;
    else if(ordered){
        int cheapest=-1;
        for(size_t i=0 ; i < menu.size() ; i++){
            if(menu[i].getType() == BVG){
                if((menu[i].getPrice() < cheapest) || (cheapest == -1)){
                    cheapest = menu[i].getPrice();
                    Strategy[0] = i;
                }
            }
        }
        if(cheapest == -1){
            Strategy.clear();
            return Strategy;
        }
    }
    int exp=0;
    for(size_t i=0;i<menu.size();i++){
        if((menu[i].getType()==SPC) && (menu[i].getPrice()>exp)){
            exp = menu[i].getPrice();
            Strategy[0]=i;
        }
    }
    if(exp == 0)
        Strategy.clear();
    ordered = true;
    return Strategy;
}
std::string SpicyCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}




AlchoholicCustomer :: AlchoholicCustomer(std::string name, int id) : Customer(name, id), current(0) {}
void AlchoholicCustomer :: Sort(){
    std::vector<Dish> Sorted;
    int i=1;
    Sorted.resize(Drinks.size());
    Sorted[0]=Drinks[0];
    while(i<Drinks.size()){
        int j=i-1;
        while(Drinks[i].getPrice()<Sorted[j].getPrice()){
            Sorted[j+1]=Sorted[j];
            j--;
        }
        Sorted[j+1]=Drinks[i];
        i++;
    }
    Drinks = Sorted; //not sure
}
std::vector<int> AlchoholicCustomer :: order(const std::vector<Dish> &menu){
    if(current>0){
        if(current<Drinks.size()){
            Strategy[0]=Drinks[current].getId();
            current++;
        }
        else Strategy.clear();
        return Strategy;
    }
    for(size_t i=0 ; i < menu.size() ; i++){
        if(menu[i].getType()==ALC)
            Drinks.push_back(menu[i]);
    }
    Sort();
    Strategy.push_back(Drinks[current].getId());
    current++;
    return Strategy;
}

std::string AlchoholicCustomer :: toString() const{
    return std::to_string(this->getId()) + " " + this->getName();
}
