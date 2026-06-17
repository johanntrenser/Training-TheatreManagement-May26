/*
 * File: Refund.h
 * Description: Defines the Refund class, which represents a refund
 *              transaction for a cancelled ticket. Stores details such
 *              as refund ID, associated ticket, refund amount, timestamp,
 *              and status.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#pragma once
#include <string>
#include "Ticket.h"
#include "Factory.h"
#include "InputHelper.h"
#include "SharedStructs.h"

class Refund
{
private:
    std::string m_refundId;
    Ticket* m_bookedTicket;
    double m_refundAmount;
    time_t m_time;
    Enums::RefundStatus m_status;
public:
    Refund();
    Refund(const std::string& refundId,
        Ticket* bookedTicket,
        double refundAmount,
        time_t time);
    const std::string& getRefundId() const;
    Ticket* getBookedTicket() const;
    double getRefundAmount() const;
    time_t getTime() const;
    Enums::RefundStatus getStatus() const;
    void setRefundId(const std::string& id);
    void setBookedTicket(Ticket* bookedTicket);
    void setRefundAmount(double refundAmount);
    void setTime(time_t time);
    void setStatus(Enums::RefundStatus status);
    void serialize(SharedRefund& sharedRefund) const;
    static Refund* deserialize(const SharedRefund* sharedRefund);
};
