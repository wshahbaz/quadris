#ifndef _COMMAND_ENGINE_
#define _COMMAND_ENGINE_

#include <iostream>
#include <memory>
#include "GameManager.h"
#include "InterpretationEngine.h"

/******************* COMMAND ENGINE *******************/

//resolve circular dependencies
class GameManager;

struct PCmdEng {
    std::shared_ptr<GameManager> gameManager_; 
    std::shared_ptr<InterpretationEngine> intEngine_;
};

class CommandEngine {
    private:
        CommandEngine();
        static std::shared_ptr<CommandEngine> cmdEngine_;    //singleton global obj
        std::shared_ptr<PCmdEng> PCmdEng_;                    //private implement

        //SMFs
        CommandEngine(const CommandEngine&) = delete;
        CommandEngine& operator= (const CommandEngine&) = delete;     

        //CORE FUNCTIONS
        //any primitive operations (grouped in helper functions) are hosted here and 
        //called on determination of command type as given by commands_ member variable
        void executeCmd(std::deque<std::string>);   //parent method to determine which function to execute based on cmd
        void executeCLA(std::deque<std::string>);  //parent method to determine which function to execute from CLAs  
        //command family functions
        void moveBlock(std::string);
        void dropBlock();
        void rotateBlock(std::string);
        void levelAdjustement(std::string);
        void setRandom(bool, std::string = "");
        void executeSequence(std::string);
        void blockOverride(std::string);
        void restart();
        void generateHint();
        void renameCommand(std::string, std::string);
        //CLA functions
        void setTextOnly();
        void setSeed(std::string);
        void setScriptfile(std::string);
        void setStartLevel(std::string);
        void disableBonus();
        //POST command functions
        void initiatePostMove();
        void terminateProgram();

    public:
        static std::shared_ptr<CommandEngine> instance();       //get global Command Engine instance
        //OPERATOR OVERLOADS
        //not quite sure how to use input overload 
        void processCommands(std::istream&);
        void processCLAs(int, char**);
};

#endif