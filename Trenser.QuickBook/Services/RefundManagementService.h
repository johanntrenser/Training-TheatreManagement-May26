/*
 * File: RefundManagementService.h
 * Description: Declares the RefundManagementService class, which provides
 *              functionality for managing refunds within the system.
 *              This service will handle operations such as initiating,
 *              processing, and tracking refund transactions.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include "FileManagement.h"
#include "DataStore.h"
#include "ApplicationConfig.h"
#include "ScopedLock.h"
#include "NamedMutex.h"

class RefundManagementService
{
private:
    DataStore& m_dataStore;
    NamedMutex m_mutex;
public:
    RefundManagementService();
    const std::vector<Refund*> getRefunds();
    void getTheatreRefunds(const std::string& userId, std::vector<Refund*>& refunds);
    void getCustomerRefunds(const std::string& userId, std::vector<Refund*>& refunds);
};