/*
 * FrameDecode.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_DECODER_FRAMEDECODE_H_
#define PRSRC_DECODER_FRAMEDECODE_H_

#include "DecodeDef.h"
#include "RecognizeDef.h"

class CFrameDecode
{

public:

	CFrameDecode();

	virtual ~CFrameDecode();

	INT32 DataInit(INT32 iPSetSize, INT8 *pcPSet);

	INT32 FullVideoInit(INT8 *pFileName);

	INT32 FullVideoDataDecode(Mat &matImgeData, INT32 &iGotFrame, INT32 &iFrameFinished);

	INT32 FrameDataDecode(INT8 *pcFrameData, INT32 iFrameSize, Mat &matImgeData, INT32 &iGotFrame);

	INT32 GetOtherFrameNumber();

	INT32 OtherFrameDataDecode(Mat &matImgeData);

	INT32 ContexFree();

private:

	INT32 RGBDataSave(Mat &matImgeData);

	INT32 YUV420pSave(AVFrame *pFrame, INT32 iWidth, INT32 iHeight);

private:
	INT32 m_iFrameIndex;

	INT32 m_iOtherFrame;

	INT32 m_iVideoStream;

	AVFormatContext *m_pFormatCtx;

	AVCodec *m_pCodec;

	AVCodecContext *m_pCodexCtx;

	AVFrame *m_pFrame;

	AVPacket m_CAVPkt;

	INT8 *m_pcPSetsBuffer;
};



#endif /* PRSRC_DECODER_FRAMEDECODE_H_ */
