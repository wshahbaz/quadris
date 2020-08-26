#include <vector>
#include "ZBlock.h"
#include "coordinate.h"

using namespace std;

ZBlock::ZBlock(int levelNum)
{
    // Set the block's position to its initial position as defined in the specification
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3
    // 1
    // 2
    // 3
    // 4 Z Z
    // 5   Z Z

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 2)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 3)));//block part 4, denoted as c4
    rotationState_ = 1;
    levelGenerated_ = levelNum;
}

char ZBlock::getType() const{
    return 'Z';
}

ZBlock::~ZBlock() {}

void ZBlock::rotate(bool clockwise)
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
    // 3             3   Z
    // 4 Z Z         4 Z Z
    // 5   Z Z       5 Z

    // Rotatation states with specific block parts (see constructor):
    // c1 and c2 will always be in the same position
    // State 1    State 2
    //   1  2  3      1  2  3
    // 1            1
    // 2            2 
    // 3            3    c4
    // 4 c1 c2      4 c1 c2
    // 5    c3 c4   5 c3

    if(rotationState_ == 1)//State 1 -> State 2
    {
        blockParts_[2]->shiftCol(-1);
        blockParts_[3]->shiftRow(-2);
        blockParts_[3]->shiftCol(-1);
    }
    else//State 2 -> State 1
    {
        blockParts_[2]->shiftCol(1);
        blockParts_[3]->shiftRow(2);
        blockParts_[3]->shiftCol(1);
    }
    changeStateTwo();
}

ZBlockFactory::ZBlockFactory() {}
shared_ptr<Block> ZBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<ZBlock>(new ZBlock(levelNum));
}
