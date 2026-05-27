/*
    File: Trenser.QuickBook.cpp
    Author: Trenser
    Created: 20 May 2026
*/
#include <iostream>
#include "UserInterface.h"
#include "DataStore.h"
#include "User.h"
#include "Enums.h"

int main()
{
    DataStore& dataStore = DataStore::getInstance();
    User* user = new User("US100", "Ajmal", "ajmal@gmail.com", "Ajmal@2003", "1234567890", Enums::UserType::ADMIN);
    dataStore.addUser(user);
    UserInterface ui = UserInterface();
    ui.run();
    return 0;
}
