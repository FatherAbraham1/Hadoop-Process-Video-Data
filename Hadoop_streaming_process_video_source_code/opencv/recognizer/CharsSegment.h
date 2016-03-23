/*
 * CharsSegment.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_CHARSSEGMENT_H_
#define PRSRC_RECOGNIZER_CHARSSEGMENT_H_

#include "RecognizeDef.h"

class CCharsSegment
{
public:
	CCharsSegment();

	//! 字符分割
	int charsSegment(Mat, vector<Mat>&, int);

	//! 字符尺寸验证
	bool verifySizes(Mat r);

	//! 字符预处理
	Mat preprocessChar(Mat in);

	//! 生成直方图
	Mat ProjectedHistogram(Mat img, int t);

	//! 生成字符的特定特征
	Mat features(Mat in, int sizeData);

	//! 直方图均衡，为判断车牌颜色做准备
	Mat histeq(Mat in);

	//! 获得车牌颜色
	int getPlateType(Mat input);

	//! 去除影响字符识别的柳钉
	Mat clearLiuDing(Mat img);

	//! 根据特殊车牌来构造猜测中文字符的位置和大小
	Rect GetChineseRect(const Rect rectSpe);

	//! 找出指示城市的字符的Rect，例如苏A7003X，就是A的位置
	int GetSpecificRect(const vector<Rect>& vecRect);

	//! 这个函数做两个事情
	//  1.把特殊字符Rect左边的全部Rect去掉，后面再重建中文字符的位置。
	//  2.从特殊字符Rect开始，依次选择6个Rect，多余的舍去。
	int RebuildRect(const vector<Rect>& vecRect, vector<Rect>& outRect, int specIndex);

	//! 将Rect按位置从左到右进行排序
	int SortRect(const vector<Rect>& vecRect, vector<Rect>& out);

	//! 设置变量
	inline void setLiuDingSize(int param){ m_LiuDingSize = param;}
	inline void setColorThreshold(int param){ m_ColorThreshold = param;}

	inline void setBluePercent(float param){ m_BluePercent = param;}
	inline float getBluePercent() const { return m_BluePercent;}
	inline void setWhitePercent(float param){ m_WhitePercent = param;}
	inline float getWhitePercent() const { return m_WhitePercent;}

private:
	//！柳钉判断参数
	int m_LiuDingSize;

	//！车牌大小参数
	int m_theMatWidth;

	//！车牌颜色判断参数
	int m_ColorThreshold;
	float m_BluePercent;
	float m_WhitePercent;
};


#endif /* PRSRC_RECOGNIZER_CHARSSEGMENT_H_ */
