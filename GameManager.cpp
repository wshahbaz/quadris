#include <iostream>
#include "GameManager.h"
#include "TextDisplay.h"
#include "GUIDisplay.h"
#include "CommandEngine.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
////////////////////// GAME MANAGER CLASS START //////////////////////////
//////////////////////////////////////////////////////////////////////////

//initialize singleton cmdEngine_ to nullptr
shared_ptr<GameManager> GameManager::gm_ = nullptr;

PImplGameManager::PImplGameManager(): board_{nullptr}, command_{nullptr}, seed_(4), startLvl_(0), guiEnabled(true), maxLvl_{5} {}

GameManager::GameManager() {
    gameManager_ = shared_ptr<PImplGameManager> (new PImplGameManager);
    //get instances of singletons
    gameManager_ -> board_ = BoardSubject::instance();
    text_ = shared_ptr<TextDisplay> (new TextDisplay(gameManager_->board_));
}


//initialization function - sets any member vars and processes that cannot be set during ctor
void GameManager::initialize(int argc, char** argv) {
    //cannot link command engine during ctor due to circular dependencies on PImpls
    gameManager_ -> command_ = CommandEngine::instance();
    gameManager_ -> command_ -> processCLAs(argc, argv);
    gameManager_ -> board_ -> initialize();
    if (gameManager_->guiEnabled) {
        //gtkmm should not take any command line argument of ours
        gui_ = shared_ptr<GUIDisplay> (new GUIDisplay(gameManager_->board_, 1, argv));
    }
}

//game start function
//sets up any additional data before printing out board and accepting input
void GameManager::startGame() {
    cout << "=================================" << endl;
    cout << "           QUADRIS!" << endl;
    cout << "=================================" << endl;
    //seed should be set here has a game level var
    srand(gameManager_->seed_);
    //start level cannot exceed max level
    if (gameManager_->startLvl_ > gameManager_->maxLvl_){
        gameManager_->startLvl_ = gameManager_->maxLvl_;
    }
    gameManager_->board_->setSartLevel(gameManager_->startLvl_);
    gameManager_->board_->setMaxLevel(gameManager_->maxLvl_);
    //if start level is 0, do nothing,
    //else:
    if (gameManager_->startLvl_ != 0) {
        gameManager_->board_->restart(gameManager_->startLvl_); //otherwise shows floating block
    } else {
        gameManager_->board_->notifyObservers();
        gameManager_->board_->changeFloatingBlockToNextBlock();
        gameManager_->board_->notifyObservers();
    }
    //set up any prelimary data, until infinite looping on commands
    while (true) {
        gameManager_ -> command_ -> processCommands(cin);
    }
}

//retriev global singleton instance of GamaManager
shared_ptr<GameManager> GameManager::instance() {
    if (gm_ == nullptr) {
        gm_ = shared_ptr<GameManager> (new GameManager());
    }
    return gm_;
}

//route move function on board
void GameManager::move(Direction dir) {
    gameManager_ -> board_ -> move(dir);
}

//route rotation function on board
void GameManager::rotate(Rotation rot) {
    gameManager_ -> board_ -> rotate(rot);
}

//route leveladjustment to board
void GameManager::levelAdjustment(LevelAdj lvlAdj) {
    gameManager_ -> board_ -> levelAdjustment(lvlAdj);
}

//route drop function to board
void GameManager::drop() {
    gameManager_ -> board_ -> drop();
}

//route hint function call to board
void GameManager::getHint() {
    gameManager_ -> board_ -> getHint();
}

//route randomness adjustment function to board
void GameManager::setRandom(bool ran, string path) {
    gameManager_ -> board_ -> setRandom(ran, path);
}

//route restart command to board
void GameManager::restart() {
    gameManager_ -> board_ -> restart(gameManager_->startLvl_);
}

//route block override to board
void GameManager::specificBlock(string s) {
    gameManager_ -> board_ -> specificBlock(s);
    gameManager_->board_->notifyObservers();
}

//sets floating block to next block, and retrieves new block for next block
void GameManager::changeFloatingBlockToNextBlock(){
    gameManager_->board_->changeFloatingBlockToNextBlock();
}

//disables gui
void GameManager::disableGUI() {
    gameManager_->guiEnabled = false;
}
	
//sets random generator seeds
void GameManager::setSeed(unsigned int sd){
    gameManager_->seed_ = sd;
}

//sets expected start level
void GameManager::setStartLvl(unsigned int sl){
    gameManager_->startLvl_ = sl;
}

//sets default filepath for scripts and block files
void GameManager::setDefaultFilePath(string newFilePath) {
    changeDefaultFilePath(newFilePath);
}

//routes postMove function to board
void GameManager::executePostMove() {
    gameManager_->board_->postMove();
}

//sets the maximum level that board to reach
void GameManager::setMaxLvl(unsigned int lvl){
    gameManager_->maxLvl_ = lvl;
}
