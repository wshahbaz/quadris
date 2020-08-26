#ifndef _JBLOCK_H_
#define _JBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

class JBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        JBlockFactory();
};

class JBlock: public Block
{
    friend class JBlockFactory;
    public:
        ~JBlock();
        virtual void rotate(bool clockwise) override;
        virtual char getType() const;

    private:
        JBlock(int levelNum);//private ctor
};

#endif
