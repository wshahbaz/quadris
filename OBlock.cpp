#include <vector>
#include "OBlock.h"
#include "coordinate.h"

using namespace std;

OBlock::OBlock(int levelNum)
{
    // Set the block's position to its initial position as defined in the specification
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3
    // 1
    // 2
    // 3
    // 4 O O
    // 5 O O

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 2)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 1)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));//block part 4, denoted as c4
    levelGenerated_ = levelNum;
}

char OBlock::getType() const{
    return 'O';
}

OBlock::~OBlock() {}

//OBlock's rotation will not change its shape (squares are rotationally symmetric)
void OBlock::rotate(bool clockwise) {}

OBlockFactory::OBlockFactory() {}
shared_ptr<Block> OBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<OBlock>(new OBlock(levelNum));
}
