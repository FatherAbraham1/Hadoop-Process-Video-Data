/*
 * IFragmentDecode.h
 *
 *  Created on: Dec 29, 2014
 *      Author: xuh14
 */

#ifndef VERIFY_UTIL_IFRAGMENTDECODE_H
#define VERIFY_UTIL_IFRAGMENTDECODE_H

#include <iostream>
#include <string>

using namespace std;

class IFragmentDecode
{

public:

	/**
	 * @brief		Constructor of IFragmentDecode.
	 * @param		no.
	 * @return		no.
	 */
	IFragmentDecode(){};

	/**
	 * @brief		Destructor of IFragmentDecode.
	 * @param		no
	 * @return		no
	 */
	virtual ~IFragmentDecode(){};

	/**
	 * @brief		Decode video fragment.
	 * @param[in]	<strFragmentData> The fragment data.
	 * @param[in]	<strInputPath> The input file path.
	 * @param[in]	<strOutputDir> The output picture path.
	 * @param[in]	<strPicFormat> The decode format of picture.
	 * @return		Function succeeds return 0, otherwise return error code.
	 */
	virtual int VideoFragmentDecode(string &strFragmentData, string strInputPath, string strOutputDir = "./result/", string strPicFormat = "yuv") = 0;

};

#endif /* VERIFY_UTIL_IFRAGMENTDECODE_H */
