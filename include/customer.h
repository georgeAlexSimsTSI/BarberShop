#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer
{
private:
    std::string name;
    int seat;

public:
    Customer();
    Customer(const std::string &name, const int &seat);

    std::string getName() const;
    int getSeat() const;

    void setName(const std::string &name);
    void setSeat(const int &seat);
};

#endif