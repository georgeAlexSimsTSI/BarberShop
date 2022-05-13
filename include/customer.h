#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

/**
 * @brief class representing a customer
 */
class Customer
{
private:
    std::string name; // customers name
    int seat;         // which seat is the customer waiting in

public:
    Customer();
    Customer(const std::string &name, const int &seat);

    std::string getName() const;
    int getSeat() const;

    void setName(const std::string &name);
    void setSeat(const int &seat);
};

#endif