/*
 * CommonDef.h
 *
 *  Created on: Nov 17, 2014
 *      Author: xuh14
 */

#ifndef VERIFY_DEFINE_COMMONDEF_H
#define VERIFY_DEFINE_COMMONDEF_H

#include <iostream>
#include <string.h>
//#include <Poco/Poco.h>

using namespace std;

///< Custom data types.
//typedef void                    VOID;
//typedef char                    INT8;
//typedef Poco::UInt8             UINT8;
//typedef Poco::Int16             INT16;
//typedef Poco::UInt16            UINT16;
//typedef Poco::Int32             INT32;
//typedef Poco::UInt32            UINT32;
//typedef Poco::IntPtr            LONG;
//typedef Poco::UIntPtr           ULONG;

typedef void                    VOID;
typedef char                    INT8;
typedef unsigned char           UINT8;
typedef short                   INT16;
typedef unsigned short          UINT16;
typedef int                     INT32;
typedef unsigned int            UINT32;


///< Custom data structures.

///< BMP file header
typedef struct BIT_MAP_FILE_HEADER {
	UINT16 bfType;
	UINT32 bfSize;
	UINT16 bfReserved1;
	UINT16 bfReserved2;
	UINT32 bfOffBits;
	BIT_MAP_FILE_HEADER()
	{
		bfType = 0;
		bfSize = 0;
		bfReserved1 = 0;
		bfReserved2 = 0;
		bfOffBits = 0;
	}
} BIT_MAP_FILE_HEADER;

typedef struct BIT_MAP_INFO_HEADER {
	UINT32 biSize;
	INT32  biWidth;
	INT32  biHeight;
	UINT16 biPlanes;
	UINT16 biBitCount;
	UINT32 biCompression;
	UINT32 biSizeImage;
	INT32  biXPelsPerMeter;
	INT32  biYPelsPerMeter;
	UINT32 biClrUsed;
	UINT32 biClrImportant;
	BIT_MAP_INFO_HEADER()
	{
		biSize = 0;
		biWidth = 0;
		biHeight = 0;
		biPlanes = 0;
		biBitCount = 0;
		biCompression = 0;
		biSizeImage = 0;
		biXPelsPerMeter = 0;
		biYPelsPerMeter = 0;
		biClrUsed = 0;
		biClrImportant = 0;
	}
} BIT_MAP_INFO_HEADER;

typedef struct BIT_MAP_FILE
{
	BIT_MAP_FILE_HEADER bfHeader;
	BIT_MAP_INFO_HEADER biInfo;
} BIT_MAP_FILE;


///< Mp4 box header.
const INT32 BOX_TYPE_LENGTH                        = 4;
const INT32 FILE_VISON_LENGTH                      = 4;
const INT8* const FILE_VERSION                     = "v0.1";

typedef struct STRUCT_BOX_HEADER
{
	INT32 iBoxSize;
	INT8  acBoxType[BOX_TYPE_LENGTH];
	STRUCT_BOX_HEADER ()
	{
		iBoxSize = 0;
		memset(acBoxType, 0, BOX_TYPE_LENGTH);
	}
} STRUCT_BOX_HEADER;


///< Custom fragment header.
typedef struct STRUCT_FILE_INFO
{
	STRUCT_BOX_HEADER stDataHdrBHdr;
	UINT32 uiOffset;
	UINT32 uiLength;
	INT8  acVersion[FILE_VISON_LENGTH];
	STRUCT_FILE_INFO ()
	{
		uiOffset = 0;
		uiLength = 0;
		memcpy(acVersion, FILE_VERSION, FILE_VISON_LENGTH);
	}
} STRUCT_FILE_INFO;

typedef struct STRUCT_FRAME_INFO
{
	INT32 iSampleSize;
	INT32 iSampleOffset;
	STRUCT_FRAME_INFO()
	{
		iSampleSize = 0;
		iSampleOffset = 0;
	}

} STRUCT_FRAME_INFO;


const INT32 FILE_NAME_LENGTH                       = 256;

const UINT32 MP4_BOX_TYPE_AVCC                     = 1635148611; ///< avcC
const UINT32 MP4_BOX_TYPE_AVC1                     = 1635148593; ///< avc1
const UINT32 MP4_BOX_TYPE_STSD                     = 1937011556; ///< stsd
const UINT32 MP4_BOX_TYPE_STCO                     = 1937007471; ///< stso
const UINT32 MP4_BOX_TYPE_STSZ                     = 1937011578; ///< stsz
const UINT32 MP4_BOX_TYPE_STSC                     = 1937011555; ///< stsc
const UINT32 MP4_BOX_TYPE_STSS                     = 1937011571; ///< stss
const UINT32 MP4_BOX_TYPE_STBL                     = 1937007212; ///< stbl
const UINT32 MP4_BOX_TYPE_MINF                     = 1835626086; ///< minf
const UINT32 MP4_BOX_TYPE_HDLR                     = 1751411826; ///< hdlr
const UINT32 MP4_BOX_TYPE_MDIA                     = 1835297121; ///< mdia
const UINT32 MP4_BOX_TYPE_TRAK                     = 1953653099; ///< trak
const UINT32 MP4_BOX_TYPE_MOOV                     = 1836019574; ///< moov
const UINT32 MP4_BOX_TYPE_FTYP                     = 1718909296; ///< ftyp

typedef enum
{
	UNKNOWN_PIC_FORMAT = -1,
	PIC_FORMAT_YUV = 0,
	PIC_FORMAT_BMP,
	PIC_FORMAT_PGM,
	PIC_FORMAT_ALL
} ENUM_DECODING_FORMAT;


// error code define
const INT32 NO_ERROR                               = 0x00000000;

const INT32 ERROR_OF_FFMPEG                        = 0x00001000;
const INT32 ERROR_OF_INPUT_FILE                    = 0x00001001;

const INT32 ERROR_OF_FILE_IO                       = 0x00002000;

const INT32 ERROR_OF_INVALID_PARAMETER             = 0x00003000;

const INT32 ERROR_OF_MEDIA_FILE_READ               = 0x00004000;

const INT32 ERROR_OF_FILE_FORMAT                   = 0x00005000;


#endif /* VERIFY_DEFINE_COMMONDEF_H */
