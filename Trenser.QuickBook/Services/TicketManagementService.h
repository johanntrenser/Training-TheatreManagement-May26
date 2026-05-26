#include <string>
#include "DataStore.h"
/*
 * File: TicketManagementService.h
 * Description: Declares the TicketManagementService class, which provides
 *              functionality for managing tickets within the system.
 *              This service will handle operations such as issuing,
 *              validating, retrieving, and canceling tickets for shows.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
class TicketManagementService
{
    DataStore& m_dataStore;
public:
    TicketManagementService();
    const std::string generateTicketId();
    Enums::ProcessStatus generateTicket(Payment* payment, User* user);
};

