/*
 * ErrorCode.h
 *
 *  Created on: Dec 21, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_INCLUDE_ERRORCODE_H_
#define PRSRC_INCLUDE_ERRORCODE_H_

typedef void                    VOID;
typedef bool                    BOOL;
typedef char                    INT8;
typedef unsigned char           UINT8;
typedef short                   INT16;
typedef unsigned short          UINT16;
typedef int                     INT32;
typedef unsigned int            UINT32;

// error code define
const INT32 NO_ERROR                            = 0x00000000;

const INT32 ERROR_OF_FFMPEG                     = 0x00001000;
const INT32 ERROR_OF_INPUT_FILE                 = 0x00001001;

const INT32 ERROR_OF_FILE_IO                    = 0x00002000;

const INT32 ERROR_OF_INVALID_PARAMETER          = 0x00003000;

const INT32 ERROR_OF_MEDIA_FILE_READ            = 0x00004000;

const INT32 ERROR_OF_FILE_FORMAT                = 0x00005000;

const INT32 ERROR_OF_MEMORY_ALLOCATE            = 0x00006000;

#endif /* PRSRC_INCLUDE_ERRORCODE_H_ */
