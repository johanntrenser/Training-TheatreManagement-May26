#pragma once

#include "DataStore.h"

class DataLinker
{
private:
    DataStore& m_dataStore;
public:
    DataLinker();
    void linkTheatresAndScreens();
    void linkShowsAndSeatAvailability();
};