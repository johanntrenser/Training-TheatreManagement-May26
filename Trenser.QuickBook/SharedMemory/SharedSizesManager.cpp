/*
 * File: SharedSizesManager.cpp
 */
#include "SharedSizesManager.h"
#include <iostream>

SharedSizesManager::SharedSizesManager()
    : m_hFile(INVALID_HANDLE_VALUE),
    m_hMapFile(NULL),
    m_hMutex(NULL),
    m_pData(nullptr)
{
}

bool SharedSizesManager::initialize()
{
    // create mutex
    m_hMutex = CreateMutex(NULL, FALSE, MTX_NAME_SIZES);
    if (!m_hMutex)
    {
        std::cout << "SharedSizesManager: CreateMutex failed." << std::endl;
        return false;
    }

    // open or create sizes file
    m_hFile = CreateFile(
        L"Files/SharedSizes.dat",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "SharedSizesManager: CreateFile failed." << std::endl;
        return false;
    }

    DWORD blockSize = sizeof(SharedSizesBlock);

    // ensure file is correct size
    LARGE_INTEGER fileSize;
    GetFileSizeEx(m_hFile, &fileSize);
    if ((DWORD)fileSize.QuadPart < blockSize)
    {
        SetFilePointer(m_hFile, blockSize, NULL, FILE_BEGIN);
        SetEndOfFile(m_hFile);
        SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);
    }

    // create file mapping
    m_hMapFile = CreateFileMapping(
        m_hFile, NULL, PAGE_READWRITE,
        0, blockSize, SHM_NAME_SIZES
    );

    if (!m_hMapFile)
    {
        std::cout << "SharedSizesManager: CreateFileMapping failed." << std::endl;
        return false;
    }

    bool alreadyExisted = (GetLastError() == ERROR_ALREADY_EXISTS);

    // map into memory
    m_pData = (SharedSizesBlock*)MapViewOfFile(
        m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, blockSize
    );

    if (!m_pData)
    {
        std::cout << "SharedSizesManager: MapViewOfFile failed." << std::endl;
        return false;
    }

    // first process zeros it out
    if (!alreadyExisted)
    {
        memset(m_pData, 0, blockSize);
        std::cout << "SharedSizesManager: initialised fresh." << std::endl;
    }
    else
    {
        std::cout << "SharedSizesManager: attached to existing." << std::endl;
    }

    return true;
}

void SharedSizesManager::lock()
{
    WaitForSingleObject(m_hMutex, INFINITE);
}

void SharedSizesManager::unlock()
{
    ReleaseMutex(m_hMutex);
}

void SharedSizesManager::flush()
{
    FlushViewOfFile(m_pData, sizeof(SharedSizesBlock));
    FlushFileBuffers(m_hFile);
}

EntitySizeInfo* SharedSizesManager::getUserSizeInfo()
{
    return &m_pData->users;
}

EntitySizeInfo* SharedSizesManager::getMovieSizeInfo()
{
    return &m_pData->movies;
}

EntitySizeInfo* SharedSizesManager::getTheatreSizeInfo()
{
    return &m_pData->theatres;
}

EntitySizeInfo* SharedSizesManager::getScreenSizeInfo()
{
    return &m_pData->screens;
}

EntitySizeInfo* SharedSizesManager::getSeatSizeInfo()
{
    return &m_pData->seats;
}

EntitySizeInfo* SharedSizesManager::getShowSizeInfo()
{
    return &m_pData->shows;
}

EntitySizeInfo* SharedSizesManager::getShowSeatSizeInfo()
{
    return &m_pData->showSeats;
}

EntitySizeInfo* SharedSizesManager::getBookingSizeInfo()
{
    return &m_pData->bookings;
}

EntitySizeInfo* SharedSizesManager::getPaymentSizeInfo()
{
    return &m_pData->payments;
}

EntitySizeInfo* SharedSizesManager::getTicketSizeInfo()
{
    return &m_pData->tickets;
}

EntitySizeInfo* SharedSizesManager::getRefundSizeInfo()
{
    return &m_pData->refunds;
}

EntitySizeInfo* SharedSizesManager::getNotificationSizeInfo()
{
    return &m_pData->notifications;
}

EntitySizeInfo* SharedSizesManager::getLogSizeInfo()
{
    return &m_pData->logs;
}

void SharedSizesManager::cleanup()
{
    if (m_pData)
    {
        UnmapViewOfFile(m_pData);
        m_pData = nullptr;
    }
    if (m_hMapFile)
    {
        CloseHandle(m_hMapFile);
        m_hMapFile = NULL;
    }
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    if (m_hMutex)
    {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
}

SharedSizesManager::~SharedSizesManager()
{
    cleanup();
}