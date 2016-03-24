# Hadoop-Process-Video-Data

This project is used to deal with this problem: how to use Hadoop to process video data, such as license plate recognition from big video data. this project includes several techniques: Hadoop Streaming, JNI, ffmpeg, and machine learning.

# Hardware environment  
This program is compiled and ran on ubuntu 13.10 or ubuntu 14.04.

# Compile the application  
enter the directory: Hadoop_streaming_process_video_source_code, compile it as followings.  
1. to generate "MapperVideo" from C++ source code, do it as followings.  
(1) sudo mkdir /lib/pkgconfig/  
(2) vi /etc/profile, then add export PKG_CONFIG_PATH=/lib/pkgconfig/  
(3) source /etc/profile  
(4) cp pkgconfig/* /lib/pkgconfig/  
(5) vi /lib/pkgconfig/opencv.pc, then change prefix to $pwd/opencv/opencv_static  
(6) enter the directory: main, then make.  
(7) main/MapperVideo.cpp is the main source file of C++ code.
