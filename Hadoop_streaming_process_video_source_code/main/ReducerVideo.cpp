#include<stdlib.h>
#include <byteswap.h>
#include <cstdlib>
#include <sys/stat.h>
#include <typeinfo>
#include <fstream>  
#include <iostream>  
#include "sys/time.h"
#include <sstream>
using namespace std;  

class FileInUtil {
 public:
  static bool ReadInt(unsigned int *len, istream &stream) {
   if(!stream.read((char *)len, sizeof(unsigned int)))
    return false;
   *len = bswap_32(*len);
   return true;
  }
 
  static bool ReadString(string &str, istream &stream) {
   unsigned int len;
   if(!ReadInt(&len, stream))
    return false;
   str.resize(len);
   if(!ReadBytes(&str[0], len, stream))
    return false;
   return true;
  }
 
  static bool ReadBytes(char *ptr, unsigned int len, istream &stream) {
   stream.read(ptr, sizeof(unsigned char) * len);
   if(stream.eof()) return false;
   return true;
  }
};

class FileOutUtil {
    public:
		static bool WriteInt(unsigned int *len,ostream &stream) {
			*len = bswap_32(*len);
			stream.write((char *)len, sizeof(unsigned int));
			return true;
		}
        static bool WriteString(string &str, ostream &stream) {
            unsigned int len;
			len = str.length();
			WriteInt(&len, stream);
            WriteBytes(&str[0], str.length(), stream);
			return true;
        }
		static bool WriteBytes(char *ptr, unsigned int len, ostream &stream) {
			stream.write(ptr, sizeof(unsigned char) * len);
			return true;
		}
};

int main() {   

	string key, value; 
	while(!cin.eof()) {
	if(!FileInUtil::ReadString(key, cin))
		break;
	FileInUtil::ReadString(value, cin);

    FileOutUtil::WriteString(key, cout);
	FileOutUtil::WriteString(value, cout);
  }	
	return 0;
} 
