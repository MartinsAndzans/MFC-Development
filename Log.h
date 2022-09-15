#ifndef __LOG_H__
#define __LOG_H__

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
*************************************/

//===== HEADERS ======//
#include <string>
#include <vector>
#include <chrono>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

enum class DebugLevel {
	Info,
	Warning,
	Error,
	None
};

struct Entry {

	Entry()
		: m_LogMessage(), m_TimeStamp(), m_DebugLevel(DebugLevel::None)
	{ /*...*/ }

	Entry(const std::string &LogMessage, std::time_t TimeStamp, DebugLevel DebugLevel)
		: m_LogMessage(LogMessage), m_TimeStamp(TimeStamp), m_DebugLevel(DebugLevel)
	{ /*...*/ }

	std::string m_LogMessage;
	std::time_t m_TimeStamp;
	DebugLevel m_DebugLevel;
	
};

class DebugLog {

	DebugLog();

	void AddEntry(const std::string &LogMessage) {
		// m_DebugLogEntryData.push_back();
	}

	void SetDebugLevel(DebugLevel debugLevel) {
		m_CurrentDebugLevel = debugLevel;
	}

	const std::vector<Entry>& GetDebugLogEntryData() const noexcept {
		return m_DebugLogEntryData;
	}
	
private:

	DebugLevel m_CurrentDebugLevel;
	std::vector<Entry> m_DebugLogEntryData;

};

#endif // !__LOG_H__
