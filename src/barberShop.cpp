#include "../include/barberShop.h"

BarberShop::BarberShop()
{
    static bool seeded = false; // seed the random number generator
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
    seats = std::vector<std::weak_ptr<Customer>>(NUMBER_OF_SEATS);
}

void BarberShop::barber(const std::string &name)
{
    std::stringstream stream;
    totalBarbers++;
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

        std::shared_ptr<Customer> customer = customers.front();
        std::string customerName = customer->getName();
        print("Barber " + name + " takes customer " + customerName + " to the chair\n");
        stream.clear();
        customers.pop();

        // need to reset weak ptr seat
        seats[customer->getSeat()].reset();

        print("Barber " + name + " starts cutting hair \n");
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds((rand() % (TIME_TO_CUT_MAX - TIME_TO_CUT_MIN)) + TIME_TO_CUT_MIN));
        lock.lock();
        stream << "Barber " << name << " has finished cutting hair" << std::endl;
        stream << customerName << " leaves" << std::endl;
        ++serviced;
        print(stream);
        // display current state of the queue
        lock = displayStatus(lock);
        hadHairCut.notify_one();
        lock.unlock();
    }
    totalBarbers--;
}

void BarberShop::customer(const std::string &name) // producer
{
    print("Customer " + name + " enters the barbers \n");
    std::unique_lock<std::mutex> lock(mu); // engage lock
    if (customers.size() >= NUMBER_OF_SEATS)
    {
        print("No free seats so customer " + name + " leaves\n ");
        ++rejected;
        return; // release lock
    }
    int seat;
    lock = getFreeSeat(lock, seat);
    std::shared_ptr<Customer> customer = std::make_shared<Customer>(name, seat);
    seats[seat] = customer;
    customers.push(customer);
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
    str.clear();              // clear any bits set
    str.str(std::string("")); // set to empty
}

void BarberShop::print(const std::string &str)
{
    std::unique_lock<std::mutex> lock(outputMu);
    std::cout << str;
}

std::unique_lock<std::mutex> BarberShop::displayStatus(std::unique_lock<std::mutex> &lock)
{
    std::stringstream str;
    str << "---------------------------------------------------------" << std::endl
        << totalBarbers << " Barbers are working " << std::endl
        << customers.size() << " Customers are waiting " << std::endl
        << serviced << " Customers have been serviced " << std::endl
        << rejected << " Customers have been rejected " << std::endl
        << "---------------------------------------------------------" << std::endl;
    // make some sort of representation of who is in what seat
    str << "Waiting Room: " << std::endl
        << " ";
    for (int i = 0; i < NUMBER_OF_SEATS; ++i)
    {
        std::shared_ptr<Customer> customer;
        std::string customerChar = "__";
        if (customer = seats[i].lock())
        {
            customerChar = (customer->getName()).substr(0, 2);
        }
        str << " " << customerChar;
    }
    str << std::endl
        << "---------------------------------------------------------" << std::endl;
    print(str);
    return move(lock);
}

std::unique_lock<std::mutex> BarberShop::getFreeSeat(std::unique_lock<std::mutex> &lock, int &freeSeat) const
{
    for (freeSeat = 0; freeSeat < NUMBER_OF_SEATS; ++freeSeat)
    {
        if (seats[freeSeat].expired())
            return std::move(lock);
    }
    return std::move(lock); // this is an error
}