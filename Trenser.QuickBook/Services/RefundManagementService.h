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
#include "DataStore.h"
#include "ApplicationConfig.h"
class RefundManagementService
{
private:
    const std::string& PATH = config::File::REFUND_FILEPATH;
    DataStore& m_dataStore;
public:
    RefundManagementService();
    void saveRefundData();
};