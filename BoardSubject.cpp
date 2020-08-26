#include "BoardSubject.h"
#include "Command.h"
#include "Level.h"
#include "coordinate.h"
#include <memory>
#include <iostream> 
#include <cmath>
#include <thread>
using namespace std;

//initialize singleton bd_ to nullptr
shared_ptr<BoardSubject> BoardSubject::bd_ = nullptr;

//get BoardSubject instance 
shared_ptr<BoardSubject> BoardSubject::instance()
{
    if (bd_ == nullptr) 
    {
        bd_ = shared_ptr<BoardSubject>(new BoardSubject());
    }
    return bd_;
}


PImplBoard::PImplBoard() : cols_(11), rows_(15), resRows_(3), score_(0), highestLvl_(5), hiScore_(0), startlevel_(0)
{
    //prepare instances of levels during inittialization
    //could make a dedicated factory for levels...might over-complicate things
    levelMap_.insert(pair<unsigned int, shared_ptr<Level>>(0, shared_ptr<Level0>(new Level0())));
    levelMap_.insert(pair<unsigned int, shared_ptr<Level>>(1, shared_ptr<Level1>(new Level1())));
    levelMap_.insert(pair<unsigned int, shared_ptr<Level>>(2, shared_ptr<Level2>(new Level2())));
    levelMap_.insert(pair<unsigned int, shared_ptr<Level>>(3, shared_ptr<Level3>(new Level3())));
    levelMap_.insert(pair<unsigned int, shared_ptr<Level>>(4, shared_ptr<Level4>(new Level4())));
    levelMap_.insert(pair<unsigned int, shared_ptr<Level>>(5, shared_ptr<Level5>(new Level5())));
    //init with level 0
    //can be changed in gamemanager if -startlevel is specified
    level_ = levelMap_[0];
    //Initialize char board
    for(int i = 0; i < rows_; i++)
    {
        vector<char> temp;
        for(int j = 0; j < cols_; j++)
        {
            temp.push_back(' ');
        }
        charBoard_.push_back(temp);
    }
    upcomingBlock_ = level_->nextBlock();
}

//constructor
BoardSubject::BoardSubject() {}

void BoardSubject::initialize() 
{
    board_ = make_shared<PImplBoard>();
}

//Returns true if floatingBlock's current position on the board is valid
//I.E. the block lies within the board's boundaries and it does not collide
//with any existing blocks on the board. 
//Blocks in the reserve rows are considered valid in this function
bool BoardSubject::isBlockPositionLegal()
{
    vector<shared_ptr<coordinate>> blockParts = board_->floatingBlock_->getCoordinates();
    for(int i = 0; i < blockParts.size(); i++)//check all 4 block parts
    {
        //check if the row value is legal
        if(blockParts[i]->row_ > board_->rows_ + 3 || blockParts[i]->row_ <= 0)
        {
            return false;
        }
        //check if the col value is legal
        if(blockParts[i]->col_ > board_->cols_ || blockParts[i]->col_ <= 0)
        {
            return false;
        }
        //check if block part's location is occupied by an existing block
        if(blockParts[i]->row_ > 3 && board_->charBoard_[blockParts[i]->row_-4][blockParts[i]->col_-1] != ' ')
        {
            return false;
        }
    }
    return true;
}

void BoardSubject::moveRight()
{
    board_->floatingBlock_->right();
    if(!isBlockPositionLegal())//undo illegal move
    {
        board_->floatingBlock_->left();
    }
}

void BoardSubject::moveLeft()
{
    board_->floatingBlock_->left();
    if(!isBlockPositionLegal())//undo illegal move
    {
        board_->floatingBlock_->right();
    }
}

void BoardSubject::moveDown()
{
    board_->floatingBlock_->down();
    if(!isBlockPositionLegal())//undo illegal move
    {
        board_->floatingBlock_->up();
    }
}

void BoardSubject::clockwise()
{
    board_->floatingBlock_->rotate(true);
    if(!isBlockPositionLegal())//undo illegal move
    {
        board_->floatingBlock_->rotate(false);
    }
}

void BoardSubject::counterclockwise()
{
    board_->floatingBlock_->rotate(false);
    if(!isBlockPositionLegal())//undo illegal move
    {
        board_->floatingBlock_->rotate(true);
    }
}

void BoardSubject::move(Direction d)
{
    if(d == Direction::left)
    {
        moveLeft();
    }
    else if(d == Direction::right)
    {
        moveRight();
    }
    else
    {
        moveDown();
    }
}

void BoardSubject::postMove() 
{
    board_->level_->postMove(board_->floatingBlock_, instance());
    notifyObservers();
}

void BoardSubject::rotate(Rotation r)
{
    if(r == Rotation::cw)
    {
        clockwise();
    }
    else
    {
        counterclockwise();
    }
}

void BoardSubject::levelAdjustment(LevelAdj adj)
{
    int currentLevel = board_->level_->whatLevel();
    bool wasReadingFromFile = board_->level_->isReadingFromFile();
    string filename = board_->level_->getFileName();

    if(adj == LevelAdj::lvUp)
    {
        currentLevel++;
    }
    else
    {
        currentLevel--;
    }
    if (0 <= currentLevel && currentLevel <= board_->highestLvl_){
        board_->level_ = board_->levelMap_[currentLevel];
		board_->level_->reset();
    } 
    //randomness should be carried to new level
    if (wasReadingFromFile) board_->level_->switchToReadFile(filename);
    else board_->level_->switchToRandom();
    notifyObservers();
}

void BoardSubject::drop(bool isCalledPostDropping)
{
    //move block as far down as possible
    while(isBlockPositionLegal())
    {
        board_->floatingBlock_->down();
    }
    //undo illegal move
    board_->floatingBlock_->up();

    if (isBlockInReservedRow()) 
    {
        //our newly generated block is at reserved rows
        //new block cannot be generated
        //restart and stop drop() function
        printGameEndSummary();
        restart(board_->startlevel_); 
        return;
    }
    board_->boardBlocks_.push_back(board_->floatingBlock_);
    updateCharBoardByBlocks();
    updateScoreRows(); //update score for rows
    removeFullRows(); //Check if row is full -> delete row
    updateScoreBlocks();
    updateHiScore();//update hiscore accordingly
    removeEmptyBlocks();//remove Blocks that are empty (no Block parts)

    if(!isCalledPostDropping)//this condition is necessary, otherwise triggers infinite loop
    {
        board_->level_->postDrop(instance());
    }
    //generate new block
    board_->floatingBlock_ = board_->upcomingBlock_;
    board_->upcomingBlock_ = board_->level_->nextBlock();

    //Check if floatingBlock_ collides -> end game if so
    if(!isCalledPostDropping)
    {
        if(!isBlockPositionLegal()) 
        {
            //our newly generated block is at illegal position
            //game should end now
            printGameEndSummary();
            restart(board_->startlevel_);
        }
        notifyObservers();
    }
}

void BoardSubject::getHint() 
{
    cout << "Here's a few tips!" << endl;
    cout << "(a) Try to place your block as far down as possible" << endl;
    cout << "(b) Try to place your block such that as many of its edges touch othe blocks are borders" << endl;
}

void BoardSubject::setRandom(bool ran, string path)
{
    // random only works for level 3 and 4
    if (board_->level_->whatLevel() == 3 || 
        board_->level_->whatLevel() == 4) {
        if (ran == false) {
            // we read from file
            board_->level_->switchToReadFile(path);
        } else {
            //randomly generate stuff
            board_->level_->switchToRandom();
        }
    }
}

void BoardSubject::restart(unsigned int startLvl)
{
    //empty blocks
    board_->boardBlocks_.clear();    
    //reset score
    board_->score_ = 0;
    //start at startlevel provided by command line arg
    board_->level_ = board_->levelMap_[startLvl];
    board_->level_->reset();
    //new floating block
    updateCharBoardByBlocks();
    board_->floatingBlock_ = nullptr;
    board_->upcomingBlock_ = board_->level_->nextBlock();
    notifyObservers();
    //Allow GUI time to display empty board
    this_thread::sleep_for(chrono::seconds(3));
    board_->floatingBlock_ = board_->upcomingBlock_;
    board_->upcomingBlock_ = board_->level_->nextBlock();
    // updateCharBoardByBlocks();
    notifyObservers();
}
void BoardSubject::specificBlock(string s)
{
    board_->floatingBlock_ = BlockFactorySingleton::getInstance().createBlock(s, board_->level_->whatLevel());
}

unsigned int BoardSubject::getNumRows() const 
{
    return board_->rows_;
}

unsigned int BoardSubject::getNumCols() const 
{
    return board_->cols_;
}

unsigned int BoardSubject::getNumResRows() const 
{
    return board_->resRows_;
}

char BoardSubject::getBoardCharacter(int row, int col)
{
    if(row > board_->rows_ || col > board_->cols_)
    {
        return ' ';
    }
    else
    {
        return board_->charBoard_[row][col];
    }
}

char BoardSubject::getFloatingBlockType()
{
    return board_->floatingBlock_->getType();
}

bool BoardSubject::floatingBlockIsHere(int row, int col)
{
    if (!board_->floatingBlock_) return false;
    vector<shared_ptr<coordinate>> fb = board_->floatingBlock_->getCoordinates();
    for(int i = 0; i < fb.size(); i++)
    {
        if(fb[i]->row_ == row && fb[i]->col_ == col)
        {
            return true;
        }
    }
    return false;
}

vector<shared_ptr<coordinate>> BoardSubject::getUpcomingBlock()
{
    return board_->upcomingBlock_->getCoordinates();
}

char BoardSubject::getUpcomingBlockType()
{
    return board_->upcomingBlock_->getType();
}

void BoardSubject::updateCharBoardByBlocks()
{
    //empty char board
    board_->charBoard_.clear();
    for(int i = 0; i < board_->rows_; i++)
    {
        vector<char> temp;
        for(int j = 0; j < board_->cols_; j++)
        {
            temp.push_back(' ');
        }
        board_->charBoard_.push_back(temp);
    }
    for (auto block:board_->boardBlocks_){
	//for each non-floating block
        for (auto p: block->getCoordinates()){
	        board_->charBoard_[p->row_ - 4][p->col_ - 1] = block->getType();
	    }
    }
}

void BoardSubject::removeFullRows()
{
    vector<int> elimRows = getRowsToEliminate();
    //for each row in elimRows
    for (auto &it : elimRows) {
        //remove this row in all blocks and update charBoard
   	    for (auto block:board_->boardBlocks_){
	        block->deleteRow(it);
	    }
	    updateCharBoardByBlocks();
    }
    if(elimRows.size() > 0)
    {
        board_->level_->resetCounter();//resets counter for level 4
    }
}

vector<int> BoardSubject::getRowsToEliminate() 
{
    vector<int> elimRows;
    //for each row
    for (unsigned int i = 0; i < board_->rows_; ++i){ 
    	//check if it's full
    	bool isfull = true;
	for (auto r:board_->charBoard_[i]){
	    if (r == ' '){
		isfull = false;
		break;
	    }
	}
    	//if row is full
    	if (isfull){
            //add this row to list of full rows (absolute row position)
            elimRows.push_back(i+4);
	    }
    }
    return elimRows;
}

vector<shared_ptr<Block>> BoardSubject::getBlocksToEliminate()
{
    std::vector<shared_ptr<Block>> ret;
    for (auto block: board_->boardBlocks_){
        if (block->getCoordinates().size() == 0) {
            ret.push_back(block);
        };
    }
    return ret;
}

void BoardSubject::removeEmptyBlocks()
{
    //filter empty blocks out by copying block ptrs into new vector via empty check predicate
    vector<shared_ptr<Block>> nonEmptyBlocks;
    for (int it = 0; it < board_->boardBlocks_.size(); it++) {
        if (board_->boardBlocks_[it]->getCoordinates().size() > 0) 
            nonEmptyBlocks.push_back(board_->boardBlocks_[it]);
    }
    board_->boardBlocks_ = nonEmptyBlocks;
}

void BoardSubject::updateScoreRows() 
{
    int points = 0;
    //get score from row eliminations
    vector<int> elimRows = getRowsToEliminate();
    if (elimRows.size() > 0) {
        points += pow((board_->level_->whatLevel() + elimRows.size()), 2);
    }
    //update score with extra point earned
    setScore(getScore() + points);
}

void BoardSubject::updateScoreBlocks() 
{
    int points = 0;
    //get score from block eliminations
    std::vector<shared_ptr<Block>> elimBlocks = getBlocksToEliminate();
    int blockPoints = 0;
    for (auto &block : elimBlocks) {
        blockPoints += pow((block->getLevelGenerated() + 1), 2);
    }
    points += blockPoints;
    //update score with extra point earned
    setScore(getScore() + points);
}

void BoardSubject::updateHiScore() 
{
    //if current score is greater than hiScore, update hiScore
    if (getScore() > getHiScore()) {
        setHiScore(getScore());
    }
}


void BoardSubject::changeFloatingBlockToNextBlock()
{
    board_->floatingBlock_ = board_->upcomingBlock_;
    board_->upcomingBlock_ = board_->level_->nextBlock();
}

void BoardSubject::fixFloatingBlock(shared_ptr<Block> b)
{
    board_->upcomingBlock_ = board_->floatingBlock_;
    board_->floatingBlock_ = b;
}

shared_ptr<Block> BoardSubject::getFloatingBlockInstance()
{
    return board_->floatingBlock_;
}

int BoardSubject::getLevelNum() 
{
    return board_ -> level_ -> whatLevel();
}

int BoardSubject::getScore() 
{
    return board_ -> score_;
}

int BoardSubject::getHiScore() 
{
    return board_ -> hiScore_;
}

void BoardSubject::setHiScore(int newHighScore) 
{
    board_ -> hiScore_ = newHighScore;
}

void BoardSubject::setScore(int points) 
{
    board_ -> score_ = points;
}

void BoardSubject::printGameEndSummary()
{
    cout << "=================================" << endl;
    cout << "Game Over!" << endl;
    cout << "Your score for this game is: " << board_ -> score_ << endl;
    cout << "You higest score is " << board_ -> hiScore_ << endl;
    cout << "=================================" << endl;
    // do not wait for user input, restart directly
    cout << "Restarting the game now" << endl;
}

void BoardSubject::setSartLevel(unsigned int lvl)
{
    board_->startlevel_ = lvl;
}

void BoardSubject::setMaxLevel(unsigned int lvl)
{
    board_->highestLvl_ = lvl;
}

bool BoardSubject::isBlockInReservedRow()
{
    vector<shared_ptr<coordinate>> blockParts = board_->floatingBlock_->getCoordinates();
    for(int i = 0; i < blockParts.size(); i++)//check all 4 block parts
    {
        //check if the row value is legal
        if(blockParts[i]->row_ <= 3)
        {
            return true;
        }
    }
    return false;
}
