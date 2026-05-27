/*
 * File: PaymentManagementService.h
 * Description: Declares the PaymentManagementService class, which provides
 *              functionality for managing payments within the system.
 *              This service will handle operations such as processing,
 *              validating, and recording payment transactions.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <map>
#include <fstream>
#include "DataStore.h"
#include "ApplicationConfig.h"
class PaymentManagementService
{
private:
    const std::string& PATH = config::File::PAYMENT_FILEPATH;
    DataStore& m_dataStore;
public:
    PaymentManagementService();
    void savePaymentData();
};