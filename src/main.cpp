#include "../include/barberShop.h"

int main()
{
    BarberShop shop;
    int numberOfBarbers = 2, numberOfCustomers = 30;
    std::vector<std::string> names{
        "John",
        "Gregor",
        "Pete",
        "Peter",
        "Oliver",
        "James",
        "Cody",
        "Jeeves",
        "Connor",
        "Hugo",
        "Sam",
        "Herman",
        "Liam",
        "Noah",
        "Oliver",
        "Elijah",
        "James",
        "William",
        "Benjamin",
        "Lucas",
        "Henry",
        "Theodore",
        "Jack",
        "Levi",
        "Alexander",
        "Jackson",
        "Daniel",
        "Michael",
        "Mason",
        "Sebastian",
        "Ethan",
        "Logan",
        "Owen",
        "Samuel",
        "Jacob",
        "Asher",
        "John",
        "Joseph",
        "Wyatt",
        "David",
        "Leo",
        "Luke",
        "Julian",
        "Hudson",
        "Grayson",
        "Matthew",
        "Ezra",
        "Gabriel",
        "Carter",
        "Isaac"};

    // std::thread barb = std::thread (&BarberShop::barber, &shop, "FAKE BARBER"); //how to create a thread in another class while outside of it

    shop.run(numberOfBarbers, numberOfCustomers, names);
    return 0;
}