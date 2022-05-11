// #include "../include/barberShop.h"

// BarberShop::BarberShop()
// {
//     open = true;
// }

// void BarberShop::customer() // producer
// {
//     std::unique_lock<std::mutex> lock(mutexLock);
//     std::string name = "Timmy";
//     std::cout << "Customer: " << name << " has entered the shop " << std::endl;

//     if (waitingCustomers.size() >= numberOfSeats) // if no seats
//     {
//         std::cout << "Customer finds no available seats so they leave" << std::endl;
//         return;
//     }

//     waitingCustomers.emplace(name); // add customer to queue
//     std::cout << "Customer: " << name << " Takes a seat " << std::endl;

//     customersCondition.notify_one(); // notify any sleeping barbers that there is a customer

//     availableBarberCondition.wait(lock); // wait for a active barber
//     std::cout << "Customer: " << name << " Leaves " << std::endl;
// }

// void BarberShop::barber()
// {
//     std::unique_lock<std::mutex> lock(mutexLock);
//     std::string name = "Tim";
//     std::cout << "Barber: " << name << " has started working" << std::endl;
//     lock.unlock();
//     while (open) // while the shop is open
//     {
//         lock.lock(); // acquire a lock

//         if (waitingCustomers.size() == 0)
//         {
//             std::cout << "Due to lack of customers Barber " << name << " is having a nap " << std::endl;
//             customersCondition.wait(lock); // wait for a customer
//         }

//         std::string customerName = waitingCustomers.front();
//         waitingCustomers.pop();

//         std::cout << "Barber: " << name << " is cutting Customer " << customerName << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(timeToCut));
//         std::cout << "Barber: " << name << " has finished cutting hair " << std::endl;
//         availableBarberCondition.notify_one();
//         lock.unlock();
//     }
//     std::cout << "Barber thread: " << name << " has ended" << std::endl;
// }

// void BarberShop::run()
// {
//     // std::string barberNames[] = {"Jason"};
//     // std::string customerNames[] = {"Tim", "Tom"};

//     //std::thread barberThread(barber);

//     // for (int i = 0; i < 10; ++i)
//     // {
//     //     std::thread customerThread1(customer, "Tim");
//     //     customerThread1.detach();
//     //     std::thread customerThread2(customer, "Tommy");
//     //     customerThread2.detach();
//     // }
//     // open = false;
// }