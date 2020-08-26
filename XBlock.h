#ifndef _XBLOCK_H_
#define _XBLOCK_H_
#include <memory>
#include "Block.h"
#include "coordinate.h"

//Bonus block used in Level 6
class XBlockFactory : public BlockFactory
{
	friend class AbstractBlockFactory;
    public:
	    virtual std::shared_ptr<Block> createBlock(int levelNum);
    protected:
        XBlockFactory();
};

class XBlock: public Block
{
    friend class XBlockFactory;
    public:
        ~XBlock();
        virtual void rotate(bool clockwise) override;
        virtual char getType() const;

    private:
        XBlock(int levelNum);//private ctor
};

#endif
