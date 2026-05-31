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
#include <map>
#include <fstream>
#include <sstream>
#include "FileManagement.h"
#include "ApplicationConfig.h"
#include "DataStore.h"

class TicketManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::TICKET_FILEPATH;
public:
	TicketManagementService();
	void saveTicketData();
};