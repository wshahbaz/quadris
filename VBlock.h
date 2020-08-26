#ifndef _VBLOCK_H_
#define _VBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

//Bonus block used in Level 6
class VBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        VBlockFactory();
};

class VBlock: public Block
{
    friend class VBlockFactory;
    public:
        ~VBlock();
        virtual void rotate(bool clockwise) override;
        virtual char getType() const;

    private:
        VBlock(int levelNum);//private ctor
};

#endif
