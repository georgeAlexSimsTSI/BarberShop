#ifndef BARBER_SHOP_H
#define BARBER_SHOP_H

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <future>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <vector>
#include <sstream>

#include "../include/customer.h"

/**
 * @brief A class that models the barbershop
 */
class BarberShop
{
private:
    const int TIME_TO_CUT_MIN = 5, TIME_TO_CUT_MAX = 15; // constants for determining how long a hair cut will take
    const int NUMBER_OF_SEATS = 3;                       // the number of seats in the waiting room

    int freeBarbers = 0, totalBarbers = 0, serviced = 0, rejected = 0; // tracking variable

    bool open = true; // is the shop open

    std::queue<std::shared_ptr<Customer>> customers; // int is the chair they are in e.g. chair 0,1,2,3,4
    std::vector<std::weak_ptr<Customer>> seats;

    std::mutex mu;
    std::mutex outputMu;
    std::condition_variable waitingCustomers;
    std::condition_variable hadHairCut;

    void print(std::stringstream &str);
    void print(const std::string &str);

    /**
     * @brief function to find the index of a free seat
     *
     * @param lock a lock to prevent anyone from modifying the seats variable
     * @param freeSeat index of the free seat
     * @return std::unique_lock<std::mutex> returns the lock to the original owner
     */
    std::unique_lock<std::mutex> getFreeSeat(std::unique_lock<std::mutex> &lock, int &freeSeat) const;

public:
    /**
     * @brief Construct a new Barber Shop object
     */
    BarberShop();

    /**
     * @brief This is the barber function thread that services customers
     * @param name the name of the Barber
     */
    void barber(const std::string &name);

    /**
     * @brief This is the customer function thread that has a customer take a seat if available
     * and waits for a haircut or leaves if there are no seats
     * @param name The name of the customer
     */
    void customer(const std::string &name);

    /**
     * @brief function that creates barber and customer threads
     *
     * @param numberOfBarbers number of barber threads to create
     * @param numberOfCustomers number of customers to create
     * @param names list of names to give to customers and barbers
     */
    void run(const int &numberOfBarbers, const int &numberOfCustomers, std::vector<std::string> &names);
    std::unique_lock<std::mutex> displayStatus(std::unique_lock<std::mutex> &lock);
};

#endif