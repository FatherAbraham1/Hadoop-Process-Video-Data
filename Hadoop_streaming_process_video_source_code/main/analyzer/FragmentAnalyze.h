/*
 * FragmentAnalyze.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_ANALYZER_FRAGMENTANALYZE_H_
#define PRSRC_ANALYZER_FRAGMENTANALYZE_H_

#include "AnalyzeDef.h"
#include <fstream>
#include <vector>

class CFragmentAnalyze
{

public:

	CFragmentAnalyze();

	virtual ~CFragmentAnalyze();

	INT32 FragmentAnalyze(string &strFragment, INT8 *pcPSets, INT32 &iSize, INT32 &iHdrSize, vector<STRUCT_FRAME_INFO> &vFrameInfo, INT32 &iMaxSize);

};



#endif /* PRSRC_ANALYZER_FRAGMENTANALYZE_H_ */
