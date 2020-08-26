#include <iostream>
#include <assert.h>
#include <stdlib.h>     /* srand, rand, atoi */

#include "Command.h"
#include "CommandEngine.h"
#include "FileIO.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
////////////////////// COMMAND ENGINE CLASS START //////////////////////
////////////////////////////////////////////////////////////////////////

//initialize singleton cmdEngine_ to nullptr
shared_ptr<CommandEngine> CommandEngine::cmdEngine_ = nullptr;

//constructor
CommandEngine::CommandEngine() {
    PCmdEng_ = shared_ptr<PCmdEng> (new PCmdEng);
    //get instance of Game Manager
    PCmdEng_->gameManager_ = GameManager::instance();
    //get instance of Interpretation Engine
    PCmdEng_ -> intEngine_ = InterpretationEngine::instance();
}

//get CommandEngine instance 
shared_ptr<CommandEngine> CommandEngine::instance(){
    if (cmdEngine_ == nullptr) {
        cmdEngine_ = shared_ptr<CommandEngine>(new CommandEngine());
    }
    return cmdEngine_;
}

//input operator for commands
void CommandEngine::processCommands(istream& sin) {
    //parse input into sequential list of primitive args and params
    deque<string> result = PCmdEng_->intEngine_ -> interpretCommands(sin);
    //execute the resulting command, taking in parameters as needed
    executeCmd(result);
}

//input/processing method for CLAs
void CommandEngine::processCLAs(int argc, char** argv) {
    deque<string> result = PCmdEng_ -> intEngine_ -> interpretCLAs(argc, argv);
    executeCLA(result);
}

//CORE FUNCTIONS
//command execution 
//routes commands, args processed by interpretation engine into member function calls
void CommandEngine::executeCmd(deque<string> commands) {
    //iterate through deque, taking commands as params when required
    if (commands.empty()) return;
    string cmd = commands.front();
    commands.pop_front();
    //determine which case of function it is and execute corresponding family function
    if (cmd == "left" || cmd == "right" || cmd == "down") {
        //directional block move
        moveBlock(cmd);
    } 
    else if (cmd == "drop") {
        //block drop
        dropBlock();
    } 
    else if (cmd == "clockwise" || cmd == "counterclockwise") {
        //rotation on block
        rotateBlock(cmd);
    } 
    else if (cmd == "levelup" || cmd == "leveldown") {
        //level change
        levelAdjustement(cmd);
    } 
    else if (cmd == "random" || cmd == "norandom") {
        //randomness adjustment for lvl 3,4
        if (cmd == "random") {
            setRandom(true);
        } 
        else {
            //non-random mode. open file strea
            string fileName = commands.front();
            commands.pop_front();
            setRandom(false, fileName);
        }
    } 
    else if (cmd == "sequence") {
        //execute a sequence of commands from file (name next param)
        string fileName = commands.front();
        commands.pop_front();
        executeSequence(fileName);
    }
    else if (cmd == "I" || cmd == "J" || cmd == "L" 
        || cmd == "S" || cmd == "Z" || cmd == "O" || cmd == "T") {
        //change the current floating block to desired shape
        blockOverride(cmd +"Block");
    }
    else if (cmd == "restart") {
        //reset game state
        restart();
    } 
    else if (cmd == "hint") {
        generateHint();
    } 
    else if (cmd == "rename") {
        //get 2 arguments for origianl + new name
        string cmd = commands.front();
        commands.pop_front();
        string newCmd = commands.front();
        commands.pop_front();
        renameCommand(cmd, newCmd);
    }
    //post move directives (not valid commands on their own)
    else if (cmd == "POSTMOVE") {
        initiatePostMove();
    }
    else if (cmd == "POSTSEQUENCE") {
        cout << "Completed reading sequence file(s). Exiting now" << endl;
        //terminate the program
        terminateProgram();
    }
    else {
        //in case an unrecognized command makes it this far, it's undefined behaviour so terminate
        cout << "INVALID COMMAND UNCAUGHT. ABORTING. " << cmd << endl;
        exit(1);
    }
    //if commands still remain in the sequence, continue executing, otherwise finish
    if (!commands.empty()) executeCmd(commands);
}

//CLA execution
void CommandEngine::executeCLA(deque<string> clas) {
    //for now, iterate through deque, taking additional elements as params when required
    if (clas.empty()) return;
    string cla = clas.front();
    clas.pop_front();
    //determine which function to execute based on cla
    if (cla == "-text") {
        //set text only gui
        setTextOnly();
    } 
    else if (cla == "-seed") {
        //set custom random seed
        string seed = clas.front();
        clas.pop_front();
        setSeed(seed);
    } 
    else if (cla == "-scriptfile") {
        //set custom name to replace `sequence.txt`
        string filename = clas.front();
        clas.pop_front();
        setScriptfile(filename);
    }
    else if (cla == "-startlevel") {
       //set start level to desired level
       string lvl = clas.front();
       clas.pop_front();
       setStartLevel(lvl);
    }
    else if (cla == "-nobonus") {
       disableBonus();
    }
    else {
         //should never happen since int. engine checks, but just in case
         cout << "INVALID COMMAND LINE ARGUMENT ENTERED. ABORTING" << endl;
         exit(1);
    }
    if (!clas.empty()) executeCLA(clas);
}

//primitive command routers
//directional move on block
void CommandEngine::moveBlock(string dir) {
    Direction direction;
    if (dir == "left") direction = Direction::left;
    else if (dir == "right") direction = Direction::right;
    else direction = Direction::down;

    PCmdEng_ -> gameManager_ -> move(direction);
}

//drop block
void CommandEngine::dropBlock() {
    PCmdEng_ -> gameManager_ -> drop();
}

//rotate block
void CommandEngine::rotateBlock(string rot) {
    Rotation rotation = (rot == "clockwise")? Rotation::cw : Rotation::ccw;
    return PCmdEng_ -> gameManager_ -> rotate(rotation);
}

//increase or decrease level
void CommandEngine::levelAdjustement(string levelDir) {
    LevelAdj adjustment = (levelDir == "levelup")? LevelAdj::lvUp : LevelAdj::lvDown;
    PCmdEng_ -> gameManager_ -> levelAdjustment(adjustment);
}

//set randomness on for levels 3,4
void CommandEngine::setRandom(bool toRandom, string filePath) {
    PCmdEng_ -> gameManager_->setRandom(toRandom, filePath);
}

//execute sequence of commands from `filepath`
void CommandEngine::executeSequence(string filePath) {
    //execute commands in filepath until reaching end
    FileIO fileStream(filePath);
    while (!fileStream.isFinished()) {
        processCommands(fileStream.getStream());
    } 
}

//override current block with chosen block
void CommandEngine::blockOverride(string block) {
    PCmdEng_ -> gameManager_ -> specificBlock(block);
}

//reset game
void CommandEngine::restart() {
    PCmdEng_ -> gameManager_ -> restart();
}

//generate hint for next placement
void CommandEngine::generateHint() {
    PCmdEng_ -> gameManager_ -> getHint();
}

void CommandEngine::renameCommand(string cmd, string newCmd) {
    //create an alias in alias map by {alias, realCommand}
    //remove existing command in commands_ to "delete" previous offerring of command
    //make sure to update alias map accordingly as well
    //STEP ONE: check cmd is a previously recognized command, and newCmd is unique
    if (!PCmdEng_->intEngine_->assertRecognizedCommand(cmd)) return;
    if (PCmdEng_->intEngine_->assertRecognizedCommand(newCmd)) return;
    //STEP TWO: add the new command as an alias in InterpretationEngine
    //must convert cmd to its primitive command representation
    PCmdEng_ -> intEngine_ -> addCommand(newCmd);
    PCmdEng_ -> intEngine_ -> addAlias(newCmd, PCmdEng_->intEngine_->decodeAliasCommand(cmd));
    //STEP THREE: clean up cmd in InterpretationEngine
    PCmdEng_ -> intEngine_  -> removeAlias(cmd);
    PCmdEng_ -> intEngine_ -> removeCommand(cmd);
}

//primitive CLA routers
void CommandEngine::setTextOnly() {
    PCmdEng_->gameManager_->disableGUI();
}

//set randomness seed
void CommandEngine::setSeed(string seed) {
    //convert string to unisgned int
    unsigned int seedInt = stoul(seed.c_str(), 0, 10);
    //srand has effect globally
    PCmdEng_->gameManager_->setSeed(seedInt);
}

//change default scriptfile path name
void CommandEngine::setScriptfile(string filename) {
    PCmdEng_->gameManager_->setDefaultFilePath(filename);
}

//set starting level
void CommandEngine::setStartLevel(string lvl) {
    //check validity
    unsigned int level = stoul(static_cast<string>(lvl), 0, 10);
    PCmdEng_->gameManager_->setStartLvl(level);
}

//disable bonus feautures
void CommandEngine::disableBonus() {
    //we only want to turn off one bonus
    //which is level 5
    PCmdEng_->gameManager_->setMaxLvl(4);
}

//POST MOVE functions
//moveBlock as required after a move
void CommandEngine::initiatePostMove() {
    PCmdEng_->gameManager_->executePostMove();
}

//terminate program with success
void CommandEngine::terminateProgram() {
    exit(EXIT_SUCCESS);
}