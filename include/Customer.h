#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    virtual Customer* clone();
    std::string getName() const;
    int getId() const;
    virtual ~Customer();
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
private:
    static std::vector<int> strategy;
    std::vector<int> ans;
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
    void setBool(bool t);
private:
    bool ordered;
    static std::vector<int> strategy;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
    void setBool(bool t);
private:
    bool ordered;
    static std::vector<int> strategy;
    std::vector<int> ans;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
    void Sort();
    int findMinimum(std::vector<Dish> &v);
private:
    static std::vector<int> strategy;
    std::vector<Dish> Alcs;
    int current;
    std::vector<int> ans;
    void erase(std::vector<Dish> &listRef, int index);
};


#endif
