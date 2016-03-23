/*
 * ImageFilter.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_FILTER_IMAGEFILTER_H_
#define PRSRC_FILTER_IMAGEFILTER_H_

#include "FilterDef.h"

class CImageFilter
{

public:

	CImageFilter();

	virtual ~CImageFilter();

	BOOL MovementJudge(Mat matCurrent, Mat matPrevious);

private:

	INT32 Otsu(IplImage* src);

	BOOL VerifySizes(double dArea);

private:

	INT32 m_iIndex;

};

#endif /* PRSRC_FILTER_IMAGEFILTER_H_ */
