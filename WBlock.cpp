#include <vector>
#include "WBlock.h"
#include "coordinate.h"

using namespace std;

WBlock::WBlock(int levelNum)
{
    // Set the block's position to its initial position as defined below
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3
    // 1
    // 2
    // 3
    // 4 W   
    // 5 W W
    // 6   W W

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 1)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(6, 2)));//block part 4, denoted as c4
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(6, 3)));//block part 5, denoted as c5
    rotationState_ = 1;
    levelGenerated_ = levelNum;
}

char WBlock::getType() const{
    return 'W';
}

WBlock::~WBlock() {}

void WBlock::rotate(bool clockwise)
{
    // The four rotation states:
    // Calling rotate will move the block from its current state an adjacent state
    // If clockwise is true, the state numbers will increase
    // I.E. Clockwise rotations: State 1 -> State 2 -> State 3 -> State 4 -> State 1
    // Counterclockwise rotations: State 1 -> State 4 -> State 3 -> State 2 -> State 1
    // Vertical numbers: row number, Horizontal numbers: column number
    // The block's default position is used for this illustration
    // State 1     State 2    State 3       State 4
    //   1 2 3       1 2 3      1 2 3         1 2 3
    // 3           3          3             3   
    // 4 W         4   W W    4 W W         4     W
    // 5 W W       5 W W      5   W W       5   W W
    // 6   W W     6 W        6     W       6 W W

    // Rotatation states with specific block parts (see constructor):
    // c3 will always be in the same position
    // State 1      State 2      State 3        State 4
    //   1  2  3      1  2  3      1  2  3        1  2  3
    // 3            3            3              3   
    // 4 c1         4    c2 c1   4 c5 c4        4       c5
    // 5 c2 c3      5 c4 c3      5    c3 c2     5    c3 c4
    // 6    c4 c5   6 c5         6       c1     6 c1 c2
 
    if(rotationState_ == 1 && clockwise)//state 1 -> state 2
    {
        blockParts_[0]->shiftCol(2);
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(1);
        blockParts_[3]->shiftRow(-1);
        blockParts_[3]->shiftCol(-1);
        blockParts_[4]->shiftCol(-2);
    }
    else if(rotationState_ == 2 && clockwise)//state 2 -> state 3
    {
        blockParts_[0]->shiftRow(2);
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(1);
        blockParts_[3]->shiftRow(-1);
        blockParts_[3]->shiftCol(1);
        blockParts_[4]->shiftRow(-2);
    }
    else if(rotationState_ == 3 && clockwise)//state 3 -> state 4
    {
        blockParts_[0]->shiftCol(-2);
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[3]->shiftRow(1);
        blockParts_[3]->shiftCol(1);
        blockParts_[4]->shiftCol(2);
    }
    else if(rotationState_ == 4 && clockwise)//state 4 -> state 1
    {
        blockParts_[0]->shiftRow(-2);
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[3]->shiftRow(1);
        blockParts_[3]->shiftCol(-1);
        blockParts_[4]->shiftRow(2);
    }
    else if(rotationState_ == 1 && !clockwise)//state 1 -> state 4
    {
        blockParts_[0]->shiftRow(2);
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(1);
        blockParts_[3]->shiftRow(-1);
        blockParts_[3]->shiftCol(1);
        blockParts_[4]->shiftRow(-2);
    }
    else if(rotationState_ == 2 && !clockwise)//state 2 -> state 1
    {
        blockParts_[0]->shiftCol(-2);
        blockParts_[1]->shiftRow(1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[3]->shiftRow(1);
        blockParts_[3]->shiftCol(1);
        blockParts_[4]->shiftCol(2);
    }
    else if(rotationState_ == 3 && !clockwise)//state 3 -> state 2
    {
        blockParts_[0]->shiftRow(-2);
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(-1);
        blockParts_[3]->shiftRow(1);
        blockParts_[3]->shiftCol(-1);
        blockParts_[4]->shiftRow(2);
    }
    else//state 4 -> state 3
    {
        blockParts_[0]->shiftCol(2);
        blockParts_[1]->shiftRow(-1);
        blockParts_[1]->shiftCol(1);
        blockParts_[3]->shiftRow(-1);
        blockParts_[3]->shiftCol(-1);
        blockParts_[4]->shiftCol(-2);
    }
    changeStateFour(clockwise);
}

WBlockFactory::WBlockFactory() {}
shared_ptr<Block> WBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<WBlock>(new WBlock(levelNum));
}
