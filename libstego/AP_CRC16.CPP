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
//::|сс| FILENAME :: AP_CRC16.cpp														  |сс|::
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
//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::

#include "stdafx.h"
#include "AP_CRC16.h"
//#include <fstream.h>

APushort APCRC16::m_CRC16STable[256] =
{
	0x0000,0xC0C1,0xC181,0x0140,
	0xC301,0x03C0,0x0280,0xC241,
	0xC601,0x06C0,0x0780,0xC741,
	0x0500,0xC5C1,0xC481,0x0440,
	0xCC01,0x0CC0,0x0D80,0xCD41,
	0x0F00,0xCFC1,0xCE81,0x0E40,
	0x0A00,0xCAC1,0xCB81,0x0B40,
	0xC901,0x09C0,0x0880,0xC841,
	0xD801,0x18C0,0x1980,0xD941,
	0x1B00,0xDBC1,0xDA81,0x1A40,
	0x1E00,0xDEC1,0xDF81,0x1F40,
	0xDD01,0x1DC0,0x1C80,0xDC41,
	0x1400,0xD4C1,0xD581,0x1540,
	0xD701,0x17C0,0x1680,0xD641,
	0xD201,0x12C0,0x1380,0xD341,
	0x1100,0xD1C1,0xD081,0x1040,
	0xF001,0x30C0,0x3180,0xF141,
	0x3300,0xF3C1,0xF281,0x3240,
	0x3600,0xF6C1,0xF781,0x3740,
	0xF501,0x35C0,0x3480,0xF441,
	0x3C00,0xFCC1,0xFD81,0x3D40,
	0xFF01,0x3FC0,0x3E80,0xFE41,
	0xFA01,0x3AC0,0x3B80,0xFB41,
	0x3900,0xF9C1,0xF881,0x3840,
	0x2800,0xE8C1,0xE981,0x2940,
	0xEB01,0x2BC0,0x2A80,0xEA41,
	0xEE01,0x2EC0,0x2F80,0xEF41,
	0x2D00,0xEDC1,0xEC81,0x2C40,
	0xE401,0x24C0,0x2580,0xE541,
	0x2700,0xE7C1,0xE681,0x2640,
	0x2200,0xE2C1,0xE381,0x2340,
	0xE101,0x21C0,0x2080,0xE041,
	0xA001,0x60C0,0x6180,0xA141,
	0x6300,0xA3C1,0xA281,0x6240,
	0x6600,0xA6C1,0xA781,0x6740,
	0xA501,0x65C0,0x6480,0xA441,
	0x6C00,0xACC1,0xAD81,0x6D40,
	0xAF01,0x6FC0,0x6E80,0xAE41,
	0xAA01,0x6AC0,0x6B80,0xAB41,
	0x6900,0xA9C1,0xA881,0x6840,
	0x7800,0xB8C1,0xB981,0x7940,
	0xBB01,0x7BC0,0x7A80,0xBA41,
	0xBE01,0x7EC0,0x7F80,0xBF41,
	0x7D00,0xBDC1,0xBC81,0x7C40,
	0xB401,0x74C0,0x7580,0xB541,
	0x7700,0xB7C1,0xB681,0x7640,
	0x7200,0xB2C1,0xB381,0x7340,
	0xB101,0x71C0,0x7080,0xB041,
	0x5000,0x90C1,0x9181,0x5140,
	0x9301,0x53C0,0x5280,0x9241,
	0x9601,0x56C0,0x5780,0x9741,
	0x5500,0x95C1,0x9481,0x5440,
	0x9C01,0x5CC0,0x5D80,0x9D41,
	0x5F00,0x9FC1,0x9E81,0x5E40,
	0x5A00,0x9AC1,0x9B81,0x5B40,
	0x9901,0x59C0,0x5880,0x9841,
	0x8801,0x48C0,0x4980,0x8941,
	0x4B00,0x8BC1,0x8A81,0x4A40,
	0x4E00,0x8EC1,0x8F81,0x4F40,
	0x8D01,0x4DC0,0x4C80,0x8C41,
	0x4400,0x84C1,0x8581,0x4540,
	0x8701,0x47C0,0x4680,0x8641,
	0x8201,0x42C0,0x4380,0x8341,
	0x4100,0x81C1,0x8081,0x4040
};

//------------------------------------------------------------------------------------------------
//--
//--APCRC16(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APCRC16::APCRC16() : m_Method(CRC16M_DYNAMIC),
					 m_CRC16DTable(NULL)
{
	//--
}

//------------------------------------------------------------------------------------------------
//--
//--~APCRC16(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APCRC16::~APCRC16()
{
	CleanupCRC16System();
}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC16(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APDINLINE APvoid APCRC16::ComputeCRC16(APuchar Byte, APushort& CRC16)
{
	if(m_Method == CRC16M_STATIC)
	{
		CRC16 = (CRC16 >> 8) ^ m_CRC16STable[(CRC16 ^ Byte) & 0xFF];
	}
	else
	{
		CRC16 = (CRC16 >> 8) ^ m_CRC16DTable[(CRC16 ^ Byte) & 0xFF];
	}
}

//------------------------------------------------------------------------------------------------
//--
//--CreateCRCBit(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APDINLINE APushort CreateCRCBit(APint TablePosition)
{
	APushort Byte = ((APushort)TablePosition);
	APuchar  URBit = 0;

	for(APint ID = 7; ID >= 0; ID--)
	{
		URBit |= (Byte & 1) << ID;
		Byte >>= 1;
	}

	APushort CRCValue = URBit << 8;
	for(APint ID = 0 ; ID < 8; ID++)
	{
		if(CRCValue & 0x8000)
		{
			CRCValue = (CRCValue << 1) ^ 0x8005;
		}
		else
		{
			CRCValue <<= 1;
		}
	}

	APushort SRBit = 0;
	for(APint ID = 15; ID >= 0; ID--)
	{
		SRBit |= (CRCValue & 1) << ID;
		CRCValue >>= 1;
	}
	return SRBit;
}

//------------------------------------------------------------------------------------------------
//--
//--InitializeCRC16System(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APvoid APCRC16::InitializeCRC16System(CRC16METHOD Method)
{
	m_Method = Method;
	if(m_Method == CRC16M_DYNAMIC)
	{
		CleanupCRC16System();
		m_CRC16DTable = new APushort[256];
		for(APint TablePosition = 0; TablePosition < 256; TablePosition++)
		{
			m_CRC16DTable[TablePosition] = CreateCRCBit(TablePosition);
		}
	}
}

//------------------------------------------------------------------------------------------------
//--
//--CleanupCRC16System(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APvoid APCRC16::CleanupCRC16System(APvoid)
{
	if(m_Method == CRC16M_DYNAMIC)
	{
		if(m_CRC16DTable)
		{
			delete[] m_CRC16DTable;
			m_CRC16DTable = NULL;
		}
	}
}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC16_String(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
//APulong APCRC16::ComputeCRC16_String(LPCTSTR Buffer, APushort& CRC16)
//{
//	APulong ErrorStatus = 0;
//	CRC16 = 0xFFFF;
//
//	if(Buffer == NULL)
//	{
//		CRC16 = ~CRC16;
//		return 1;
//	}
//
//	if(m_Method == CRC16M_DYNAMIC && m_CRC16DTable == NULL)
//	{
//		InitializeCRC16System();
//	}
//
//	try
//	{
//		while(*Buffer != '\0')
//		{
//			ComputeCRC16((APuchar)*Buffer++, CRC16);
//		}
//	}
//	catch(...)
//	{
//		ErrorStatus = 1;
//	}
//
//	CRC16 = ~CRC16;
//	return ErrorStatus;
//}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC16_ByteArray(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
APulong APCRC16::ComputeCRC16_ByteArray(const APuchar* Bytes, APlong Length, APushort& CRC16)
{
	APulong ErrorStatus = 0;
	CRC16 = 0xFFFF;

	if(Bytes == NULL || Length == 0)
	{
		CRC16 = ~CRC16;
		return 1;
	}

	if(m_Method == CRC16M_DYNAMIC && m_CRC16DTable == NULL)
	{
		InitializeCRC16System();
	}

	try
	{
		for(APlong ID = 0; ID < Length; ID++)
		{
			ComputeCRC16(*Bytes++, CRC16);
		}
	}
	catch(...)
	{
		ErrorStatus = 1;
	}

	CRC16 = ~CRC16;
	return ErrorStatus;
}

//------------------------------------------------------------------------------------------------
//--
//--ComputeCRC16_File(*)
//--Version: 1.0
//--
//------------------------------------------------------------------------------------------------
//APulong APCRC16::ComputeCRC16_File(LPCTSTR Filename, APushort& CRC16)
//{
//	APHANDLE FileHandle = NULL;
//	WIN32_FIND_DATA FindData;
//	APulong ErrorStatus = 0;
//	APuchar Buffer[4096];
//	APulong BytesRead;
//	CRC16 = 0xFFFF;
//
//	if(Filename == NULL)
//	{
//		CRC16 = ~CRC16;
//		return 1;
//	}
//
//	FileHandle = FindFirstFile((APcrptr)Filename, &FindData);
//	if(FileHandle == INVALID_HANDLE_VALUE)
//	{
//		CRC16 = ~CRC16;
//		FindClose(FileHandle);
//		return 1;
//	}
//	CloseHandle(FileHandle);
//
//	if(m_Method == CRC16M_DYNAMIC && m_CRC16DTable == NULL)
//	{
//		InitializeCRC16System();
//	}
//
//	try
//	{
//		FileHandle = CreateFile((APcrptr)Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
//		if(FileHandle == INVALID_HANDLE_VALUE)
//		{
//			CRC16 = ~CRC16;
//			return 1;
//		}
//
//		APint ReadSuccess = ReadFile(FileHandle, Buffer, sizeof(Buffer), &BytesRead, NULL);
//		while(ReadSuccess && BytesRead)
//		{
//			for(APulong BID = 0; BID < BytesRead; BID++)
//			{
//				ComputeCRC16(Buffer[BID], CRC16);
//			}
//			ReadSuccess = ReadFile(FileHandle, Buffer, sizeof(Buffer), &BytesRead, NULL);
//		}
//
//		if(FileHandle != NULL)
//		{
//			CloseHandle(FileHandle);
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
//	CRC16 = ~CRC16;
//	return ErrorStatus;
//}
