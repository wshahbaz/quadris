#include "Level.h"

//to use block factory:
//The following are necessary to call the block factory
#include "AbstractBlockFactory.h"
#include "IBlock.h"
#include "JBlock.h"
#include "LBlock.h"
#include "OBlock.h"
#include "SBlock.h"
#include "ZBlock.h"
#include "TBlock.h"
#include "HintBlock.h"
#include "StarBlock.h"
#include "WBlock.h"
#include "VBlock.h"
#include "XBlock.h"
#include "Command.h"//for postMove
#include "BoardSubject.h"//for postMove

#include <memory>
#include <algorithm>    
#include <string>
#include <iostream> //for debug only
#include <stdlib.h>     /* srand, rand */

using namespace std;

void changeDefaultFilePath(std::string newFilePath) {
    defaultFilePath = newFilePath;
}

string defaultFilePath = "sequence.txt";
/////////////// BLOCK GENERATING STRATEGIES ///////////////

string GenBlock::getFileName() {
    return "";
}
///////////////////////////
//GenBlockFromFile
//////////////////////////
GenBlockFromFile::GenBlockFromFile(string path) {
    gb_ = make_shared<PGenBlockFromFile>();
    gb_->filename_ = path;
    //file reader stuff
    FileIO seq(gb_->filename_);
    //make blockSeq_: 
    gb_->blockSeq_ = seq.readToStringList();
    //current block to be generated is set to the beggining
    gb_->curr_ = gb_->blockSeq_.begin();
}

string GenBlockFromFile::getNextBlockType(){
    if (gb_->curr_ == gb_->blockSeq_.end()){
        //if reached end of file, start form beggining
        gb_->curr_ = gb_->blockSeq_.begin();
    }
    string ret = *(gb_->curr_);
    gb_->curr_++;
    return ret;
}
string GenBlockFromFile::getFileName(){
    return gb_->filename_;
}

bool GenBlockFromFile::isReadingFromFile() const {return true;}

///////////////////////////
//GenRandomBlock
//////////////////////////
GenRandomBlock::GenRandomBlock() {
    gb_ = make_shared<PGenRandomBlock>();
}

string GenRandomBlock::getNextBlockType() {
    //probability of getting any char we seen so far in the loop below
    double cumulativeProbabilty = 0;
    //the upper bound of random number we'll be generating
    //could be any number -- but 100 is more intuitive
    double ranUpperBound = 100;
    //random number generator
    // generate ran number < 100
    int randNum = rand() % 100;
    // figure out what char this num correspond to
    for (auto it = gb_->blockProbabilities_.begin(); it != gb_->blockProbabilities_.end(); ++it) { 
        cumulativeProbabilty += it->second;
        if (randNum < cumulativeProbabilty * 100) {
            string ret(1, it->first);
            return ret;
        }
    } 
    // execution shouln't reach here
    // maximum of randNum is 99
    // cumulativeProbabilty is guarenteed to be really close to 1 at the end
}

bool GenRandomBlock::isReadingFromFile() const {return false;}

///////////////////////////
//GenRandomBlockLv1
//////////////////////////
GenRandomBlockLvl1::GenRandomBlockLvl1(){
    double loProb = 1.0/12.0;
    double hiProb = 1.0/6.0;
    // we only have 2 probabilities in all current levels
    // so we can, technically use two member variables: hi and lo to indicate them
    // instead of using a map
    // but we need to accomodate future levels with potentially more than 2 types of probabilities
    gb_->blockProbabilities_.insert(pair<char, double>('S', loProb));
    gb_->blockProbabilities_.insert(pair<char, double>('Z', loProb));
    gb_->blockProbabilities_.insert(pair<char, double>('I', hiProb));
    gb_->blockProbabilities_.insert(pair<char, double>('J', hiProb));
    gb_->blockProbabilities_.insert(pair<char, double>('L', hiProb));
    gb_->blockProbabilities_.insert(pair<char, double>('O', hiProb));
    gb_->blockProbabilities_.insert(pair<char, double>('T', hiProb));
}

///////////////////////////
//GenRandomBlockLv2
//////////////////////////

GenRandomBlockLvl2::GenRandomBlockLvl2(){
    double prob = 1.0/7.0;
    vector<char> blocks = {'S', 'Z', 'I', 'J', 'L', 'O', 'T'};
    for (auto b: blocks) gb_->blockProbabilities_.insert(pair<char, double>(b, prob));
}

///////////////////////////
//GenRandomBlockLv3
//////////////////////////

GenRandomBlockLvl3::GenRandomBlockLvl3(){
    double loProb = 1.0/9.0;
    double hiProb = 2.0/9.0;
    // we only have 2 probabilities in all current levels
    // so we can, technically use two member variables: hi and lo to indicate them
    // instead of using a map
    // but we need to accomodate future levels with potentially more than 2 types of probabilities
    gb_->blockProbabilities_.insert(pair<char, double>('S', hiProb));
    gb_->blockProbabilities_.insert(pair<char, double>('Z', hiProb));
    gb_->blockProbabilities_.insert(pair<char, double>('I', loProb));
    gb_->blockProbabilities_.insert(pair<char, double>('J', loProb));
    gb_->blockProbabilities_.insert(pair<char, double>('L', loProb));
    gb_->blockProbabilities_.insert(pair<char, double>('O', loProb));
    gb_->blockProbabilities_.insert(pair<char, double>('T', loProb));
}

///////////////////////////
//GenRandomBlockLv5
//////////////////////////

GenRandomBlockLvl5::GenRandomBlockLvl5(){
    double prob = 1.0/10.0;
    vector<char> blocks = {'S', 'Z', 'I', 'J', 'L', 'O', 'T', 'V', 'W', 'X'};
    for (auto b: blocks) gb_->blockProbabilities_.insert(pair<char, double>(b, prob));
}

/////////////// LEVELS ///////////////

///////////////////////////
//Level base class
//////////////////////////
PImplLevel::PImplLevel() : blockFactory_ (BlockFactorySingleton::getInstance()){}

// level base class function
void Level::postMove(shared_ptr<Block> block, shared_ptr<BoardSubject> bd){}

void Level::postDrop(shared_ptr<BoardSubject> bd){}

void Level::resetCounter() {}

shared_ptr<Block> Level::nextBlock(){
    string nextType = lv_->blockGen_->getNextBlockType();
    return lv_->blockFactory_.createBlock(nextType + "Block", whatLevel());
}

void Level::switchToReadFile(string path){}
void Level::switchToRandom(){}

bool Level::isReadingFromFile(){
    return lv_->blockGen_->isReadingFromFile();
}

string Level::getFileName(){
    return lv_->blockGen_->getFileName();
}

void Level::reset(){}; 
///////////////////////////
// level code 
///////////////////////////

Level0::Level0() {
    lv_ = make_shared<PImplLevel> ();
    lv_->blockGen_ = make_shared<GenBlockFromFile>();
}

void Level0::reset(){
    lv_->blockGen_ = make_shared<GenBlockFromFile>();
}

unsigned int Level0::whatLevel() const {return 0;}

Level1::Level1() {
    lv_ = make_shared<PImplLevel> ();
    lv_->blockGen_ = make_shared<GenRandomBlockLvl1> ();
}

unsigned int Level1::whatLevel() const {return 1;}

Level2::Level2() {
    lv_ = make_shared<PImplLevel> ();
    lv_->blockGen_ = make_shared<GenRandomBlockLvl2> ();
}

unsigned int Level2::whatLevel() const {return 2;}

Level3::Level3() {
    lv_ = make_shared<PImplLevel> ();
    lv_->blockGen_ = make_shared<GenRandomBlockLvl3> ();
}

void Level3::switchToReadFile(string path){
    //if we are not rlready reading from file
    if (!lv_->blockGen_->isReadingFromFile()) {
        lv_->blockGen_ = make_shared<GenBlockFromFile>(path);
    } 
}

void Level3::switchToRandom(){
    //if we are not rlready random
    if (lv_->blockGen_->isReadingFromFile()) {
        lv_->blockGen_ = make_shared<GenRandomBlockLvl3>();
    }
}

void Level3::postMove(shared_ptr<Block> block, shared_ptr<BoardSubject> bd){
    //assumption: curBlock is floatingBlock
    block->down();
    if(!bd->isBlockPositionLegal())//undo illegal move
    {
        block->up();
    }
}

unsigned int Level3::whatLevel() const {return 3;}

Level4::Level4() {
    lv_ = make_shared<PImplLevel> ();
    dropCounter_ = 0;//floatingBlock and next block were generated at a different level
    //use same random function
    lv_->blockGen_ = make_shared<GenRandomBlockLvl3> ();
}

void Level4::postDrop(shared_ptr<BoardSubject> bd)
{
    ++dropCounter_;
    if (dropCounter_ == 5) 
    {
        //next block is *
        shared_ptr<Block> floatingBlock = bd->getFloatingBlockInstance();
        bd->specificBlock("StarBlock");
        bd->drop(true);
        bd->fixFloatingBlock(floatingBlock);
        dropCounter_=0;
    }
}

//Only called after a row is cleared
//Value of -1 ensures that the block that cleared the row 
//is not counted
void Level4::resetCounter()
{
    dropCounter_ = -1;
}

void Level4::reset(){
	dropCounter_ = 0;
}
unsigned int Level4::whatLevel() const {return 4;}

Level5::Level5() {
    lv_ = make_shared<PImplLevel> ();
    lv_->blockGen_ = make_shared<GenRandomBlockLvl5> ();
}

unsigned int Level5::whatLevel() const {return 5;}
