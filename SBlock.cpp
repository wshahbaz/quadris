#include <vector>
#include "SBlock.h"
#include "coordinate.h"

using namespace std;

SBlock::SBlock(int levelNum)
{
    // Set the block's position to its initial position as defined in the specification
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3
    // 1
    // 2
    // 3
    // 4   S S
    // 5 S S

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 2)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 3)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 1)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));//block part 4, denoted as c4
    rotationState_ = 1;
    levelGenerated_ = levelNum;
}

char SBlock::getType() const{
    return 'S';
}

SBlock::~SBlock() {}

void SBlock::rotate(bool clockwise)
{
    // The two rotation states:
    // Calling rotate will move the block from its current state to the other state
    // I.E. State 1 -> State 2 or State 2 -> State 1
    // The clockwise value, in this case, is irrelevant
    // Vertical numbers: row number, Horizontal numbers: column number
    // The block's default position is used for this illustration
    // State 1       State 2
    //   1 2 3         1 2 3
    // 1             1
    // 2             2
    // 3             3 S
    // 4   S S       4 S S
    // 5 S S         5   S

    // Rotatation states with specific block parts (see constructor):
    // c1 and c4 will always be in the same position
    // State 1    State 2
    //   1  2  3      1  2  3
    // 1            1
    // 2            2 
    // 3            3 c2
    // 4    c1 c2   4 c3 c1
    // 5 c3 c4      5    c4

    if(rotationState_ == 1)//State 1 -> State 2
    {
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(-2);
        blockParts_[2]->shiftRow(-1);
    }
    else//State 2 -> State 1
    {
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(2);
        blockParts_[2]->shiftRow(1);
    }
    changeStateTwo();
}

SBlockFactory::SBlockFactory() {}
shared_ptr<Block> SBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<SBlock>(new SBlock(levelNum));
}

