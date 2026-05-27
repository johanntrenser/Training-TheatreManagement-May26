#include <iostream>
#include "UserInterface.h"
#include "DataStore.h"
#include "User.h"
#include "Enums.h"

int main()
{
    DataStore& dataStore = DataStore::getInstance();
    User* user1 = new User("US100", "Ajmal", "ajmal@gmail.com", "Ajmal@2003", "1234567890", Enums::UserType::ADMIN);
    dataStore.addUser(user1);
    User* user2 = new User("US101", "Ashlin", "ashlin@gmail.com", "ashlin@2002", "1478523698", Enums::UserType::THEATRE_OWNER);
    dataStore.addUser(user2);
    User* user3 = new User("US103", "Amal", "amal@gmail.com", "amal@2002", "2589631478", Enums::UserType::CUSTOMER);
    dataStore.addUser(user3);
    UserInterface ui = UserInterface();
    ui.run();
    return 0;
}