/*
 * PlateJudge.h
 *
 *  Created on: Dec 18, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_RECOGNIZER_PLATEJUDGE_H_
#define PRSRC_RECOGNIZER_PLATEJUDGE_H_

#include "RecognizeDef.h"

class CPlateJudge
{
public:
	CPlateJudge();

	//! 车牌判断
	int plateJudge(const vector<Mat>&, vector<Mat>&);

	//! 直方图均衡
	Mat histeq(Mat);

	//! 装载SVM模型
	void LoadModel();

	//! 装载SVM模型
	void LoadModel(string s);

	//! 设置与读取模型路径
	inline void setModelPath(string path){	m_path = path;	}
	inline string getModelPath() const{	 return m_path;	}

private:
	//！使用的SVM模型
	CvSVM svm;

	//! 模型存储路径
	string m_path;
};

#endif /* PRSRC_RECOGNIZER_PLATEJUDGE_H_ */
