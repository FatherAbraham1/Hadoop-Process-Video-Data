/*
 * PlateRecognize.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_PLATERECOGNIZE_H_
#define PRSRC_RECOGNIZER_PLATERECOGNIZE_H_

#include "RecognizeDef.h"
#include "PlateDetect.h"
#include "CharsRecognise.h"

class CPlateRecognize : public CPlateDetect, public CCharsRecognise
{
public:
	CPlateRecognize();

	//! 车牌检测与字符识别
	int plateRecognize(Mat, vector<string>&);

	////! 车牌定位与判断
	//int plateDetect(Mat, vector<Mat>&);

	////! 字符分割与识别
	//int charsRecognise(Mat, String&);

	////! 装载SVM
	//void LoadSVM(string s);

	////! 装载ANN模型
	//void LoadANN(string s);

private:
	////！车牌检测
	//CPlateDetect* m_plateDetect;

	////! 字符识别
	//CCharsRecognise* m_charsRecognise;
};

#endif /* PRSRC_RECOGNIZER_PLATERECOGNIZE_H_ */
