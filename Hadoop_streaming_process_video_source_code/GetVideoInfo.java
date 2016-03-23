package org.apache.hadoop.streaming;

import java.util.ArrayList;
import java.util.HashMap;

/*
 * 此类用在类ContentInputFormat中，是JNI的Java接口，通过它可以获取每个分片的GOP的信息。
 * 
 */

public class GetVideoInfo {
	
	/*
	 * 函数setSplitNum()
	 * 输入参数：
	 * 参数MP4TotalSize表示待处理mp4的字节总长度，
	 * 参数splitNum表示分片的数目。
	 * 输出参数：
	 * void
	 */
	
	public native static void setSplitNum(long MP4TotalSize, int splitNum);
	
	/*
	 * 函数getVideoOffsetAndSizeAndFlag()
	 * 输入参数：
	 * 参数videoStream表示一定字节长度MP4流，其长度由offset和size决定
	 * 参数offset表示videoStream的字节偏移量
	 * 参数size表示videoSteam的字节大小
	 * 输出参数：
	 * long[]. 其中该数组中包括3个元素，
	 * 第1个元素代表C++接口对videoStream处理的情况，包括三种处理情况(PROCESS_NO_ERROR(1), PROCESS_ERROR(1001), PROCESS_COMPLETE(0))。
	 * 第2个元素代表下一个videoStream的offset
	 * 第3个元素代表下一个videoStream的size
	 */
	
	public native static long[] getVideoOffsetAndSizeAndFlag(byte[] videoStream, long offset, long size);
	
	/*
	 * 函数getSplitAndGOPInfo(),
	 * 输入参数：
	 * void
	 * 输出参数：
	 * 是一个大的HashMap：
	 * 其中key是每个split的offset和size（offset和size存放在ArrayList<Long>中），
	 * value是该split所包含的所有的GOP的信息，该信息存放在HashMap<ArrayList<Long>, byte[]>里，
	 * 其中每一个GOP的offset和size存放在ArrayList<Long>中，每一个GOP的头信息存放在byte[]中。
	 * 
	 */
	
	public native static HashMap<ArrayList<Long>, HashMap<ArrayList<Long>, byte[]>> getSplitAndGOPInfo(); 
	/*
	 * 在类中声明所调用的库名称
	 */
	
    static
    {
        System.loadLibrary("InputFormatSo");
    }

}
