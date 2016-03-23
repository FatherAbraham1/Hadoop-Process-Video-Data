
# 编译生成hadoop streaming的map函数的静态库。
#该静态库MapperVideo，依赖于ffmpeg文件夹和opencv文件夹中的一些库,
#利用pkg-config进行编译

all: clean MapperVideo ReducerVideo

clean:
	rm -f MapperVideo
	rm -f ReducerVideo

MapperVideo: 
	g++  -o MapperVideo MapperVideo.cpp -I../ffmpeg/include/ -L../ffmpeg/lib/ -lFragmentDecode -I../opencv/opencv_static/include/ -L../opencv/opencv_static/lib/ -lLPRProcess `pkg-config --cflags --libs libavcodec libavfilter libavutil libswscale libavdevice libavformat libswresample opencv` -lpthread

ReducerVideo:
	g++  -o ReducerVideo ReducerVideo.cpp

