#include <vector>
#include "TBlock.h"
#include "coordinate.h"

using namespace std;

TBlock::TBlock(int levelNum)
{
    // Set the block's position to its initial position as defined in the specification
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3
    // 1
    // 2
    // 3
    // 4 T T T
    // 5   T

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 2)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 3)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));//block part 4, denoted as c4
    rotationState_ = 1;
    levelGenerated_ = levelNum;
}

char TBlock::getType() const{
    return 'T';
}

TBlock::~TBlock() {}

void TBlock::rotate(bool clockwise)
{
    // The four rotation states:
    // Calling rotate will move the block from its current state an adjacent state
    // If clockwise is true, the state numbers will increase
    // I.E. Clockwise rotations: State 1 -> State 2 -> State 3 -> State 4 -> State 1
    // Counterclockwise rotations: State 1 -> State 4 -> State 3 -> State 2 -> State 1
    // Vertical numbers: row number, Horizontal numbers: column number
    // The block's default position is used for this illustration
    // State 1     State 2    State 3     State 4
    //   1 2 3       1 2 3      1 2 3       1 2 3
    // 3           3   T      3           3 T
    // 4 T T T     4 T T      4   T       4 T T
    // 5   T       5   T      5 T T T     5 T

    // Rotatation states with specific block parts (see constructor):
    // State 1      State 2      State 3      State 4
    //   1  2  3      1  2  3      1  2  3      1  2  3
    // 3            3    c1      3            3 c3
    // 4 c1 c2 c3   4 c4 c2      4    c4      4 c2 c4
    // 5    c4      5    c3      5 c3 c2 c1   5 c1

    if(rotationState_ == 1 && clockwise)//state 1 -> state 2
    {
        blockParts_[0]->shiftRow(-1);
        blockParts_[0]->shiftCol(1);
        blockParts_[2]->shiftRow(1);
        blockParts_[2]->shiftCol(-1);
        blockParts_[3]->shiftRow(-1);
        blockParts_[3]->shiftCol(-1);
    }
    else if(rotationState_ == 2 && clockwise)//state 2 -> state 3
    {
        blockParts_[0]->shiftRow(2);
        blockParts_[0]->shiftCol(1);
        blockParts_[1]->shiftRow(1);
        blockParts_[2]->shiftCol(-1);
        blockParts_[3]->shiftCol(1);
    }
    else if(rotationState_ == 3 && clockwise)//state 3 -> state 4
    {
        blockParts_[0]->shiftCol(-2);
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[2]->shiftRow(-2);
    }
    else if(rotationState_ == 4 && clockwise)//state 4 -> state 1
    {
        blockParts_[0]->shiftRow(-1);
        blockParts_[1]->shiftCol(1);
        blockParts_[2]->shiftRow(1);
        blockParts_[2]->shiftCol(2);
        blockParts_[3]->shiftRow(1);
    }
    else if(rotationState_ == 1 && !clockwise)//state 1 -> state 4
    {
        blockParts_[0]->shiftRow(1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[2]->shiftRow(-1);
        blockParts_[2]->shiftCol(-2);
        blockParts_[3]->shiftRow(-1);
    }
    else if(rotationState_ == 2 && !clockwise)//state 2 -> state 1
    {
        blockParts_[0]->shiftRow(1);
        blockParts_[0]->shiftCol(-1);
        blockParts_[2]->shiftRow(-1);
        blockParts_[2]->shiftCol(1);
        blockParts_[3]->shiftRow(1);
        blockParts_[3]->shiftCol(1);
    }
    else if(rotationState_ == 3 && !clockwise)//state 3 -> state 2
    {
        blockParts_[0]->shiftRow(-2);
        blockParts_[0]->shiftCol(-1);
        blockParts_[1]->shiftRow(-1);
        blockParts_[2]->shiftCol(1);
        blockParts_[3]->shiftCol(-1);
    }
    else//state 4 -> state 3
    {
        blockParts_[0]->shiftCol(2);
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(1);
        blockParts_[2]->shiftRow(2);
    }
    changeStateFour(clockwise);
}

TBlockFactory::TBlockFactory() {}
shared_ptr<Block> TBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<TBlock>(new TBlock(levelNum));
}
