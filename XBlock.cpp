#include <vector>
#include "XBlock.h"
#include "coordinate.h"

using namespace std;

XBlock::XBlock(int levelNum)
{
    // Set the block's position to its initial position as shown below
    // Vertical numbers: row number, Horizontal numbers: column number
    //   1 2 3
    // 1
    // 2
    // 3
    // 4 X   X
    // 5   X
    // 6 X   X

    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));//block part 1, denoted as c1
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 3)));//block part 2, denoted as c2
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));//block part 3, denoted as c3
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(6, 1)));//block part 4, denoted as c4
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(6, 3)));//block part 5, denoted as c5
    levelGenerated_ = levelNum;
}

char XBlock::getType() const{
    return 'X';
}

XBlock::~XBlock() {}

//This block's rotational symmetry makes the rotate function trivial
void XBlock::rotate(bool clockwise) {}

XBlockFactory::XBlockFactory() {}
shared_ptr<Block> XBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<XBlock>(new XBlock(levelNum));
}
