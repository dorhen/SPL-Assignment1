#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    Restaurant(const Restaurant& rhs);
    Restaurant& operator=(const Restaurant &rhs);
    Restaurant(Restaurant&& rhs);
    Restaurant& operator=(Restaurant &&rhs);
    void copy(const Restaurant& other);
    void steal(Restaurant& other);
    void clean();
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    virtual ~Restaurant();
    void update(std::string &command, BaseAction *ba, std::string &args, std::string &action);
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;

    DishType convert(std::string str);
};

#endif