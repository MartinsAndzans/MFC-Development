#ifndef __BYTE_ARRAY_H__
#define __BYTE_ARRAY_H__

/**************************************
*                                     *
* Copyright(c) [2022] Martins Andzans *
* Licensed Under [MIT License]        *
*                                     *
**************************************/

//===== HEADERS ======//
#include <string.h>
#ifndef _STDINT
#include <stdint.h>
#endif // !_STDINT
//====================//

using Byte = unsigned char;
using Void = void;

class ByteArray {
public:

	ByteArray(void) {



	}

	ByteArray(size_t Size) {



	}

	ByteArray(const ByteArray &other) {

		m_Capacity = other.m_Capacity;
		m_Size = other.m_Size;
		m_Buffer = new Byte[m_Capacity]{};

		memcpy(m_Buffer, other.m_Buffer, m_Size);

	}

	void AppendBytes(_In_bytecount_(size) const Void *data, _In_ size_t size) {

		if (data != nullptr && size != 0) {
			for (size_t i = 0; i < size; i++) {
				
			}
		}

	}

	// # Default Destructor #
	~ByteArray() noexcept = default;

private:
	
	size_t m_Size;
	size_t m_Capacity;

	Byte *m_Buffer;

};


#endif // !__BYTE_ARRAY_H__
