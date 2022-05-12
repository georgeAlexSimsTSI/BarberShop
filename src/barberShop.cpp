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
        std::cout << "Barber " << name << " takes customer " << customerName << " to the chair" << std::endl;
        customers.pop();
        std::cout << "Barber " << name << " starts cutting hair " << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds((rand() % (timeToCutMax-timeToCutMin))+timeToCutMin));
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
    std::vector<std::string> names{"John", "Gregor", "Pete", "Peter", "Oliver", "James", "Cody", "Jeeves", "Connor", "Hugo", "Sam", "Herman"};
    std::vector<std::thread> barberThreads;
    std::vector<std::thread> customerThreads;

    int numberOfBarbers = 1, numberOfCustomers = 20;

    for (int i = 0; i < numberOfBarbers; ++i)
    {
        barberThreads.emplace_back(std::thread(&BarberShop::barber, this, names[rand() % names.size()]));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < numberOfCustomers; ++i)
    {
        customerThreads.emplace_back(std::thread(&BarberShop::customer, this, names[rand() % names.size()]));
        std::this_thread::sleep_for(std::chrono::seconds((rand() % 7)+1));
    }

    open = false;
    for (auto &i : barberThreads)
        i.join();

    for (auto &i : customerThreads)
        i.join();
}