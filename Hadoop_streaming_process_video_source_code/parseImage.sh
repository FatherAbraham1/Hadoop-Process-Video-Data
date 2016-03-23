#!/bin/bash

javac -classpath "classes/*" ImageToBin_local.java
mkdir -p imageChange/
cp ImageToBin_local.class imageChange/
jar -cvf ImageToBin_local.jar imageChange/ImageToBin_local.class

rm *.class 
rm -r imageChange/
