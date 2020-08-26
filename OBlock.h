#ifndef _OBLOCK_H_
#define _OBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class OBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        OBlockFactory();
};

class OBlock: public Block
{
    friend class OBlockFactory;
    public:
        ~OBlock();
        virtual char getType() const;
        virtual void rotate(bool clockwise) override;

    private:
        OBlock(int levelNum);//private ctor
};

#endif
