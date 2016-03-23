package org.apache.hadoop.streaming;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapred.FileSplit;


/*
 * 此类用在类ContentInputFormat中，是自定义的输入分片，该分片不仅包括FileSplit的功能，而且还包含GOP的信息。
 */

public class CustomInputSplit extends FileSplit implements Writable{

	/*
	 * 变量GOPInfo，定义了GOP的结构。key为每个GOP的offset和size，存在一个ArrayList中，value为该GOP的头信息，存在byte[]中。
	 */
	private HashMap<ArrayList<Long>, byte[]> GOPInfo = new HashMap<ArrayList<Long>, byte[]>();
	
	public CustomInputSplit() {
		
	}
	
	public CustomInputSplit(Path file, long start, long length, String[] hosts) {
		super(file, start, length, hosts);
	}
	
	/*
	 * 包含GOP的分片CustomInputSplit
	 */
	
	public CustomInputSplit(Path file, long start, long length, String[] hosts, HashMap<ArrayList<Long>, byte[]> theSplitGOPInfo) {
		super(file, start, length, hosts);
		this.GOPInfo = theSplitGOPInfo;
	}
	
	/*
	 * 利用函数getGOPInfo，可以从类ContentRecordReader中获取GOP信息。
	 */
	
	public HashMap<ArrayList<Long>, byte[]> getGOPInfo() {
		return GOPInfo;
	}
	
	/*
	 * (non-Javadoc)
	 * @see org.apache.hadoop.mapred.FileSplit#readFields(java.io.DataInput)
	 * 
	 * 实现Writable时，必须重新实现函数readFields()和write()，这样才可以把自己的GOP信息发送到各个节点上。
	 */
	
	public void readFields(DataInput in) throws IOException {
		super.readFields(in);
		ArrayList<Long> GopStartAndEnd;
		byte[] tmp;
		int GOPNumsOfThisSplit = in.readInt(); 
		GOPInfo.clear();
		for (int i = 0; i < GOPNumsOfThisSplit; i++) {
			GopStartAndEnd = new ArrayList<Long>();
			long GOPstart = in.readLong();
			long GOPend = in.readLong();
			GopStartAndEnd.add(GOPstart);  
			GopStartAndEnd.add(GOPend);  
			int len = in.readInt();  
			tmp = new byte[len];
			in.readFully(tmp, 0, len); 
			GOPInfo.put(GopStartAndEnd, tmp);
		}
	}
	
	public void write(DataOutput out) throws IOException {
		super.write(out);
		out.writeInt(GOPInfo.size()); 
		for (Entry<ArrayList<Long>, byte[]> GOP : GOPInfo.entrySet()) {
			out.writeLong(GOP.getKey().get(0));  
			out.writeLong(GOP.getKey().get(1));  
			out.writeInt(GOP.getValue().length); 
			out.write(GOP.getValue(), 0, GOP.getValue().length);
		}
	}
	
}
