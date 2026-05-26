#include <string>
#include "DataStore.h"
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
class PaymentManagementService
{
	DataStore& m_dataStore;
public:
    PaymentManagementService();
    const std::string generatePaymentId();
    Enums::ProcessStatus initiatePayment(const std::string& bookingId, Enums::PaymentMethod paymentMethod, double amount);
    Payment* getPaymentById(const std::string& paymentId);
    Enums::ProcessStatus viewPaymentStatus(const std::string& paymentId, std::string& bookingId, double& amount,
        Enums::PaymentMethod& paymentMethod, Enums::PaymentStatus& paymentStatus, std::string& paymentDate);
};

