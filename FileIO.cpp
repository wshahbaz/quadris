#include "FileIO.h"
#include <stdexcept> // for std::runtime_error
#include <vector>
#include <iostream>
using namespace std;

FileIO::FileIO(const string filename){
    ifs_.open(filename);
    if (!ifs_){
        //notify user of error and terminate
        cerr << "Could not open file: " << filename << ". Exiting now" << endl; 
        exit(1);
    }
}

FileIO::~FileIO() {
    ifs_.close();
}

vector<string> FileIO::readToStringList() {
    vector<string> ret;
    string temp;
    while (ifs_ >> temp) ret.push_back(temp);
    return ret;
}

bool FileIO::isFinished() {
    return ifs_.eof();
}

ifstream& FileIO::getStream() {
    return ifs_;
}