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
#include <sstream>

class BarberShop
{
private:
    const int timeToCutMin = 5; // how long it takes the barber to cut someones hair
    const int timeToCutMax = 15;
    const int numberOfSeats = 3;
    int freeBarbers = 0, totalBarbers = 0, serviced = 0, rejected = 0;
    bool open = true;
    std::queue<std::string> customers; // int is the chair they are in e.g. chair 0,1,2,3,4

    std::mutex mu;
    std::mutex outputMu;
    std::condition_variable waitingCustomers;
    std::condition_variable hadHairCut;

    void print(std::stringstream &str);
    void print(const std::string &str);

public:
    BarberShop();

    void barber(const std::string &name);
    void customer(const std::string &name);
    void run(const int &numberOfBarbers, const int &numberOfCustomers, std::vector<std::string> &names);
    std::unique_lock<std::mutex> displayStatus(std::unique_lock<std::mutex> &lock);
};

#endif