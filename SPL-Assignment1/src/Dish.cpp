
#include <iostream>
#include "../include/Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), price(d_price), type(d_type), name(d_name) {}

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
    std::cout<< name << " " << type << " " << price << "NIS" << std::endl;
}
