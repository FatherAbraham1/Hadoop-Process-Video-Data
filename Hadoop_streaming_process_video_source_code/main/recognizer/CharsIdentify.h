/*
 * CharsIdentify.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_CHARSIDENTIFY_H_
#define PRSRC_RECOGNIZER_CHARSIDENTIFY_H_

#include "RecognizeDef.h"

class CCharsIdentify
{
public:
	CCharsIdentify();

	//! 字符分割
	string charsIdentify(Mat input, ENUM_CHARS_TYPE enCharsType);

	//! 字符分类
	int classify(Mat f, ENUM_CHARS_TYPE enCharsType);

	//create the accumulation histograms,img is a binary image, t is 水平或垂直
	Mat ProjectedHistogram(Mat img, int t);

	//! 获得字符的特征图
	Mat features(Mat in, int sizeData);

	//! 装载ANN模型
	void LoadModel();

	//! 装载ANN模型
	void LoadModel(string s);

	//! 设置与读取模型路径
	inline void setModelPath(string path){	m_path = path;	}
	inline string getModelPath() const{	 return m_path;	}

private:
	//！使用的SVM模型
	CvANN_MLP ann;

	//! 模型存储路径
	string m_path;

	//! 特征尺寸
	int m_predictSize;

	//! 省份对应map
	map<string, string> m_map;
};

#endif /* PRSRC_RECOGNIZER_CHARSIDENTIFY_H_ */
