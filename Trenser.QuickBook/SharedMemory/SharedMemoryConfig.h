/*
 * File: SharedMemoryConfig.h
 * Description: All constants for shared memory names,
 *              file paths, and mutex names.
 */
#pragma once

 // ── shared memory names (used by CreateFileMapping / OpenFileMapping) ──
#define SHM_NAME_SIZES      L"QuickBook_Sizes"
#define SHM_NAME_USERS      L"QuickBook_Users"
#define SHM_NAME_BOOKINGS   L"QuickBook_Bookings"
#define SHM_NAME_SHOWS      L"QuickBook_Shows"
#define SHM_NAME_MOVIES     L"QuickBook_Movies"
#define SHM_NAME_THEATRES   L"QuickBook_Theatres"
#define SHM_NAME_SCREENS    L"QuickBook_Screens"
#define SHM_NAME_SEATS      L"QuickBook_Seats"
#define SHM_NAME_TICKETS    L"QuickBook_Tickets"
#define SHM_NAME_PAYMENTS   L"QuickBook_Payments"
#define SHM_NAME_REFUNDS    L"QuickBook_Refunds"
#define SHM_NAME_NOTIFS     L"QuickBook_Notifications"
#define SHM_NAME_LOGS       L"QuickBook_Logs"

// ── mutex names ──
#define MTX_NAME_SIZES      L"QuickBook_Mutex_Sizes"
#define MTX_NAME_USERS      L"QuickBook_Mutex_Users"
#define MTX_NAME_BOOKINGS   L"QuickBook_Mutex_Bookings"
#define MTX_NAME_SHOWS      L"QuickBook_Mutex_Shows"
#define MTX_NAME_MOVIES     L"QuickBook_Mutex_Movies"
#define MTX_NAME_THEATRES   L"QuickBook_Mutex_Theatres"
#define MTX_NAME_SCREENS    L"QuickBook_Mutex_Screens"
#define MTX_NAME_SEATS      L"QuickBook_Mutex_Seats"
#define MTX_NAME_TICKETS    L"QuickBook_Mutex_Tickets"
#define MTX_NAME_PAYMENTS   L"QuickBook_Mutex_Payments"
#define MTX_NAME_REFUNDS    L"QuickBook_Mutex_Refunds"
#define MTX_NAME_NOTIFS     L"QuickBook_Mutex_Notifications"
#define MTX_NAME_LOGS       L"QuickBook_Mutex_Logs"

// ── binary file paths ──
#define SHM_FILE_SIZES      "Files/SharedSizes.dat"
#define SHM_FILE_USERS      "Files/Users.dat"
#define SHM_FILE_BOOKINGS   "Files/Bookings.dat"
#define SHM_FILE_SHOWS      "Files/Shows.dat"
#define SHM_FILE_MOVIES     "Files/Movies.dat"
#define SHM_FILE_THEATRES   "Files/Theatres.dat"
#define SHM_FILE_SCREENS    "Files/Screens.dat"
#define SHM_FILE_SEATS      "Files/Seats.dat"
#define SHM_FILE_TICKETS    "Files/Tickets.dat"
#define SHM_FILE_PAYMENTS   "Files/Payments.dat"
#define SHM_FILE_REFUNDS    "Files/Refunds.dat"
#define SHM_FILE_NOTIFS     "Files/Notifications.dat"
#define SHM_FILE_LOGS       "Files/Logs.dat"