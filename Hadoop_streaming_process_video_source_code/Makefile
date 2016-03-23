
#编译生成GetVideoInfo.java接口的动态库.

run:
	g++ -o libInputFormatSo.so ParAbstract.cpp GetVideoInfo.cpp -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -fPIC -shared #编译生成JNI所需要的动态库
	sudo cp libInputFormatSo.so $($HADOOP_INSTALL)/lib #把该动态库加入到hadoop的java.library.path路径，该路径默认为$HADOOP_INSTALL/lib/native
