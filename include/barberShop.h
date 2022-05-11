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

class BarberShop
{
private:
    const int timeToCut = 5; // how long it takes the barber to cut someones hair
    const int numberOfSeats = 3;
    int freeBarbers = 0;
    bool open = true;
    std::queue<std::string> customers; // int is the chair they are in e.g. chair 0,1,2,3,4

    std::mutex mu;
    std::condition_variable waitingCustomers;
    std::condition_variable hadHairCut;

public:
    BarberShop();

    void barber(const std::string &name);
    void customer(const std::string &name);
    void run();
};

#endif