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
#include <vector>
#include <map>

using namespace std;

///< Custom data types.

typedef void                    VOID;
typedef char                    INT8;
typedef unsigned char           UINT8;
typedef short                   INT16;
typedef unsigned short          UINT16;
typedef int                     INT32;
typedef unsigned int            UINT32;
typedef long                    LONG;


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


// error code define
const INT32 PROCESS_COMPLETE                       = 0;
const INT32 PROCESS_NO_ERROR                       = 1;
const INT32 PROCESS_ERROR                          = 1001;


#endif /* VERIFY_DEFINE_COMMONDEF_H */
