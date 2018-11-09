
#include <iostream>
#include "../include/Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), price(d_price), type(d_type){}

int Dish::getId() const{
    return id;
}
std::string Dish::getName() const{
    return name;
}
int Dish::getPrice() const{
    return price;
}
DishType Dish::getType() const{
    return type;
}
std::string Dish::toString() const {
    return name + " " + typeToString() + " " +std::to_string(price) + "NIS";
}
std::string Dish::typeToString() const {
    if(type == 0)return "VEG";
    else if(type == 1)return "SPC";
    else if(type == 2)return "BVG";
    else return "ALC";
}
