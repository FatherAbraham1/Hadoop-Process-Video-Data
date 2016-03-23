#include<stdlib.h>
#include <byteswap.h>
#include <cstdlib>
#include <sys/stat.h>
#include <typeinfo>
#include <fstream>  
#include <iostream>  
#include "SystemProcessor.h"
#include "sys/time.h"
#include <sstream>
using namespace std;  


/*
 * MapperVideo.cpp 为 hadoop streaming 的  map 函数。
 */


/*
 *类FileInUtil，用来解析从Java类ContentRecordReader中读取的key, value.
 *注意解析前的key和value是由Rawbytes生成的，key和value均用4个字节的长度+原始字节表示。
 */



class FileInUtil {
 public:
  static bool ReadInt(unsigned int *len, istream &stream) {
   if(!stream.read((char *)len, sizeof(unsigned int)))
    return false;
   *len = bswap_32(*len);
   return true;
  }
 
  static bool ReadString(string &str, istream &stream) {
   unsigned int len;
   if(!ReadInt(&len, stream))
    return false;
   str.resize(len);
   if(!ReadBytes(&str[0], len, stream))
    return false;
   return true;
  }
 
  static bool ReadBytes(char *ptr, unsigned int len, istream &stream) {
   stream.read(ptr, sizeof(unsigned char) * len);
   if(stream.eof()) return false;
   return true;
  }
};


/*
 *类FileOutUtil，把map函数生成的key和value，分别解析成4个字节的长度+原始字节，
 *然后以SequenceFileOutputFormat的格式，写入到HDFS文件系统中。
 *
 */


class FileOutUtil {
    public:
		static bool WriteInt(unsigned int *len,ostream &stream) {
			*len = bswap_32(*len);
			stream.write((char *)len, sizeof(unsigned int));
			return true;
		}
        static bool WriteString(string &str, ostream &stream) {
            unsigned int len;
			len = str.length();
			WriteInt(&len, stream);
            WriteBytes(&str[0], str.length(), stream);
			return true;
        }
		static bool WriteBytes(char *ptr, unsigned int len, ostream &stream) {
			stream.write(ptr, sizeof(unsigned char) * len);
			return true;
		}
};

class LPRUtil {

	private:

		string key, value, PlateInfo, ImageData;
		string ssNow;
		int inum;
		CSystemProcessor CSP;

	public:

		LPRUtil(string k, string v, string P, string I, int num) {
			key = k;
			value = v;
			PlateInfo = P;
			ImageData = I;
			inum = num;
		}
	
		~LPRUtil() {}

		bool parseKeyValue() {

		    /*
			 * 从Rawbytes中解析出key, value值
			 */

    		if(! FileInUtil::ReadString(key, cin)) {
				cerr << "the key of inputdata is wrong!" << endl;
				return false;
			} 
    		if(! FileInUtil::ReadString(value, cin)) {
				cerr << "the value of inputdata is wrong!" << endl;
				return false;
			} 
			if(!LPRProcess()) {
				cerr << "LPR process is wrong!" << endl;
				return false;
			}
			return true;
		}

		bool LPRProcess() {

			/*
			 *函数FragmentProcess(value, inum)为车牌识别中处理GOP的C++接口，
			 *其中value代表二进制的GOP， inum代表该GOP经车牌识别系统处理后生成
			 *图片的个数。
			 */

			if(!CSP.FragmentProcess(value, inum)) 
				return false;
			if(!LPRGetImage())
				return false;
			return true;
		}

		bool LPRGetImage() {
			for(int i=1; i<=inum; i++) { 

				/*
				 *函数GetImageInfo(PlateInfo, ImageData)获取车牌识别后的每一个图片的C++接口，
				 *其中PlateInfo代表该图片的名称，ImageData代表该图片的二进制内容。
				 */

				if(!CSP.GetImageInfo(PlateInfo, ImageData)) 
					return false;
				if(! outPutKeyValue()) {
					return false;
				}
			}
			return true;
		}

		bool outPutKeyValue() {

	        /*
			 * 把key,value值解析成Rawbytes的格式，并输出到HDFS上。
			 * 其中车牌号为key, 车牌图片为value.
			 */

			string PlateInfoNT = PlateInfo + "_" + nowTime();
			if(! FileOutUtil::WriteString(PlateInfoNT, cout)) {
				cerr << "the key of outputdata is wrong!" << endl;
				return false;
			} 
        	if(! FileOutUtil::WriteString(ImageData, cout)) {
				cerr << "the value of outputdata is wrong!" << endl;
				return false;
			} 
			return true;
		}


		/*
		 * 获取系统当前的时间
		 */
		
		string nowTime() {
			struct   timeval   start;
			gettimeofday(&start,0);
    		stringstream ss;
    		ss << start.tv_usec + start.tv_sec;
			ss >> ssNow;
			return ssNow;
		}
	
	
};


/*
 * main函数为streaming的map函数的入口，定义string类型的key和value，分别存储从ContentRecordReader中读取的.
 * key和value值。
 */

int main() {   
	string key, value;
	cerr << "will be doing map ..." << endl;
	bool isExit = false;
	while(!cin.eof() && cin.good()) {
		cerr << "doing map..." << endl;
		string PlateInfo, ImageData;
		int inum;
		LPRUtil LPR(key, value, PlateInfo, ImageData, inum);
		if(! LPR.parseKeyValue()) {
			cerr << "exited..." << endl;
			isExit = true;
			break;
		}
		cerr << "map done..." << endl;
	}
	if (isExit) {	
		cerr << "has exited >>>" << endl;
	}

	return 0;
} 
