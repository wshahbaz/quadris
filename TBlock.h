#ifndef _TBLOCK_H_
#define _TBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class TBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        TBlockFactory();
};

class TBlock: public Block
{
    friend class TBlockFactory;
    public:
        ~TBlock();
        virtual char getType() const;
        virtual void rotate(bool clockwise) override;

    private:
        TBlock(int levelNum);//private ctor
};

#endif
