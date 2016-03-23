/*
 * PlateDetect.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_PLATEDETECT_H_
#define PRSRC_RECOGNIZER_PLATEDETECT_H_

#include "RecognizeDef.h"
#include "PlateLocate.h"
#include "PlateJudge.h"

class CPlateDetect
{
public:
	CPlateDetect();

	//! 车牌检测：车牌定位与判断
	int plateDetect(Mat, vector<Mat>&);

	//! 装载SVM模型
	void LoadSVM(string s);

	//! 设置与读取变量
	inline void setGaussianBlurSize(int param){	m_plateLocate->setGaussianBlurSize(param);}
	inline int getGaussianBlurSize() const{	return m_plateLocate->getGaussianBlurSize();}

	inline void setMorphSizeWidth(int param){m_plateLocate->setMorphSizeWidth(param);}
	inline int getMorphSizeWidth() const{return m_plateLocate->getMorphSizeWidth();}

	inline void setMorphSizeHeight(int param){m_plateLocate->setMorphSizeHeight(param);}
	inline int getMorphSizeHeight() const{return m_plateLocate->getMorphSizeHeight();}

	inline void setVerifyError(float param){m_plateLocate->setVerifyError(param);}
	inline float getVerifyError() const { return m_plateLocate->getVerifyError();}
	inline void setVerifyAspect(float param){m_plateLocate->setVerifyAspect(param);}
	inline float getVerifyAspect() const { return m_plateLocate->getVerifyAspect();}

	inline void setVerifyMin(int param){m_plateLocate->setVerifyMin(param);}
	inline void setVerifyMax(int param){m_plateLocate->setVerifyMax(param);}

	inline void setJudgeAngle(int param){m_plateLocate->setJudgeAngle(param);}

private:
	//！车牌定位
	CPlateLocate* m_plateLocate;

	//! 车牌判断
	CPlateJudge* m_plateJudge;
};


#endif /* PRSRC_RECOGNIZER_PLATEDETECT_H_ */
