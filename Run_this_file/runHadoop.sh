#!/bin/bash


#块注释
:<<eof
mapTaskNum代表MapReduce中map的个数;
reduceTaskNum代表MapReduce中reduce的个数;
inputPath代表HDFS的输入路径;
outputPath代表HDFS的输出路径;

-jt local
-fs local
inputPath=file:///tmp/mapinput1
outputPath=file:///tmp/output
上面四行命令，代表hadoop streaming的本地运行模式，该模式很方便代码测试，
如果本地运行没什么问题，可直接把inputPath, outputPath换成HDFS的文件系统路径，把-jt local和-fs local两行去掉，
这时就可以在分布式模式上跑了。
eof


#下面的程序，如果flag为true,则是本地运行模式；如果flag为false,则是分布式运行模式.

flag=true

mapTaskNum=4
reduceTaskNum=0

if $flag; then

 inputPath=file:///tmp/mapinput1
 outputPath=file:///tmp/output

 hadoop jar hadoop-streaming-2.5.1.jar \
 -D mapreduce.job.maps=$mapTaskNum \
 -D mapreduce.job.reduces=$reduceTaskNum \
 -D stream.map.input=rawbytes \
 -D stream.map.output=rawbytes \
 -D stream.reduce.input=rawbytes \
 -D stream.reduce.output=rawbytes \
 -D mapreduce.task.timeout=3600000 \
 -jt local \
 -fs local \
 -mapper MapperVideo \
 -file MapperVideo \
 -file libInputFormatSo.so \
 -file ann.xml \
 -file svm.xml \
 -inputformat ContentInputFormat \
 -input $inputPath \
 -outputformat org.apache.hadoop.mapred.SequenceFileOutputFormat \
 -output $outputPath

else

 inputPath=/mapinput1
 outputPath=/output

 hadoop jar hadoop-streaming-2.5.1.jar \
 -D mapreduce.job.maps=$mapTaskNum \
 -D mapreduce.job.reduces=$reduceTaskNum \
 -D stream.map.input=rawbytes \
 -D stream.map.output=rawbytes \
 -D stream.reduce.input=rawbytes \
 -D stream.reduce.output=rawbytes \
 -D mapreduce.task.timeout=3600000 \
 -mapper MapperVideo \
 -file MapperVideo \
 -file libInputFormatSo.so \
 -file ann.xml \
 -file svm.xml \
 -inputformat ContentInputFormat \
 -input $inputPath \
 -outputformat org.apache.hadoop.mapred.SequenceFileOutputFormat \
 -output $outputPath
fi
