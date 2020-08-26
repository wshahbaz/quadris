#ifndef _LBLOCK_H_
#define _LBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class LBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        LBlockFactory();
};

class LBlock: public Block
{
    friend class LBlockFactory;

    public:
        ~LBlock();
        virtual char getType() const;
        virtual void rotate(bool clockwise) override;

    private:
        LBlock(int levelNum);//private ctor
};

#endif
