#include <vector>
#include "StarBlock.h"
#include "coordinate.h"

using namespace std;

StarBlock::StarBlock(int levelNum)
{
    // Set the block's position to its initial position as defined in the specification
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3 4 5 6 7 8 9 10 11
    // 1
    // 2
    // 3
    // 4           *  

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 6)));//block part 1, denoted as c1
    levelGenerated_ = levelNum;
}

char StarBlock::getType() const{
    return '*';
}

StarBlock::~StarBlock() {}

//StarBlock will never need to rotate
void StarBlock::rotate(bool clockwise) {}

StarBlockFactory::StarBlockFactory() {}
shared_ptr<Block> StarBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<StarBlock>(new StarBlock(levelNum));
}
