/*
 * File: BookingManagementService.h
 * Description: Declares the BookingManagementService class, which provides
 *              functionality for managing bookings. The class interacts with
 *              the shared DataStore instance to handle booking-related data
 *              operations such as creation, retrieval, and cancellation.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include "DataStore.h"
#include "FileManagement.h"
#include "LogManagementService.h"
#include "NamedMutex.h"
#include "ScopedLock.h"
#include "NotificationEvent.h"
#include "NotificationManagementService.h"

class BookingManagementService
{
private:
	DataStore& m_dataStore;
	LogManagementService logManagementService;
	NamedMutex m_mutex;
	NotificationEvent m_event;
	NotificationManagementService m_notificationManagementService;
public:
	BookingManagementService();
	const std::vector<const Booking*> getAllBookings();
	const std::vector<const Booking*> getTheatreBookings();
	const std::vector<const Booking*> getCustomerBookings();
	const std::vector<std::string> getAllBookingIds();
	const Booking* getBookingById(const std::string& bookingId);
	const std::vector<const Booking*> getCancellableCustomerBookings();
	Enums::ProcessStatus cancelBooking(const std::string& bookingId);
	const Booking* bookSelectedSeats(const std::string& showId, const std::vector<std::string>& selectSeatIds);
	const std::string generateBookingId();
	void getSeatsFromSeatIds(std::vector<Seat*>& seats, const std::vector<std::string>& seatIds);
	double getBookingAmount(const std::vector<Seat*>& seats);
	Enums::ProcessStatus cancelTicketAndProcessRefund(const Booking* booking);
	void cancelBookingForFailedPayment(const std::string& bookingId);
	const std::vector<std::string> getSeatIdsFromBooking(const Booking*);
};

