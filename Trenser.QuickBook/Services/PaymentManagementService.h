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
#include <string>
#include "DataStore.h"
#include "FileManagement.h"
#include "LogManagementService.h"
#include "NotificationManagementService.h"
#include "NamedMutex.h"
#include "ScopedLock.h"
#include "NotificationEvent.h"

class PaymentManagementService
{
	DataStore& m_dataStore;
    NamedMutex m_paymentMutex;
    NamedMutex m_refundMutex;
    LogManagementService logManagementService;
    NotificationManagementService m_notificationManagementService;
    NotificationEvent m_event;
public:
    PaymentManagementService();
    const std::string generatePaymentId();
    Enums::ProcessStatus initiatePayment(const std::string& bookingId, Enums::PaymentMethod paymentMethod, double amount);
    Payment* getPaymentById(const std::string& paymentId);
    Enums::ProcessStatus refundPayment(Ticket* ticket, Payment* payment);
    const std::string generateRefundId();
    const std::vector<Payment*> getAllPayments();
};