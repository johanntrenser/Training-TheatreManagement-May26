/*
 * File: SharedMemoryManager.h
 * Description: Generic manager for one entity type.
 *              Handles dynamic growth, remapping, lock/unlock,
 *              and direct pointer access into shared memory.
 */
#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include "SharedSizesManager.h"

template <typename T>
class SharedMemoryManager
{
private:
    HANDLE          m_hFile;
    HANDLE          m_hMapFile;
    HANDLE          m_hMutex;
    void* m_pData;
    std::wstring    m_shmName;
    std::wstring    m_mutexName;
    std::wstring    m_filePath;
    EntitySizeInfo* m_pSizeInfo;
    int             m_lastKnownVersion;

    // ── header at start of shared memory block ────────────────────
    // first sizeof(int) bytes = record count
    // actual records start after header

    static const size_t HEADER_SIZE = sizeof(int);

    int* getCountPtr()
    {
        return (int*)m_pData;
    }

    T* getBasePtr()
    {
        return (T*)((char*)m_pData + HEADER_SIZE);
    }

    // ── grow file and remap ───────────────────────────────────────
    bool remap(size_t newSize)
    {
        // unmap current view
        if (m_pData)
        {
            UnmapViewOfFile(m_pData);
            m_pData = nullptr;
        }

        // close old map handle
        if (m_hMapFile)
        {
            CloseHandle(m_hMapFile);
            m_hMapFile = NULL;
        }

        // grow the file
        LARGE_INTEGER liSize;
        liSize.QuadPart = (LONGLONG)newSize;
        if (!SetFilePointerEx(m_hFile, liSize, NULL, FILE_BEGIN))
        {
            return false;
        }
        if (!SetEndOfFile(m_hFile))
        {
            return false;
        }
        SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);

        // create new mapping with new size
        m_hMapFile = CreateFileMapping(
            m_hFile, NULL, PAGE_READWRITE,
            (DWORD)(newSize >> 32),
            (DWORD)(newSize & 0xFFFFFFFF),
            m_shmName.c_str()
        );

        if (!m_hMapFile)
        {
            std::cout << "remap: CreateFileMapping failed "
                << GetLastError() << std::endl;
            return false;
        }

        // map new view
        m_pData = MapViewOfFile(
            m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, newSize
        );

        if (!m_pData)
        {
            std::cout << "remap: MapViewOfFile failed "
                << GetLastError() << std::endl;
            return false;
        }

        // update size info
        m_pSizeInfo->currentSize = newSize;
        m_pSizeInfo->version++;
        m_lastKnownVersion = m_pSizeInfo->version;

        return true;
    }

    // ── check if another process grew the mapping ─────────────────
    void syncRemap()
    {
        if (m_pSizeInfo->version == m_lastKnownVersion)
        {
            return;  // no change — nothing to do
        }

        size_t newSize = m_pSizeInfo->currentSize;

        // unmap old view
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

        // open existing mapping with new size
        m_hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            m_shmName.c_str()
        );

        if (m_hMapFile)
        {
            m_pData = MapViewOfFile(
                m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, newSize
            );
        }

        m_lastKnownVersion = m_pSizeInfo->version;
    }

public:
    SharedMemoryManager(
        const std::wstring& shmName,
        const std::wstring& mutexName,
        const std::wstring& filePath,
        EntitySizeInfo* pSizeInfo)
        : m_hFile(INVALID_HANDLE_VALUE),
        m_hMapFile(NULL),
        m_hMutex(NULL),
        m_pData(nullptr),
        m_shmName(shmName),
        m_mutexName(mutexName),
        m_filePath(filePath),
        m_pSizeInfo(pSizeInfo),
        m_lastKnownVersion(0)
    {
    }

    // ── initialise ───────────────────────────────────────────────
    bool initialize()
    {
        // create mutex
        m_hMutex = CreateMutex(NULL, FALSE, m_mutexName.c_str());
        if (!m_hMutex)
        {
            std::cout << "CreateMutex failed: " << GetLastError() << std::endl;
            return false;
        }

        // open or create file
        m_hFile = CreateFile(
            m_filePath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (m_hFile == INVALID_HANDLE_VALUE)
        {
            std::cout << "CreateFile failed: " << GetLastError() << std::endl;
            return false;
        }

        bool isNewFile = (GetLastError() != ERROR_ALREADY_EXISTS);

        // determine initial size
        size_t currentSize = m_pSizeInfo->currentSize;
        if (currentSize == 0 || isNewFile)
        {
            // start with just the header
            currentSize = HEADER_SIZE;
            m_pSizeInfo->currentSize = currentSize;
            m_pSizeInfo->recordCount = 0;
            m_pSizeInfo->idCounter = 0;
            m_pSizeInfo->version = 1;
        }

        // set file size
        LARGE_INTEGER liSize;
        liSize.QuadPart = (LONGLONG)currentSize;
        SetFilePointerEx(m_hFile, liSize, NULL, FILE_BEGIN);
        SetEndOfFile(m_hFile);
        SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);

        // create file mapping
        m_hMapFile = CreateFileMapping(
            m_hFile, NULL, PAGE_READWRITE,
            0, (DWORD)currentSize,
            m_shmName.c_str()
        );

        if (!m_hMapFile)
        {
            std::cout << "CreateFileMapping failed: "
                << GetLastError() << std::endl;
            return false;
        }

        bool alreadyExisted = (GetLastError() == ERROR_ALREADY_EXISTS);

        // map into memory
        m_pData = MapViewOfFile(
            m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, currentSize
        );

        if (!m_pData)
        {
            std::cout << "MapViewOfFile failed: "
                << GetLastError() << std::endl;
            return false;
        }

        // first process initialises header
        if (!alreadyExisted)
        {
            memset(m_pData, 0, currentSize);
            *getCountPtr() = 0;
        }

        m_lastKnownVersion = m_pSizeInfo->version;
        return true;
    }

    // ── lock / unlock ────────────────────────────────────────────
    void lock() { WaitForSingleObject(m_hMutex, INFINITE); }
    void unlock() { ReleaseMutex(m_hMutex); }

    // ── get record count ─────────────────────────────────────────
    int getCount()
    {
        syncRemap();
        return m_pSizeInfo->recordCount;
    }

    // ── get record by index ───────────────────────────────────────
    // returns pointer directly into shared memory
    T* getRecord(int index)
    {
        syncRemap();
        if (index < 0 || index >= m_pSizeInfo->recordCount)
        {
            return nullptr;
        }
        return &getBasePtr()[index];
    }

    // ── find record — returns direct pointer into shared memory ───
    // use for READ operations
    const T* findRecord(bool (*predicate)(const T&, const void*),
        const void* param) const
    {
        int count = m_pSizeInfo->recordCount;
        T* base = (T*)((char*)m_pData + HEADER_SIZE);
        for (int i = 0; i < count; ++i)
        {
            if (base[i].isOccupied && predicate(base[i], param))
            {
                return &base[i];
            }
        }
        return nullptr;
    }

    // ── find record for update — non-const pointer ────────────────
    // use for WRITE operations
    T* findRecordForUpdate(bool (*predicate)(const T&, const void*),
        const void* param)
    {
        syncRemap();
        int count = m_pSizeInfo->recordCount;
        T* base = getBasePtr();
        for (int i = 0; i < count; ++i)
        {
            if (base[i].isOccupied && predicate(base[i], param))
            {
                return &base[i];
            }
        }
        return nullptr;
    }

    // ── add a new record — grows shared memory if needed ─────────
    T* addRecord()
    {
        syncRemap();

        int    currentCount = m_pSizeInfo->recordCount;
        size_t neededSize = HEADER_SIZE + (currentCount + 1) * sizeof(T);
        size_t currentSize = m_pSizeInfo->currentSize;

        if (neededSize > currentSize)
        {
            // grow
            if (!remap(neededSize))
            {
                return nullptr;
            }
        }

        // get pointer to new empty slot at end
        T* newSlot = &getBasePtr()[currentCount];
        memset(newSlot, 0, sizeof(T));

        // increment count
        m_pSizeInfo->recordCount++;
        *getCountPtr() = m_pSizeInfo->recordCount;

        return newSlot;
    }

    // ── generate next unique ID ───────────────────────────────────
    int getNextId()
    {
        return ++m_pSizeInfo->idCounter;
    }

    // ── flush a single record to disk ────────────────────────────
    void flushRecord(T* recordPtr)
    {
        FlushViewOfFile(recordPtr, sizeof(T));
        FlushFileBuffers(m_hFile);
    }

    // ── flush entire block to disk ───────────────────────────────
    void flushAll()
    {
        FlushViewOfFile(m_pData, m_pSizeInfo->currentSize);
        FlushFileBuffers(m_hFile);
    }

    // ── cleanup ──────────────────────────────────────────────────
    void cleanup()
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

    ~SharedMemoryManager() { cleanup(); }
};