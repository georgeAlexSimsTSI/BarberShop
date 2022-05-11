#include "../include/barberShop.h"

const int timeToCut = 5; // how long it takes the barber to cut someones hair
const int numberOfSeats = 3;
int freeBarbers = 0;
bool open = true;
std::queue<std::string> customers; // int is the chair they are in e.g. chair 0,1,2,3,4

std::mutex mu;
std::condition_variable waitingCustomers;
std::condition_variable hadHairCut;
// std::counting_semaphore<1> freeBarbers;

void barber()
{
    while (open || customers.size() != 0)
    {
        std::unique_lock<std::mutex> lock(mu);
        if (customers.size() == 0)
        {
            std::cout << "Barber is having a nap " << std::endl;
            freeBarbers++;
            waitingCustomers.wait(lock);
            std::cout << "Barber has woken up " << std::endl;
            freeBarbers--;
        }
        std::string customerName = customers.front();
        std::cout << "Barber takes customer " << customerName << " to the chair" << std::endl;
        customers.pop();
        std::cout << "Barber starts cutting hair " << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(timeToCut));
        lock.lock();
        std::cout << "Barber has finished cutting hair " << std::endl;
        std::cout << customerName << " leaves" << std::endl;
        hadHairCut.notify_one();
        lock.unlock();
    }
}

void customer(const std::string &name) // producer
{
    std::unique_lock<std::mutex> lock(mu);
    std::cout << "Customer " << name << " enters the barbers " << std::endl;
    if (customers.size() >= numberOfSeats)
    {
        std::cout << " No free seats so customer " << name <<  " leaves " << std::endl;
        return;
    }
    customers.push(name);
    if (freeBarbers) 
        waitingCustomers.notify_one(); // if there is a free barber notify one of them
    else
        hadHairCut.wait(lock); // otherwise wait for a barber
}

int main()
{
    std::thread barberShopOwner(barber);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::thread customer1(customer, "A");
    std::thread customer2(customer, "B");
    std::thread customer3(customer, "C");
    std::thread customer4(customer, "D");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::thread customer5(customer, "E");
    std::thread customer6(customer, "F");
    std::thread customer7(customer, "G");

    open = false;
    barberShopOwner.join();
    return 0;
}