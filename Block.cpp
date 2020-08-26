#include<vector>
#include "coordinate.h"
#include "Block.h"
using namespace std;


void Block::right()
{
    //shift every element to the right by one
    for(int i = 0; i < blockParts_.size(); i++)
    {
        blockParts_[i]->shiftCol(1);
    }
}

void Block::left()
{
    //shift every element to the left by one
    for(int i = 0; i < blockParts_.size(); i++)
    {
        blockParts_[i]->shiftCol(-1);
    }
}

void Block::down()
{
    //shift every element down by one
    for(int i = 0; i < blockParts_.size(); i++)
    {
        blockParts_[i]->shiftRow(1);
    }
}

void Block::up()
{
    //shift every element up by one
    for(int i = 0; i < blockParts_.size(); i++)
    {
        blockParts_[i]->shiftRow(-1);
    }
}

vector<shared_ptr<coordinate>> Block::getCoordinates()
{
    return blockParts_;
}

int Block::getLevelGenerated()
{
    return levelGenerated_;
}

//Delete coordinate if coordinate's row equals the inputted row
//If the coordinate's row is above the inputted row, move the row down by one
void Block::deleteRow(int row)
{
    for(int i = 0; i < blockParts_.size(); i++)//Delete block parts inside the row
    {
        if(blockParts_[i]->row_ == row)
        {
            blockParts_.erase(blockParts_.begin() + i);
            i--;
        }
    }
    for(int i = 0; i < blockParts_.size(); i++)//Increment rows if necessary
    {
        if(blockParts_[i]->row_ < row)
        {
            blockParts_[i]->shiftRow(1);
        }
    }
}

void Block::changeStateTwo()
{
    rotationState_++;
    if(rotationState_ > 2)
    {
        rotationState_ = 1;
    }
}

void Block::changeStateFour(bool forward)
{
    if(forward)
    {
        rotationState_++;
        if(rotationState_ > 4)
        {
            rotationState_ = 1;
        }
    }
    else
    {
        rotationState_--;
        if(rotationState_ < 1)
        {
            rotationState_ = 4;
        }
    }
}

BlockFactory::BlockFactory() {}
BlockFactory::~BlockFactory() {}
