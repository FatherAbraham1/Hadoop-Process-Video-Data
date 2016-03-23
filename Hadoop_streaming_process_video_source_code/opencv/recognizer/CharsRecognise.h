/*
 * CharsRecognise.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_CHARSRECOGNISE_H_
#define PRSRC_RECOGNIZER_CHARSRECOGNISE_H_


#include "RecognizeDef.h"
#include "CharsSegment.h"
#include "CharsIdentify.h"


class CCharsRecognise
{
public:
	CCharsRecognise();

	//! 字符分割与识别
	int charsRecognise(Mat, String&, int);

	//! 装载ANN模型
	void LoadANN(string s);

	//! 获得车牌颜色
	inline string getPlateType(Mat input) const
	{
		string color = "未知";
		int result = m_charsSegment->getPlateType(input);
		if (1 == result)
		{
			color = "蓝牌";
		}
		if (2 == result)
		{
			color = "黄牌";
		}
		return color;
	}

	//! 设置变量
	inline void setLiuDingSize(int param){ m_charsSegment->setLiuDingSize(param);}

	inline void setColorThreshold(int param){ m_charsSegment->setColorThreshold(param);}

	inline void setBluePercent(float param){ m_charsSegment->setBluePercent(param);}

	inline float getBluePercent() const { return m_charsSegment->getBluePercent();}

	inline void setWhitePercent(float param){ m_charsSegment->setWhitePercent(param);}

	inline float getWhitePercent() const { return m_charsSegment->getWhitePercent();}

private:
	//！字符分割
	CCharsSegment* m_charsSegment;

	//! 字符识别
	CCharsIdentify* m_charsIdentify;
};


#endif /* PRSRC_RECOGNIZER_CHARSRECOGNISE_H_ */
