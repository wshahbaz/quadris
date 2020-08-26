#ifndef _IBLOCK_H_
#define _IBLOCK_H_
#include "Block.h"
#include "coordinate.h"

class IBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        IBlockFactory();
};

class IBlock: public Block
{
    friend class IBlockFactory;
    public:
       ~IBlock();
        virtual void rotate(bool clockwise) override;
        virtual char getType() const;

    private:
        IBlock(int levelNum);//private ctor
};

#endif
