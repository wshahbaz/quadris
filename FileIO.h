#ifndef _FILEIO_
#define _FILEIO_

#include <fstream>
#include <vector>

class FileIO { 
       private: 
       std::ifstream ifs_;
       public: 
       FileIO(std::string filename);
       ~FileIO();
       // prevent copying and assignment 			
       FileIO(const FileIO &) = delete; 			
       FileIO& operator= (const FileIO &) = delete;
       //read string separated by space to a vector
       std::vector<std::string> readToStringList();
       bool isFinished();
       std::ifstream& getStream();
};

#endif