#ifndef _LEVEL_
#define _LEVEL_

#include "FileIO.h"
#include "Block.h"
#include "AbstractBlockFactory.h"
#include <string>
#include <vector>
#include <memory>
#include <map> 

class BoardSubject;//for postMove

extern std::string defaultFilePath;
//use strategy pattern to get next block
//could use some sort of member variable and if statement to achieve the same thing
//but the coupling is huge

//each Level instancce will hold a reference to GenBlock (or a child of it)
//each level will call getNextBlockType() to determine what is the next block it is going to generate
//then level takes the result and use it in nextBlock() to generate a shared_ptr<Block>
class GenBlock {
    public:
    virtual std::string getNextBlockType() = 0;
    virtual bool isReadingFromFile() const = 0;
    virtual std::string getFileName();
};

struct PGenBlockFromFile {
    //sequence of block to be generated
    std::vector<std::string> blockSeq_;
    //current block
    std::vector<std::string>::iterator curr_;
    std::string filename_;
};

class GenBlockFromFile: public GenBlock {
    private:
    std::shared_ptr<PGenBlockFromFile> gb_;
    public:
    GenBlockFromFile(std::string path = defaultFilePath);
    std::string getFileName() override;
    std::string getNextBlockType() override;
    bool isReadingFromFile() const override;
};

struct PGenRandomBlock {
    // <char of block type, probability of it being generated>
    std::map<char, double> blockProbabilities_;
};

class GenRandomBlock: public GenBlock {
    protected:
    std::shared_ptr<PGenRandomBlock> gb_;
    public:
    GenRandomBlock();
    std::string getNextBlockType() override;
    bool isReadingFromFile() const override;
};

class GenRandomBlockLvl1: public GenRandomBlock {
    public:
    GenRandomBlockLvl1();
};

class GenRandomBlockLvl2: public GenRandomBlock {
    public:
    GenRandomBlockLvl2();
};

class GenRandomBlockLvl3: public GenRandomBlock {
    public:
    GenRandomBlockLvl3();
};

class GenRandomBlockLvl5: public GenRandomBlock {
    public:
    GenRandomBlockLvl5();
};

struct PImplLevel{
    AbstractBlockFactory& blockFactory_;
    std::shared_ptr<GenBlock> blockGen_;
    PImplLevel();
};


class Level{
    protected:
    std::shared_ptr<PImplLevel> lv_;

    public:
    virtual void postMove(std::shared_ptr<Block> block, std::shared_ptr<BoardSubject> bd); //for "heavy" blocks, empty for level < 3
    virtual void postDrop(std::shared_ptr<BoardSubject> bd); //for * blocks generated after drop, empty for level < 4
    virtual void resetCounter(); //counter of drop that doesn't eliminate rows, empty for level < 4
    virtual std::shared_ptr<Block> nextBlock();
    virtual unsigned int whatLevel() const = 0;
    virtual void switchToReadFile(std::string path = defaultFilePath);
    virtual void switchToRandom();
    virtual bool isReadingFromFile();
    virtual std::string getFileName();
    virtual void reset();
    // virtual Block generateBlock(string b) = 0;
};

class Level0: public Level {
    public:
    Level0();
    unsigned int whatLevel() const override;
    void reset() override;
};


class Level1: public Level {
    public:
    Level1();
    unsigned int whatLevel() const override;
};

class Level2: public Level {
    public:
    Level2();
    unsigned int whatLevel() const override;
};

class Level3: public Level {
    public:
    Level3();
    void switchToReadFile(std::string path = defaultFilePath) override;
    void switchToRandom() override;
    unsigned int whatLevel() const override;
    void postMove(std::shared_ptr<Block> block, std::shared_ptr<BoardSubject> bd) override;
};

class Level4: public Level3 {
    private:
    //how many times you placed a block without eliminating a row
    unsigned int dropCounter_;
    public:
    Level4();
    unsigned int whatLevel() const override;
    void postDrop(std::shared_ptr<BoardSubject> bd); //empty for level < 4
    void resetCounter();
    void reset() override;
};

class Level5: public Level {
    public:
    Level5();
    unsigned int whatLevel() const override;
};

//global function to change default file path (external variablle)
void changeDefaultFilePath(std::string);

#endif
