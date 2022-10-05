#ifndef __IJTYPE_H__
#define __IJTYPE_H__

/*************************************************
*                                                *
* Copyright(c) 2022 Martins Andzans              *
* Licensed Under MIT License                     *
*                                                *
*************************************************/

//===== HEADERS ======//
#include <string>
#include <ostream>
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
// - "ToStringA" or "ToStringW" Return Value Is Passed to Output Stream
//----------------------------------------
interface IJType {

	IJType() noexcept = default;

	//----------------------------------------
	// This Methods Must Convert
	// Derived Class Content In JSON Format
	
	// ASCII Charecter Version
	virtual std::string ToStringA() const = 0;
	
	// Unicode "UTF-16" Charecter Version
	virtual std::wstring ToStringW() const {

		std::string Format = ToStringA();
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
	
	// # !IMPORTANT! : This Operator Call "ToStringA" Method #
	friend std::ostream& operator<<(std::ostream &outStream, const IJType &Object) {
		outStream << Object.ToStringA();
		return outStream;
	}
	
	// # !IMPORTANT! : This Operator Call "ToStringW" Method #
	friend std::wostream& operator<<(std::wostream &outStream, const IJType &Object) {
		outStream << Object.ToStringW();
		return outStream;
	}

};

#pragma pop_macro("interface")

#endif // !__IJTYPE_H__
