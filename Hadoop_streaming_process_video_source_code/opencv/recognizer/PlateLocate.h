/*
 * PlateLocate.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_PLATELOCATE_H_
#define PRSRC_RECOGNIZER_PLATELOCATE_H_

#include "RecognizeDef.h"

class CPlateLocate
{
public:

	CPlateLocate();

	//! 车牌定位
	int plateLocate(Mat, vector<Mat>& );

	//! 车牌的尺寸验证
	bool verifySizes(RotatedRect mr);

	//! 结果车牌显示
	Mat showResultMat(Mat src, Size rect_size, Point2f center, int index);

	//! 设置与读取变量
	inline void setGaussianBlurSize(int param){ m_GaussianBlurSize = param;}
	inline int getGaussianBlurSize() const{ return m_GaussianBlurSize;}

	inline void setMorphSizeWidth(int param){ m_MorphSizeWidth = param;}
	inline int getMorphSizeWidth() const{ return m_MorphSizeWidth;}

	inline void setMorphSizeHeight(int param){ m_MorphSizeHeight = param;}
	inline int getMorphSizeHeight() const{ return m_MorphSizeHeight;}

	inline void setVerifyError(float param){ m_error = param;}
	inline float getVerifyError() const { return m_error;}
	inline void setVerifyAspect(float param){ m_aspect = param;}
	inline float getVerifyAspect() const { return m_aspect;}

	inline void setVerifyMin(int param){ m_verifyMin = param;}
	inline void setVerifyMax(int param){ m_verifyMax = param;}

	inline void setJudgeAngle(int param){ m_angle = param;}

	inline void setDebug(int param){ m_debug = param;}

protected:
	//! 高斯模糊所用变量
	int m_GaussianBlurSize;

	//! 连接操作所用变量
	int m_MorphSizeWidth;
	int m_MorphSizeHeight;

	//! verifySize所用变量
	float m_error;
	float m_aspect;
	int m_verifyMin;
	int m_verifyMax;

	//! 角度判断所用变量
	int m_angle;

	//! 是否开启调试模式，0关闭，非0开启
	int m_debug;
};


#endif /* PRSRC_RECOGNIZER_PLATELOCATE_H_ */
