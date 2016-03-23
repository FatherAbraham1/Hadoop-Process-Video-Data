/*
 * SystemProcessor.h
 *
 *  Created on: Dec 21, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_SYSTEMPROCESSOR_H_
#define PRSRC_SYSTEMPROCESSOR_H_


#include "analyzer/FragmentAnalyze.h"
#include "decoder/FrameDecode.h"
#include "filter/ImageFilter.h"
#include "recognizer/PlateRecognize.h"
#include <map>

class CSystemProcessor
{

public:

	CSystemProcessor();

	virtual ~CSystemProcessor();

	bool FragmentProcess(string &strFragment, INT32 &iNumber);

	bool GetImageInfo(string &strPlateInfo, string &strImageData);

private:

	INT32 ResultCollected(Mat &matCurrent, vector<string> &vPlateInfo, int iFrameIndex);

	void getFiles(vector<string> &vFileNameList);

private:

	INT32 m_iLPRNumber;

	CFragmentAnalyze *m_pFragmentAnalyze;
	CFrameDecode     *m_pFrameDecode;
	CImageFilter     *m_pImageFilter;
	CPlateRecognize  *m_pPlateRecognize;

	vector<INT32> m_vecMovingIndex;
	map<string, vector<INT32> > m_mapResult;
};


#endif /* PRSRC_SYSTEMPROCESSOR_H_ */
