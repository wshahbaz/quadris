#ifndef _SBLOCK_H_
#define _SBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class SBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        SBlockFactory();
};

class SBlock: public Block
{
    friend class SBlockFactory;
    public:
        ~SBlock();
        virtual char getType() const;
        virtual void rotate(bool clockwise) override;

    private:
        SBlock(int levelNum);//private ctor
};

#endif
