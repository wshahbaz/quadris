#ifndef _STARBLOCK_H_
#define _STARBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class StarBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        StarBlockFactory();
};

class StarBlock: public Block
{
    friend class StarBlockFactory;
    public:
        ~StarBlock();
        virtual char getType() const;
        virtual void rotate(bool clockwise) override;

    private:
        StarBlock(int levelNum);//private ctor
};

#endif
