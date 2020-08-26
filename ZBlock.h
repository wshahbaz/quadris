#ifndef _ZBLOCK_H_
#define _ZBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class ZBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        ZBlockFactory();
};

class ZBlock: public Block
{
    friend class ZBlockFactory;
    public:
        ~ZBlock();
        virtual char getType() const;
        virtual void rotate(bool clockwise) override;

    private:
        ZBlock(int levelNum);//private ctor
};

#endif
