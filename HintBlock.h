#ifndef _HINTBLOCK_H_
#define _HINTBLOCK_H_
#include <vector>
#include <memory>
#include "Block.h"
#include "coordinate.h"

class HintBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        HintBlockFactory();
};

class HintBlock: public Block
{
    friend class HintBlockFactory;
    public:
        ~HintBlock();
        virtual void rotate(bool clockwise) override;
        void setCoordinates(std::vector<std::shared_ptr<coordinate>>& coords);
        virtual char getType() const;

    private:
        HintBlock();//private ctor
};

#endif
