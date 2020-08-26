#include <vector>
#include "HintBlock.h"
#include "coordinate.h"

using namespace std;

HintBlock::HintBlock()
{
    //default position
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 1)));
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(4, 2)));
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 1)));
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 2)));
    blockParts_.push_back(shared_ptr<coordinate>(new coordinate(5, 3)));//extra part for bonus blocks
}

char HintBlock::getType() const
{
    return 'H';
}

HintBlock::~HintBlock() {}

//HintBlock will never need to rotate
void HintBlock::rotate(bool clockwise) {}

//Set block coordinates to the optimal placement that has been found
void HintBlock::setCoordinates(vector<shared_ptr<coordinate>>& coords)
{
    for(int i = 0; i < blockParts_.size() && i < coords.size(); i++)
    {
        blockParts_[i]->row_ = coords[i]->row_;
        blockParts_[i]->col_ = coords[i]->col_;
    }
}

HintBlockFactory::HintBlockFactory() {}
shared_ptr<Block> HintBlockFactory::createBlock(int levelNum)
{
	return shared_ptr<HintBlock>(new HintBlock());
}
