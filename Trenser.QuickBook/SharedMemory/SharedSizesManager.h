/*
 * File: SharedSizesManager.h
 */
#pragma once
#include <windows.h>
#include "SharedMemoryConfig.h"

 // size info for one entity
struct EntitySizeInfo
{
    size_t currentSize;    // current byte size of shared memory block
    int    recordCount;    // how many records currently stored
    int    idCounter;      // counter for ID generation
    int    version;        // incremented every time block is remapped
};

// one block holding size info for all entities
struct SharedSizesBlock
{
    EntitySizeInfo users;
    EntitySizeInfo movies;
    EntitySizeInfo theatres;
    EntitySizeInfo screens;
    EntitySizeInfo seats;
    EntitySizeInfo shows;
    EntitySizeInfo showSeats;
    EntitySizeInfo bookings;
    EntitySizeInfo payments;
    EntitySizeInfo tickets;
    EntitySizeInfo refunds;
    EntitySizeInfo notifications;
    EntitySizeInfo logs;
};

class SharedSizesManager
{
private:
    HANDLE            m_hFile;
    HANDLE            m_hMapFile;
    HANDLE            m_hMutex;
    SharedSizesBlock* m_pData;

public:
    SharedSizesManager();
    bool initialize();
    void lock();
    void unlock();
    void flush();
    void cleanup();

    EntitySizeInfo* getUserSizeInfo();
    EntitySizeInfo* getMovieSizeInfo();
    EntitySizeInfo* getTheatreSizeInfo();
    EntitySizeInfo* getScreenSizeInfo();
    EntitySizeInfo* getSeatSizeInfo();
    EntitySizeInfo* getShowSizeInfo();
    EntitySizeInfo* getShowSeatSizeInfo();
    EntitySizeInfo* getBookingSizeInfo();
    EntitySizeInfo* getPaymentSizeInfo();
    EntitySizeInfo* getTicketSizeInfo();
    EntitySizeInfo* getRefundSizeInfo();
    EntitySizeInfo* getNotificationSizeInfo();
    EntitySizeInfo* getLogSizeInfo();

    ~SharedSizesManager();
};