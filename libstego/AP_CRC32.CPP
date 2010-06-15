//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс|						~ A R I E L   P R O D U C T I O N S ~                     |сс|::
//::|сс|                               ~ ALL RIGHTS RESERVED ~                            |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| ~ PROGRAM DATA ~													      /-----/ |сс|::
//::|сс|							            								/-----/ | |сс|::
//::|сс| PROGRAMMER:: James Dougherty											| |   | | |сс|::
//::|сс| COPYRIGHT :: ©2005 Ariel Productions									| |   | | |сс|::
//::|сс| TYPE      :: Checksum Library											| /---|-/ |сс|::
//::|сс|																		/-----/   |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| ~ SOURCE DATA ~																  |сс|::
//::|сс|																				  |сс|::
//::|сс| FILENAME :: AP_CRC32.cpp														  |сс|::
//::|сс| VERSION  :: 1.0																  |сс|::
//::|сс|																				  |сс|::
//::|сс| HISTORY DATA																	  |сс|::
//::|сс| | 																				  |сс|::
//::|сс| |-N/A																			  |сс|::
//::|сс| |																				  |сс|::
//::|сс|																				  |сс|::
//::|сс| BUGS																			  |сс|::
//::|сс| |																				  |сс|::
//::|сс| |-N/A																			  |сс|::
//::|сс| |																				  |сс|::
//::|сс|																				  |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| This program is Copyright ©2005 Ariel Productions (James Dougherty). This		  |сс|::
//::|сс| program is not in the public domain, therefore, it is not open source and all	  |сс|::
//::|сс| copyrights remain intact. This program constitutes proprietary information and   |сс|::
//::|сс| trade secrets of Ariel Productions, Ashland, Ohio. This program or any part of   |сс|::
//::|сс| this program may not be copied, sold, rented, leased, given, assigned, or		  |сс|::
//::|сс| otherwise made available to any other individual or organization without the     |сс|::
//::|сс| written consent of Ariel Productions. Also, no part of this unpublished work may |сс|::
//::|сс| be reproduced, transmitted, or stored in a retrieval system. Nor can this		  |сс|::
//::|сс| program be translated into any human or computer language in any form or by any  |сс|::
//::|сс| means electronically, mechanically, magnetically, optically, chemically, manual  |сс|::
//::|сс| or otherwise without the written permission of the copyright owner				  |сс|::
//::|сс| (Ariel Productions). Contact information is listed below for more information.   |сс|::
//::|сс|																				  |сс|::
//::|сс| Ariel Productions																  |сс|::
//::|сс| James Dougherty																  |сс|::
//::|сс| arielproductions@zoominternet.net												  |сс|::
//::|сс|																				  |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| Special Thanks To Brian Friesen For The Wonderful Article On CRC32				  |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::

#include "stdafx.h"
#include "AP_CRC32.h"

APulong APCRC32::m_CRC32STable[256] =
{
	0x00000000,0x77073096,0xEE0E612C,0x990951BA,
	0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,
	0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,
	0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
	0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,
	0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,
	0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
	0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
	0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,
	0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,
	0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,
	0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
	0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,
	0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
	0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,
	0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
	0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,
	0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,
	0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,
	0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
	0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
	0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
	0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,
	0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
	0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,
	0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
	0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,
	0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,
	0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,
	0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,
	0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
	0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,
	0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,
	0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
	0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
	0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,
	0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,
	0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,
	0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
	0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,
	0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
	0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,
	0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
	0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,
	0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,
	0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,
	0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
	0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
	0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
	0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,
	0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
	0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,
	0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,
	0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,
	0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
	0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,
	0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,
	0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,
	0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
	0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,
	0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,
	0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
	0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D,
};

//------------------------------------------------------------------------------------------------
//--
//--APCRC32(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APCRC32::APCRC32() : m_Method(CRC32M_DYNAMIC),
					 m_CRC32DTable(NULL)
{
	//--
}

//------------------------------------------------------------------------------------------------
//--
//--~APCRC32(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APCRC32::~APCRC32()
{
	CleanupCRC32System();
}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC32(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APDINLINE APvoid APCRC32::ComputeCRC32(const APuchar Byte, APulong& CRC32) const
{
	if(m_Method == CRC32M_STATIC)
	{
		CRC32 = ((CRC32) >> 8) ^ m_CRC32STable[Byte ^ (CRC32 & 0x000000FF)];
	}
	else
	{
		CRC32 = ((CRC32) >> 8) ^ m_CRC32DTable[Byte ^ (CRC32 & 0x000000FF)];
	}
}

//------------------------------------------------------------------------------------------------
//--
//--InitializeCRC32System(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APvoid APCRC32::InitializeCRC32System(CRC32METHOD Method)
{
	APulong CRCValue = 0;

	m_Method = Method;
	if(m_Method == CRC32M_DYNAMIC)
	{
		CleanupCRC32System();
		m_CRC32DTable = new APulong[256];
		for(APint TablePosition = 0; TablePosition < 256; TablePosition++)
		{
			CRCValue = TablePosition;
			for(APint CID = 8; CID > 0; CID--)
			{
				if(CRCValue & 1)
				{
					CRCValue = (CRCValue >> 1) ^ 0xEDB88320;
				}
				else
				{
					CRCValue >>= 1;
				}
			}
			m_CRC32DTable[TablePosition] = CRCValue;
		}
	}
}

//------------------------------------------------------------------------------------------------
//--
//--CleanupCRC32System(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APvoid APCRC32::CleanupCRC32System(APvoid)
{
	if(m_Method == CRC32M_DYNAMIC)
	{
		if(m_CRC32DTable)
		{
			delete[] m_CRC32DTable;
			m_CRC32DTable = NULL;
		}
	}
}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC32_String(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
//APulong APCRC32::ComputeCRC32_String(LPCTSTR Buffer, APulong& CRC32)
//{
//	APulong ErrorStatus = 0;
//	CRC32 = 0xFFFFFFFF;
//
//	if(Buffer == NULL)
//	{
//		CRC32 = ~CRC32;
//		return 1;
//	}
//
//	if(m_Method == CRC32M_DYNAMIC && m_CRC32DTable == NULL)
//	{
//		InitializeCRC32System();
//	}
//
//	try
//	{
//		while(*Buffer != '\0')
//		{
//			ComputeCRC32((APuchar)*Buffer++, CRC32);
//		}
//	}
//	catch(...)
//	{
//		ErrorStatus = 1;
//	}
//
//	CRC32 = ~CRC32;
//	return ErrorStatus;
//}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC32_ByteArray(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APulong APCRC32::ComputeCRC32_ByteArray(const APuchar* Bytes, APlong Length, APulong& CRC32)
{
	APulong ErrorStatus = 0;
	CRC32 = 0xFFFFFFFF;

	if(Bytes == NULL || Length <= 0)
	{
		CRC32 = ~CRC32;
		return 1;
	}

	if(m_Method == CRC32M_DYNAMIC && m_CRC32DTable == NULL)
	{
		InitializeCRC32System();
	}

	try
	{
		for(APlong ID = 0; ID < Length; ID++)
		{
			ComputeCRC32(*Bytes++, CRC32);
		}
	}
	catch(...)
	{
		ErrorStatus = 1;
	}

	CRC32 = ~CRC32;
	return ErrorStatus;
}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC32_File(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
//APulong APCRC32::ComputeCRC32_File(LPCTSTR Filename, APulong& CRC32)
//{
//	APHANDLE FileHandle = NULL;
//	APvoid* CRC32Table = NULL;
//	WIN32_FIND_DATA FindData;
//	APulong ErrorStatus = 0;
//	APuchar Buffer[4096];
//	APulong BytesRead;
//	CRC32 = 0xFFFFFFFF;
//
//	if(Filename == NULL)
//	{
//		CRC32 = ~CRC32;
//		return 1;
//	}
//
//	FileHandle = FindFirstFile((APcrptr)Filename, &FindData);
//	if(FileHandle == INVALID_HANDLE_VALUE)
//	{
//		CRC32 = ~CRC32;
//		FindClose(FileHandle);
//		return 1;
//	}
//	FindClose(FileHandle);
//
//	if(m_Method == CRC32M_DYNAMIC && m_CRC32DTable == NULL)
//	{
//		InitializeCRC32System();
//	}
//
//	try
//	{
//		FileHandle = CreateFile((APcrptr)Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
//		if(FileHandle == INVALID_HANDLE_VALUE)
//		{
//			CRC32 = ~CRC32;
//			return 1;
//		}
//
//		if(m_Method == CRC32M_STATIC)
//		{
//			CRC32Table = &m_CRC32STable;
//		}
//
//		APint ReadSuccess = ReadFile(FileHandle, Buffer, sizeof(Buffer), &BytesRead, NULL);
//		while(ReadSuccess && BytesRead)
//		{
//			if(m_Method == CRC32M_STATIC)
//			{
//				__asm
//				{
//					push esi
//					push edi
//					mov eax, CRC32
//					mov ecx, [eax]
//					mov edi, CRC32Table
//					lea esi, Buffer
//					mov ebx, BytesRead
//					lea edx, [esi + ebx]
//
//					CRC32LS:
//						xor eax, eax
//						mov bl, byte ptr [esi]
//						mov al, cl
//						inc esi
//						xor al, bl
//						shr ecx, 8
//						mov ebx, [edi + eax * 4]
//						xor ecx, ebx
//						cmp edx, esi
//					jne CRC32LS
//
//					pop edi
//					pop esi
//					mov eax, CRC32
//					mov [eax], ecx
//				}
//			}
//			else
//			{
//				__asm
//				{
//					push esi
//					push edi
//					mov eax, CRC32
//					mov ecx, [eax]
//					mov ebx, this
//					mov edi, [ebx]APCRC32.m_CRC32DTable
//					lea esi, Buffer
//					mov ebx, BytesRead
//					lea edx, [esi + ebx]
//
//					CRC32LD:
//						xor eax, eax
//						mov bl, byte ptr [esi]
//						mov al, cl
//						inc esi
//						xor al, bl
//						shr ecx, 8
//						mov ebx, [edi + eax * 4]
//						xor ecx, ebx
//						cmp edx, esi
//					jne CRC32LD
//
//					pop edi
//					pop esi
//					mov eax, CRC32
//					mov [eax], ecx
//				}
//			}
//			ReadSuccess = ReadFile(FileHandle, Buffer, sizeof(Buffer), &BytesRead, NULL);
//		}
//	}
//	catch(...)
//	{
//		ErrorStatus = 1;
//	}
//
//	if(FileHandle != NULL)
//	{
//		CloseHandle(FileHandle);
//	}
//
//	CRC32 = ~CRC32;
//	return ErrorStatus;
//}
