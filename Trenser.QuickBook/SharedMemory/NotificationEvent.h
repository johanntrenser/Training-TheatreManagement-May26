/*
 * File: NotificationEvent.h
 * Description: Declares the NotificationEvent class, which provides inter-process
 *              communication for notifications using shared memory, named events,
 *              and mutex synchronization. This class enables sending and receiving
 *              real-time messages across processes, with thread-safe access and
 *              console output handling. Core functionality includes initialization
 *              of IPC resources, sending notifications, starting a listener thread,
 *              and converting UTF-8 strings to wide strings for console display.
 * Author: Trenser
 * Created: 17 June 2026
 */
#pragma once
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <codecvt>
#include <locale>
#include "Factory.h"
#include "DataStore.h"
#include "NamedMutex.h"
#include "ScopedLock.h"
#include "ApplicationConfig.h"

class NotificationEvent
{
private:
	static HANDLE m_hMapFile;
	static HANDLE m_hEvent;
	static char* m_pointerToBuffer;
	static const size_t m_SHARED_BUFFER_SIZE;
	static NamedMutex* m_mutex;
public:
	NotificationEvent() = default;
	void init(const std::string& userId);
	void notify(const std::string& targetType, const std::string& targetId, const std::string& incomingMessage);
	void notify(const std::string& targetType,const std::vector<std::string>& targetIds, const std::string& message);
	void startListener(const std::string& currentUserType, const std::string& currentUserId,const std::string& userName);
	static std::wstring toWide(const std::string& str);
	~NotificationEvent();
};