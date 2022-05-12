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
    std::stringstream stream;
    while (open || customers.size() != 0)
    {
        std::unique_lock<std::mutex> lock(mu); // engage lock
        if (customers.size() == 0)
        {
            print("Barber " + name + " is having a nap \n");
            freeBarbers++;
            waitingCustomers.wait(lock); // release lock and reengage 
            print("Barber " + name + " has woken up \n");
            freeBarbers--;
        }

        if (customers.size() == 0) // mainly here to remove a compiler warning
            continue;

        std::string customerName = customers.front();
        print("Barber " + name + " takes customer " + customerName + " to the chair\n");
        stream.clear();
        customers.pop();
        print("Barber " + name + " starts cutting hair \n");
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds((rand() % (timeToCutMax - timeToCutMin)) + timeToCutMin));
        lock.lock();
        stream << "Barber " << name << " has finished cutting hair" << std::endl;
        stream << customerName << " leaves" << std::endl;
        print(stream);
        hadHairCut.notify_one();
        lock.unlock();
    }
}

void BarberShop::customer(const std::string &name) // producer
{
    print("Customer " + name + " enters the barbers \n");
    std::unique_lock<std::mutex> lock(mu); // engage lock
    if (customers.size() >= numberOfSeats)
    {
        print("No free seats so customer " + name + " leaves\n ");
        return; // release lock
    }
    customers.push(name);
    if (freeBarbers)
        waitingCustomers.notify_one(); // if there is a free barber notify one of them
    else
        hadHairCut.wait(lock); // otherwise wait for a barber release lock then engage
}

void BarberShop::run(const int &numberOfBarbers, const int &numberOfCustomers, std::vector<std::string> &names)
{
    std::vector<std::thread> barberThreads;
    std::vector<std::thread> customerThreads;

    for (int i = 0; i < numberOfBarbers; ++i)
    {
        barberThreads.emplace_back(std::thread(&BarberShop::barber, this, names[rand() % names.size()]));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < numberOfCustomers; ++i)
    {
        customerThreads.emplace_back(std::thread(&BarberShop::customer, this, names[rand() % names.size()]));
        std::this_thread::sleep_for(std::chrono::seconds((rand() % 7) + 1));
    }

    open = false;
    for (auto &i : barberThreads)
        i.join();

    for (auto &i : customerThreads)
        i.join();
}

void BarberShop::print(std::stringstream &str)
{
    std::unique_lock<std::mutex> lock(outputMu);
    std::string tmpStr = str.str();
    std::cout << tmpStr;

    // clean out the stream
    str.clear(); // clear any bits set
    str.str(std::string("")); // set to empty
}

void BarberShop::print(const std::string &str)
{
    std::unique_lock<std::mutex> lock(outputMu);
    std::cout << str;
}