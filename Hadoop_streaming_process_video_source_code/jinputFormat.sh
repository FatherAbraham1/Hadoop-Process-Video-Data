#!/bin/bash

javac -classpath "classes/*" GetVideoInfo.java
jar -cvf GetVideoInfo.jar GetVideoInfo.class

javac -classpath "classes/*"  CustomInputSplit.java
jar -cvf CustomInputSplit.jar CustomInputSplit.class

javac -classpath "classes/*:./GetVideoInfo.jar:./CustomInputSplit.jar" ./Content*.java ./GetVideoInfo.java ./CustomInputSplit.java -Xlint:deprecation
mkdir -p org/apache/hadoop/streaming
cp ./Content*.class ./GetVideoInfo.class ./CustomInputSplit.class  org/apache/hadoop/streaming
jar uf hadoop-streaming-2.5.1.jar org/apache/hadoop/streaming/Content*.class org/apache/hadoop/streaming/GetVideoInfo.class org/apache/hadoop/streaming/CustomInputSplit.class

rm org -rf
rm CustomInputSplit.jar GetVideoInfo.jar
rm *.class
