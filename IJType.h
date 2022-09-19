#ifndef __IJTYPE_H__
#define __IJTYPE_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
* Licensed Under MIT License                     *
*                                                *
*************************************************/

//===== HEADERS ======//
#include <ostream>
#include <string>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

//======== REDEFINE MACROS ========//
#pragma push_macro("interface")
#undef interface
//=================================//

#define interface struct
#define implements public

//----------------------------------------
// Interface "IJType"
//----------------------------------------
// - Provides Functionality To Work With Output Streams
//----------------------------------------
// Virtual Methods
//----------------------------------------
// - "ToString" - ASCII Charecter Version = 0
// - "ToWString" - Unicode "UTF-16" Charecter Version
//----------------------------------------
// Virtual Methods Return Value
//----------------------------------------
// - "ToString" or "ToWString" Return Value Is Passed to Output Stream
//----------------------------------------
interface IJType {

	IJType() noexcept = default;

	//----------------------------------------
	// This Methods Must Convert
	// Derived Class Content In JSON Format
	
	// ASCII Charecter Version
	virtual std::string ToString() const = 0;
	
	// Unicode "UTF-16" Charecter Version
	virtual std::wstring ToWString() const {

		std::string Format = ToString();
		const size_t FormatLength = Format.length();

		std::wstring WFormat;
		WFormat.reserve(FormatLength);

		for (const char &Character : Format) {
			WFormat.push_back((wchar_t)(Character));
		}

		return WFormat;

	}

	//----------------------------------------

	virtual ~IJType() noexcept = default;
	
	// # !IMPORTANT! : This Operator Call "ToString" Method #
	friend std::ostream& operator<<(std::ostream &outStream, const IJType &Object) {
		outStream << Object.ToString();
		return outStream;
	}
	
	// # !IMPORTANT! : This Operator Call "ToWString" Method #
	friend std::wostream& operator<<(std::wostream &outStream, const IJType &Object) {
		outStream << Object.ToWString();
		return outStream;
	}

};

#pragma pop_macro("interface")

#endif // !__IJTYPE_H__
