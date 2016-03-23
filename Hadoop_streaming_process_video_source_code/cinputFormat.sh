#!/bin/bash
javac GetVideoInfo.java

mkdir -p org/apache/hadoop/streaming
cp ./GetVideoInfo.class org/apache/hadoop/streaming
javah -jni org.apache.hadoop.streaming.GetVideoInfo
rm -r org

make

rm *.class
