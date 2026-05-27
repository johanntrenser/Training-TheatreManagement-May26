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

class BookingManagementService
{
private:
	DataStore& m_dataStore;
public:
	BookingManagementService();
	const std::vector<const Booking*> getAllBookings() const;
	const std::vector<const Booking*> getTheatreBookings() const;
	const std::vector<const Booking*> getCustomerBookings() const;
	const std::vector<std::string> getAllBookingIds() const;
	const Booking* getBookingById(const std::string& bookingId) const;
};

