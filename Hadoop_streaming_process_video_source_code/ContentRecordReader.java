package org.apache.hadoop.streaming;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.RecordReader;


/*
 * 自定义的ContentRecordReader，在其next()方法中，由于key值对我们没用，故定义key值为空值，value值为GOP的信息
 * 读取key和value，并送入map函数处理。
 */

public class ContentRecordReader implements RecordReader<NullWritable,BytesWritable> {  
	public static final Log LOG =
		    LogFactory.getLog(ContentRecordReader.class);
	public static final String INPUT_DIR_RECURSIVE = org.apache.hadoop.mapreduce.lib.input.FileInputFormat.INPUT_DIR_RECURSIVE;
    private long start;    
    private long pos;    
    private long end;  
    private long len;
    private Path path;
    private long GOPNums;
    private int currentGOPNum;
    private byte[] buffer = null; 
    private byte[] GOPHeader = null;
    private Iterator<ArrayList<Long>> theSplitGOPInfoIterator;
    private HashMap<ArrayList<Long>, byte[]> theSplitGOPInfo = null;
    private FSDataInputStream fileIn = null;  
    
    
    /*
     * 注意：构造函数ContentRecordReader中的分片为自定义的CustomInputSplit
     */
    public ContentRecordReader(JobConf job,CustomInputSplit split) throws IOException{    

		path = split.getPath();  
		final FileSystem fs = path.getFileSystem(job);
		fileIn = fs.open(path);
        start = split.getStart();
        len = split.getLength();
        end = start + len;
		System.out.println("*** the split offset : " + start);
		System.out.println("*** the split size : " + len);
		this.currentGOPNum = 0;
		this.pos = start;
		System.out.println("*** the split's GOP numbers : " + split.getGOPInfo().size());
		theSplitGOPInfo = split.getGOPInfo();
		theSplitGOPInfoIterator = theSplitGOPInfo.keySet().iterator();
		GOPNums = theSplitGOPInfo.size();
    }    
    
    public NullWritable createKey() {    
        return NullWritable.get();    //ignore key
    }    
    
    public BytesWritable createValue() {    
        return new BytesWritable();  
    }    
    
    public long getPos() throws IOException{    
        return pos;    
    }    
    
    public float getProgress() {    //获取作业处理进度情况。
        if (start == end) {    
            return 0.0f;    
        } else {    
            return Math.min(1.0f, (pos - start) / (float)(end - start));    
        }    
    }    
    
/* get next key value pairs */   
    public boolean next(NullWritable key, BytesWritable value) throws IOException{
    	 if((pos < end) && (currentGOPNum < GOPNums) && theSplitGOPInfoIterator.hasNext()) {
    		 ArrayList<Long> GOPIndex = theSplitGOPInfoIterator.next();
    		 long startGOP = GOPIndex.get(0);
    		 long GOPSize = GOPIndex.get(1);
    		 GOPHeader = new byte[(int) GOPSize + theSplitGOPInfo.get(GOPIndex).length];
    		 byte[] GOPHeaderTemp = theSplitGOPInfo.get(GOPIndex);
    		 System.arraycopy(GOPHeaderTemp, 0, GOPHeader, 0, GOPHeaderTemp.length);
    		 buffer = new byte[(int) GOPSize];
    		 fileIn.readFully(startGOP,buffer);
    		 System.arraycopy(buffer, 0, GOPHeader, GOPHeaderTemp.length, buffer.length);
//    		 key.set(new Text(Long.toString(pos)));
    		 //value 为GOP的包含头的GOP的信息。
    		 value.set(GOPHeader, 0, GOPHeader.length);
    		 pos += GOPSize;
    		 currentGOPNum += 1;
    		 LOG.info("*** emc << this split end is : " + end  + " and its pos is : " + pos + " >> emc ***");
    		 return true;
    	 }
    	 else {
    		 return false;  
		}
    }    

    public void close() throws IOException{ 
        if(fileIn != null) {    
            fileIn.close();    
        }    
    }   
}
