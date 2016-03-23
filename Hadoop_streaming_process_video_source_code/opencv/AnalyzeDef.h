/*
 * AnalyzeDef.h
 *
 *  Created on: Dec 21, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_INCLUDE_ANALYZEDEF_H_
#define PRSRC_INCLUDE_ANALYZEDEF_H_

#include "CommonDef.h"

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

#endif /* PRSRC_INCLUDE_ANALYZEDEF_H_ */
