#ifndef _GAME_MANAGER_
#define _GAME_MANAGER_

#include <memory>
#include "BoardSubject.h"
#include "CommandEngine.h"
#include "TextDisplay.h"
#include "GUIDisplay.h"

//resolve circular dependencies
class CommandEngine;

struct PImplGameManager{
    //GameManager vars
    std::shared_ptr<BoardSubject> board_; 
    std::shared_ptr<CommandEngine> command_; 
    unsigned int seed_;
    unsigned int startLvl_;
    unsigned int maxLvl_;

    bool guiEnabled;
    PImplGameManager();
};


class GameManager{
    private:
    	GameManager();
        //singleton glocal obj
        static std::shared_ptr<GameManager> gm_;                   
        std::shared_ptr<PImplGameManager> gameManager_;
        std::shared_ptr<TextDisplay> text_;
        std::shared_ptr<GUIDisplay> gui_;

        //Special Member Functions
        GameManager(const GameManager&) = delete;                 //prevent copying the class         
        GameManager& operator= (const GameManager&) = delete;     //prevent copying the class

    public:
        static std::shared_ptr<GameManager> instance();    //get global GameManager instance
        void startGame();           //client calls this to start a Game
        void initialize(int, char**);          //setup any data that can't be in ctor
        void move(Direction);
        void rotate(Rotation);
        void levelAdjustment(LevelAdj);
        void drop();
        void getHint();
        void setRandom(bool ran, std::string path = "");
        void restart();
        void specificBlock(std::string s);
        void changeFloatingBlockToNextBlock();
        void setSeed(unsigned int sd);
        void setStartLvl(unsigned int sl);
        void disableGUI();
        void setDefaultFilePath(std::string);
        void executePostMove();
        void setMaxLvl(unsigned int lvl);
};

#endif
