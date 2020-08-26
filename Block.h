#ifndef _BLOCK_H_
#define _BLOCK_H_
#include <vector>
#include <memory>
#include "coordinate.h"

class Block;
class AbstractBlockFactory;

//The base factory all other the factories will inherit.
class BlockFactory
{
	friend class AbstractBlockFactory;
    public:
        virtual ~BlockFactory();
        virtual std::shared_ptr<Block> createBlock(int levelNum) = 0;
	
    protected:
	    BlockFactory();
};

class Block
{
    friend class BlockFactory;
    public:
        virtual ~Block() {}
        virtual void rotate(bool clockwise) = 0;
        void deleteRow(int row);
        void right();//move right by one
        void left();//move left by one
        void down();//move down by one
        void up();//move up by one
        std::vector<std::shared_ptr<coordinate>> getCoordinates();
        int getLevelGenerated();
        virtual char getType() const = 0;

    protected:
        Block() {};
        int levelGenerated_;
        int rotationState_;
        std::vector<std::shared_ptr<coordinate>> blockParts_;
        void changeStateTwo();//change rotation states for blocks with 2 states
        void changeStateFour(bool forward);//change rotation states for blocks with 4 states
};


#endif
