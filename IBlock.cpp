#include <vector>
#include "IBlock.h"
#include "coordinate.h"

using namespace std;


IBlock::IBlock(int levelNum)
{
    // Set the block's position to its initial position as defined in the specification
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3 4
    // 1
    // 2
    // 3
    // 4 I I I I
    
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 2)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 3)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 4)));//block part 4, denoted as c4
    rotationState_ = 1;
    levelGenerated_ = levelNum;
}

char IBlock::getType() const{
    return 'I';
}
IBlock::~IBlock() {}

void IBlock::rotate(bool clockwise)
{
    // The two rotation states:
    // Calling rotate will move the block from its current state to the other state
    // I.E. State 1 -> State 2 or State 2 -> State 1
    // The clockwise value, in this case, is irrelevant
    // Vertical numbers: row number, Horizontal numbers: column number
    // The block's default position is used for this illustration
    // State 1       State 2
    //   1 2 3 4       1 2 3 4
    // 1             1 I
    // 2             2 I
    // 3             3 I
    // 4 I I I I     4 I

    // Rotatation states with specific block parts (see constructor):
    // State 1          State 2
    //   1  2  3  4       1  2  3  4
    // 1                1 c4
    // 2                2 c3
    // 3                3 c2
    // 4 c1 c2 c3 c4    4 c1

    if(rotationState_ == 1)//State 1 -> State 2
    {
        //c1's position doesn't change
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[2]->shiftRow(-2);
        blockParts_[2]->shiftCol(-2);
        blockParts_[3]->shiftRow(-3);
        blockParts_[3]->shiftCol(-3);
    }
    else//State 2 -> State 1
    {
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(1);
        blockParts_[2]->shiftRow(2);
        blockParts_[2]->shiftCol(2);
        blockParts_[3]->shiftRow(3);
        blockParts_[3]->shiftCol(3);
    }
    changeStateTwo();
}

IBlockFactory::IBlockFactory() {}
shared_ptr<Block> IBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<IBlock>(new IBlock(levelNum));
}

