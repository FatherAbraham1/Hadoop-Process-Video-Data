/*
 * DecodeDef.h
 *
 *  Created on: Dec 19, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_INCLUDE_DECODEDEF_H_
#define PRSRC_INCLUDE_DECODEDEF_H_

#include "CommonDef.h"

#ifndef INT64_C
#define INT64_C
#define UINT64_C
#endif

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

const INT32 FILE_NAME_LENGTH                       = 256;

#endif /* PRSRC_INCLUDE_DECODEDEF_H_ */
