/*
 * RecognizeDef.h
 *
 *  Created on: Dec 19, 2014
 *      Author: xuh14
 */

#ifndef PRSRC_INCLUDE_RECOGNIZEDEF_H_
#define PRSRC_INCLUDE_RECOGNIZEDEF_H_

#include "FilterDef.h"
#include <sys/time.h>

//ä¸­å›½è½¦ç‰Œ
const char strCharacters[] = {
		'0', '1', '2', '3', '4',
		'5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E',    //  æ²¡æœ‰I å’Œ O
		'F', 'G', 'H', 'J', 'K',
		'L', 'M', 'N', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V',
		'W', 'X', 'Y', 'Z'
};

const int numCharacter = 34; /* æ²¡æœ‰Iå’ŒO,10ä¸ªæ•°å­—ä¸Ž24ä¸ªè‹±æ–‡å­—ç¬¦ä¹‹å’Œ */

//ä»¥ä¸‹éƒ½æ˜¯æˆ‘è®­ç»ƒæ—¶ç”¨åˆ°çš„ä¸­æ–‡å­—ç¬¦æ•°æ�®ï¼Œå¹¶ä¸�å…¨é�¢ï¼Œæœ‰äº›çœ�ä»½æ²¡æœ‰è®­ç»ƒæ•°æ�®æ‰€ä»¥æ²¡æœ‰å­—ç¬¦
const string strChinese[] = {
		"zh_sx"    /* æ™‹ */,  "zh_e"    /* é„‚ */,  "zh_gan"  /* èµ£ */,  "zh_hei"   /* é»‘ */,
		"zh_hu"    /* æ²ª */,  "zh_ji"   /* å†€ */,  "zh_jl"   /* å�‰ */,  "zh_jin"   /* æ´¥ */,
		"zh_jing"  /* äº¬ */,  "zh_shan" /* é™• */,  "zh_liao" /* è¾½ */,  "zh_lu"    /* é²� */,
		"zh_min"   /* é—½ */,  "zh_ning" /* å®� */,  "zh_su"   /* è‹� */,  "zh_chuan" /* å·� */,
		"zh_wan"   /* çš– */,  "zh_yu"   /* è±« */,  "zh_yue"  /* ç²¤ */,  "zh_zhe"   /* æµ™ */
};

typedef enum
{
	CHARS_DIGITAL = 0,
	CHARS_LETTER = 10,
	CHARS_CHINESE = 34
} ENUM_CHARS_TYPE;


const int numChinese = 20;
const int numAll = 54; /* 34+20=54 */

//! preprocessCharæ‰€ç”¨å¸¸é‡�
const int CHAR_SIZE                       = 20;
const int HORIZONTAL                      = 1;
const int VERTICAL                        = 0;

//! preprocessCharæ‰€ç”¨å¸¸é‡�
const int DEFAULT_LIUDING_SIZE            = 9;
const int DEFAULT_MAT_WIDTH               = 136;
const int DEFAULT_COLOR_THRESHOLD         = 130;
const float DEFAULT_BLUE_PERCEMT          = 0.3;
const float	DEFAULT_WHITE_PERCEMT         = 0.1;

const char* const DEFAULT_SVM_MODEL_PATH  = "svm.xml";
const char* const DEFAULT_ANN_MODEL_PATH  = "ann.xml";
const char* const DEFAULT_OUTPUT_PATH     = "/tmp/FragmentTmpDir/";

//! PlateLocateæ‰€ç”¨å¸¸é‡�
const int DEFAULT_GAUSSIANBLUR_SIZE       = 5;
const int SOBEL_SCALE                     = 1;
const int SOBEL_DELTA                     = 0;
const int SOBEL_DDEPTH                    = CV_16S;
const int SOBEL_X_WEIGHT                  = 1;
const int SOBEL_Y_WEIGHT                  = 0 ;

const int DEFAULT_MORPH_SIZE_WIDTH        = 7; // 11 , 7
const int DEFAULT_MORPH_SIZE_HEIGHT       = 3; // 9 , 3

//! showResultMatæ‰€ç”¨å¸¸é‡�
const int WIDTH                           = 136;
const int HEIGHT                          = 36;
const int TYPE                            = CV_8UC3;

//! verifySizeæ‰€ç”¨å¸¸é‡�
const int DEFAULT_VERIFY_MIN              = 1;
const int DEFAULT_VERIFY_MAX              = 10;

//! è§’åº¦åˆ¤æ–­æ‰€ç”¨å¸¸é‡�
const int DEFAULT_ANGLE                   = 30;

//! æ˜¯å�¦å¼€å�¯è°ƒè¯•æ¨¡å¼�å¸¸é‡�ï¼Œé»˜è®¤0ä»£è¡¨å…³é—­
const int DEFAULT_DEBUG                   = 0;
const int DEBUG_DATA_COLLECTED            = 0;

const char* const ANN_COLLECTED_CN_PATH   = "traindata/ann/chinese/";
const char* const ANN_COLLECTED_EN_PATH   = "traindata/ann/letter/";
const char* const SVM_COLLECTED_HAS_PATH  = "traindata/svm/has/";
const char* const SVM_COLLECTED_NO_PATH   = "traindata/svm/no/";


const float DEFAULT_ERROR                 = 0.6;
const float DEFAULT_ASPECT                = 3.75;

const float MAX_H_CHECK_AREA              = 1;
const float MIN_H_CHECK_AREA              = 0.5;
const float MAX_W_CHECK_AREA              = 1;
const float MIN_W_CHECK_AREA              = 0;

#endif /* PRSRC_INCLUDE_RECOGNIZEDEF_H_ */
