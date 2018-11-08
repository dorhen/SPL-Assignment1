#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    int getCurrentSize() const;
    std::vector<OrderPair> removeOrders(int id);
    void addOrder(OrderPair P);
    bool status() const;
    void copy(const Table& rhs);
    void clear();
    void steal(Table& other);
    Table(Table&& other);
    virtual ~Table();
    Table(const Table& rhs);
    Table& operator=(const Table& rhs);
    Table& operator=(Table&& other);
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
    void erase(std::vector<OrderPair> &listRef, int index);
};


#endif