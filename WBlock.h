#ifndef _WBLOCK_H_
#define _WBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

//Bonus block used in Level 6
class WBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        WBlockFactory();
};

class WBlock: public Block
{
    friend class WBlockFactory;
    public:
        ~WBlock();
        virtual void rotate(bool clockwise) override;
        virtual char getType() const;

    private:
        WBlock(int levelNum);//private ctor
};

#endif
