#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer
{
private:
    std::string name;
    int seat;

public:
    Customer() : Customer("John", 0) {}
    Customer(const std::string &name, const int &seat) : name(name), seat(seat) {}

    std::string getName() const { return this->name; }
    int getSeat() const { return this->seat; }

    void setName(const std::string &name) { this->name = name; }
    void setSeat(const int &seat) { this->seat = seat; }
};

#endif