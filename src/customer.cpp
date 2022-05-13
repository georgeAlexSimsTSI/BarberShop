#include "../include/customer.h"

Customer::Customer() : Customer("John", 0) {}
Customer::Customer(const std::string &name, const int &seat) : name(name), seat(seat) {}

std::string Customer::getName() const { return this->name; }
int Customer::getSeat() const { return this->seat; }

void Customer::setName(const std::string &name) { this->name = name; }
void Customer::setSeat(const int &seat) { this->seat = seat; }