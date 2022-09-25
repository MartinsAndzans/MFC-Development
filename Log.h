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

struct Entry {

	std::string m_LogMessage;
	std::time_t m_TimeStamp;

	Entry()
		: m_LogMessage("Empty Log"), m_TimeStamp(0)
	{ /*...*/ }

	Entry(const std::string &LogMessage, std::time_t TimeStamp)
		: m_LogMessage(LogMessage), m_TimeStamp(TimeStamp)
	{ /*...*/ }
	
};

class DebugLog {
public:

	enum class DebugLevel {
		Info,
		Warning,
		Error,
		None
	};

	DebugLog()
		: m_CurrentDebugLevel(DebugLevel::None), m_DebugLogVector()
	{ /*...*/ }

	void AddEntry(const std::string &LogMessage) {
		
		switch (m_CurrentDebugLevel) {
		case DebugLevel::Info:
			break;
		case DebugLevel::Warning:
			break;
		case DebugLevel::Error:
			break;
		case DebugLevel::None:
			break;
		}

	}

	size_t GetEntryCount(void) {
		return m_DebugLogVector.size();
	}

	void SetDebugLevel(DebugLevel NewDebugLevel) {
		m_CurrentDebugLevel = NewDebugLevel;
	}
	
private:

	DebugLevel m_CurrentDebugLevel;
	std::vector<Entry> m_DebugLogVector;

};

#endif // !__LOG_H__
