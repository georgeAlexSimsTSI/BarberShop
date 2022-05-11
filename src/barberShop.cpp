#include "../include/barberShop.h"

BarberShop::BarberShop()
{
    static bool seeded = false; // seed the random number generator
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
}

void BarberShop::barber(const std::string &name)
{
    while (open || customers.size() != 0)
    {
        std::unique_lock<std::mutex> lock(mu);
        if (customers.size() == 0)
        {
            std::cout << "Barber " << name << " is having a nap " << std::endl;
            freeBarbers++;
            waitingCustomers.wait(lock);
            std::cout << "Barber " << name << " has woken up " << std::endl;
            freeBarbers--;
        }

        if (customers.size() == 0)
            continue;

        std::string customerName = customers.front();
        std::cout << "Barber takes customer " << customerName << " to the chair" << std::endl;
        customers.pop();
        std::cout << "Barber " << name << " starts cutting hair " << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(timeToCut));
        lock.lock();
        std::cout << "Barber " << name << " has finished cutting hair " << std::endl;
        std::cout << customerName << " leaves" << std::endl;
        hadHairCut.notify_one();
        lock.unlock();
    }
}

void BarberShop::customer(const std::string &name) // producer
{
    std::unique_lock<std::mutex> lock(mu);
    std::cout << "Customer " << name << " enters the barbers " << std::endl;
    if (customers.size() >= numberOfSeats)
    {
        std::cout << " No free seats so customer " << name << " leaves " << std::endl;
        return;
    }
    customers.push(name);
    if (freeBarbers)
        waitingCustomers.notify_one(); // if there is a free barber notify one of them
    else
        hadHairCut.wait(lock); // otherwise wait for a barber
}

void BarberShop::run()
{
    std::vector<std::string> barberNames{"Steve", "Virgilio"};
    std::vector<std::string> customerNames{"John", "Greg", "Pete", "Peter", "Oliver", "James", "Cody", "Jeeves", "Connor", "Guprit"};
    std::vector<std::thread> barberThreads;
    std::vector<std::thread> customerThreads;

    for (int i = 0; i < 1; ++i)
    {
        barberThreads.emplace_back(std::thread(&BarberShop::barber, this, barberNames[rand() % barberNames.size()]));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 20; ++i)
    {
        customerThreads.emplace_back(std::thread(&BarberShop::customer, this, customerNames[rand() % customerNames.size()]));
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 3));
    }

    open = false;
    for (auto &i : barberThreads)
        i.join();

    for (auto &i : customerThreads)
        i.join();
}