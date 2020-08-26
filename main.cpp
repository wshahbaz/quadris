#include "GameManager.h"
#include <memory>



using namespace std;

int main(int argc, char** argv){
	//grab handle of game instance and start game
	shared_ptr<GameManager> game = GameManager::instance();
	game -> initialize(argc, argv);
	game -> startGame();

	
	return 0;
}
