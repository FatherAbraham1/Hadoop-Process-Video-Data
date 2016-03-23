/*
 * GetVideoInfo.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: lij67
 */
#include "org_apache_hadoop_streaming_GetVideoInfo.h"

#include "ParAbstract.h"
#include <sys/stat.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace std;

/*
 * GetVideoInfo类中JNI接口的C++代码实现。
 */



/*
 * char* to jstring
 */
jstring chartoJstring(JNIEnv* env, const char* pat, long len)
{
       jclass strClass = env->FindClass("Ljava/lang/String;");
       jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
       jbyteArray bytes = env->NewByteArray(len);
       env->SetByteArrayRegion(bytes, 0, len, (jbyte*)pat);
       jstring encoding = env->NewStringUTF("utf-8");
       return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

/*
 * jstring to char*
 */

char* jstringToChar(JNIEnv *env, jstring jstr)
{
    char * rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte * ba = env->GetByteArrayElements(barr,JNI_FALSE);
    if(alen > 0)
    {
        rtn = (char*)malloc(alen+1); //new char[alen+1];
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements(barr,ba,0);

    return rtn;
}

/*
 *char * to jbyteArray
 */

jbyteArray char_to_jbyteArray(JNIEnv *env, const char* pat, jsize len) {
	jbyteArray jarray = env->NewByteArray(len);
	env->SetByteArrayRegion(jarray, 0, len, (jbyte*)pat);
	return jarray;
}

/*
 * jbyteArray to char*
 */

char* as_char_array(JNIEnv *env, jbyteArray array) {
    int len = env->GetArrayLength (array);
    char* buf = new char[len];
    env->GetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
    return buf;
}

/*
 * 实现GetVideoInfo.java中的函数setSplitNum().
 * 调用C++的(CParAbstract::getInstance())->init()接口。
 */


JNIEXPORT void JNICALL Java_org_apache_hadoop_streaming_GetVideoInfo_setSplitNum
  (JNIEnv *env, jclass, jlong totalSize, jint splitNum) {

	(CParAbstract::getInstance())->init((UINT32)totalSize, splitNum);

}

/*
 * 实现GetVideoInfo.java中的函数getVideoOffsetAndSizeAndFlag().
 * 调用C++的(CParAbstract::getInstance())->analyzeData()接口。
 */


JNIEXPORT jlongArray JNICALL Java_org_apache_hadoop_streaming_GetVideoInfo_getVideoOffsetAndSizeAndFlag
  (JNIEnv * env, jclass, jbyteArray jstr, jlong joffset, jlong jsize) {
	LONG offset = joffset;
	LONG size = jsize;
	int len = 3;
	char* videoStream = as_char_array(env, jstr);

	INT32 iRet = (CParAbstract::getInstance())->analyzeData(videoStream, offset, size);

	jlong *pointer = new jlong [len];
	pointer[0] = iRet;
	pointer[1] = offset;
	pointer[2] = size;
	jlongArray result = env-> NewLongArray (len);
	env-> SetLongArrayRegion (result, 0, len, pointer);
	delete [] pointer;
	delete [] videoStream;
	videoStream = NULL;
	return result;
}

/*
 *实现GetVideoInfo.java中的函数getSplitAndGOPInfo().
 *调用C++的(CParAbstract::getInstance())->getPartitionHeaderInfo()接口。
 */

JNIEXPORT jobject JNICALL Java_org_apache_hadoop_streaming_GetVideoInfo_getSplitAndGOPInfo
  (JNIEnv *env, jclass) {

	map< pair< LONG, LONG >, map< pair< LONG, LONG >, string > > partitionHeaderInfo;

	(CParAbstract::getInstance())->getPartitionHeaderInfo(partitionHeaderInfo);

	jclass jmapclass = env->FindClass("java/util/HashMap");
	jmethodID mid = env->GetMethodID(jmapclass, "<init>", "()V");
	jmethodID putmethod = env->GetMethodID(jmapclass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	jclass jarraylistclass = env->FindClass("java/util/ArrayList");
	jmethodID midlist = env->GetMethodID(jarraylistclass,"<init>","()V");
	jmethodID arrayList_add = env->GetMethodID(jarraylistclass,"add","(Ljava/lang/Object;)Z");
	jobject jmap = env->NewObject(jmapclass,mid);
	for (map<pair< LONG, LONG >, map< pair< LONG, LONG >, string > >::iterator Itor = partitionHeaderInfo.begin(); Itor != partitionHeaderInfo.end(); Itor++) {
		jobject jarraylist = env->NewObject(jarraylistclass,midlist); 
		jlong offset = (Itor->first).first;
		jclass Long_class = env->FindClass("java/lang/Long");
		jmethodID Long_init = env->GetMethodID(Long_class, "<init>", "(J)V");
		jobject Long_object_offset = env->NewObject(Long_class, Long_init, offset);

		env->CallVoidMethod(jarraylist, arrayList_add, Long_object_offset);
		jlong size = (Itor->first).second;
		jobject Long_object_size = env->NewObject(Long_class, Long_init, size);
		env->CallVoidMethod(jarraylist, arrayList_add, Long_object_size);
		jobject jmapChild = env->NewObject(jmapclass,mid);
		for (map< pair< LONG, LONG >, string >::iterator ItorChild = (Itor->second).begin(); ItorChild != (Itor->second).end(); ItorChild++) {
			jobject jarraylistChild = env->NewObject(jarraylistclass,midlist); 
			jlong offsetChild = (ItorChild->first).first;
			jobject Long_object_offsetChild = env->NewObject(Long_class, Long_init, offsetChild);
			env->CallVoidMethod(jarraylistChild, arrayList_add, Long_object_offsetChild);
			jlong sizeChild = (ItorChild->first).second;
			jobject Long_object_sizeChild = env->NewObject(Long_class, Long_init, sizeChild);
			env->CallVoidMethod(jarraylistChild, arrayList_add, Long_object_sizeChild);
			jbyteArray jGOPHead = char_to_jbyteArray(env, (ItorChild->second).c_str(), (ItorChild->second).length());
			env->CallVoidMethod(jmapChild, putmethod, jarraylistChild, jGOPHead);
		}
		env->CallVoidMethod(jmap, putmethod, jarraylist, jmapChild);
	}
	CParAbstract::deleteInstance();
	return jmap;
}

