/*
 * VideoDecode.h
 *
 *  Created on: Nov 17, 2014
 *      Author: xuh14
 */

#ifndef VERIFY_UTIL_VIDEODECODE_H
#define VERIFY_UTIL_VIDEODECODE_H

#include "CommonDef.h"
#include "IFragmentDecode.h"

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

class CVideoDecode : public IFragmentDecode
{

public:

	CVideoDecode();

	virtual ~CVideoDecode();

	INT32 VideoFragmentDecode(INT8 *pFileName, INT8 *pcOutputPath, ENUM_DECODING_FORMAT enPicFormat);

	INT32 VideoDecode(INT8 *pFileName, INT8 *pcOutputPath, ENUM_DECODING_FORMAT enPicFormat);

	virtual int VideoFragmentDecode(string &strFragmentData, string strInputPath, string strOutputDir = "./result/", string strPicFormat = "yuv");

private:

	INT32 BMPSave(AVFrame *pFrame, AVPixelFormat enPixelFormat, INT32 iWidth, INT32 iHeight, INT32 iFrameIndex);

	INT32 PGMSave(AVFrame *pFrame, INT32 iWidth, INT32 iHeight, INT32 iFrameIndex);

	INT32 YUV420pSave(AVFrame *pFrame, INT32 iWidth, INT32 iHeight, INT32 iFrameIndex);

	INT32 DecodeWriteFrame(AVCodecContext *pAvCtx, AVFrame *pFrame, INT32 *pFrameIndex, AVPacket *pPkt, INT32 iLastFrame);

	INT32 GetParameters(INT8 *pcFileName, INT8 *pcBuffer, INT32 &iSize);

private:

	INT32 m_iOtherFrame;

	const INT8 *m_pcOutputPath;

	ENUM_DECODING_FORMAT m_enPicFormat;

	string m_strFileName;
};

#endif /* VERIFY_UTIL_VIDEODECODE_H_ */
