# Hadoop-Process-Video-Data

This project is used to deal with this problem: how to use Hadoop to process video data, such as license plate recognition from big video data. this project includes several techniques: Hadoop Streaming, JNI, ffmpeg, and machine learning.

## Hardware environment  
This program is compiled and ran on ubuntu 13.10 or ubuntu 14.04.

## Compile the application  
enter the directory: Hadoop_streaming_process_video_source_code, compile it as followings.  
1. to generate "MapperVideo" from C++ source code, do it as followings.  
  (1) `sudo mkdir /lib/pkgconfig/`  
  (2) `vi /etc/profile`, then add `export PKG_CONFIG_PATH=/lib/pkgconfig/ `  
  (3) `source /etc/profile`  
  (4) `cp pkgconfig/* /lib/pkgconfig/`  
  (5) `vi /lib/pkgconfig/opencv.pc`, then change `prefix` to `$pwd/opencv/opencv_static`  
  (6) enter the directory: main, then `make`.  
  (7) main/MapperVideo.cpp is the main source file of C++ code.  
2. to generate "ContentInputFormat.class,ContentRecordReader.class,GetVideoInfo.class,CustomInputSplit.class" from Java source code, do it as followings.  
  (1) `./jinputFormat.sh`  
  (2) ContentInputFormat.java; ContentRecordReader.java; GetVideoInfo.java; CustomInputSplit.java, these files are the source files of Java code.  
3. to generate "libInputFormatSo.so", do it as followings.  
  (1) `./cinputFormat.sh`  
  (2) GetVideoInfo.cpp; ParAbstract.cpp, are the corresponding source files.  
4. to parse the picture from sequencefile, you should generate "ImageToBin_local.class", do it as followings.  
  (1) `./parseImage.sh`  
  (2) ImageToBin_local.java is the corresponding source file.  
## Run this application
please enter the directory: Run_this_file, run it as followings.  
1. to run MapReduce application, do it as followings.  
  (1) `./runHadoop.sh`  
2. to parse the pictures of license plate from sequencefile on HDFS, do it as followings.  
  (1) `./imageTolocal.sh` 
