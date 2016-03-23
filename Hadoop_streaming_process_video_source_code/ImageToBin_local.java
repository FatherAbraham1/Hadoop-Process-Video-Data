package imageChange;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.util.ReflectionUtils;


/*
 * 解析HDFS上的SequenceFile，生成车牌名和车牌图片，并存储在本地文件系统中。
 */

public class ImageToBin_local {
    @SuppressWarnings("deprecation")
	public static void main(String args[]) throws Exception {
		String keyName;
		int plateNum=1;
        Configuration conf = new Configuration();     
        FileSystem fs = FileSystem.get(URI.create(args[0]),conf);
        SequenceFile.Reader reader = null;
        FileOutputStream out = null;
		System.out.println("HDFS文件输入路径:");
		System.out.println(args[0]);
		System.out.println("本地文件输出路径:");
		System.out.println(args[1]);
		
		System.out.println("车牌号码:");
        Path inPath = new Path(args[0]);    
        /*
         * pattern = "part", 表明只匹配HDFS目录中含有part的文件。
         */
        String pattern = "part";
        
        List<Path> paths = ImageToBin_local.getFilesUnderFolder(fs, inPath, pattern);
        
        for (Path path : paths) {
        	try{
    	        reader = new SequenceFile.Reader(fs, path, conf);
    			BytesWritable key = (BytesWritable)ReflectionUtils.newInstance(reader.getKeyClass(), conf);
    		    BytesWritable value = (BytesWritable)ReflectionUtils.newInstance(reader.getValueClass(), conf);
    		    while(reader.next(key, value)) {
    				key.setCapacity(key.getLength()); // 将后面多余的空间去除掉
    				keyName = new String(key.getBytes()); // BytesWritable 转成 String类型
    				
    				File dir = new File(args[1]);
    				if(dir.exists()) {
    					dir.delete();
    				}
    				dir.mkdir();
    				File file = new File(args[1] + "/picture" + Integer.toString(plateNum++) + "_" + keyName + ".bmp");
    				out = new FileOutputStream(file);
    			
    				System.out.println("keyName : " + keyName); // 输出车牌名
    		    	value.setCapacity(value.getLength());
    		    	System.out.println("value length : " + value.getLength());
    		    	out.write(value.getBytes(), 0, value.getLength()); //把车牌内容输出到file文件中
    		    	System.out.println("--------right--------");
    		    	out.close();}
            }catch (Exception e) {
            	System.out.println("--------wrong--------");
                System.out.println("Exception MESSAGES = "+e.getMessage());
            }
            finally {
                System.out.println("last line of the code....!!!!!!!!!!");
            }
		}
        reader.close();
    }
    
    /*
     * 获取一个目录下符合正则pattern模式的所有文件的路径列表
     */
    public static List<Path> getFilesUnderFolder(FileSystem fs, Path folderPath, String pattern) throws IOException {  
        List<Path> paths = new ArrayList<Path>();  
        if (fs.exists(folderPath)) {  
            FileStatus[] fileStatus = fs.listStatus(folderPath);  
            for (int i = 0; i < fileStatus.length; i++) {  
                FileStatus fileStatu = fileStatus[i];  
                // 如果是目录，递归向下找  
                if (!fileStatu.isDirectory()) {  
                    Path oneFilePath = fileStatu.getPath();  
                    if (pattern == null) {  
                        paths.add(oneFilePath);  
                    } else {  
                        if (oneFilePath.getName().contains(pattern)) {  
                            paths.add(oneFilePath);  
                        }  
                    }         
                }  else {
					System.out.println(fileStatu.getPath() + " is a directory!");
				}
            }  
        }  
        return paths;  
    } 
}

