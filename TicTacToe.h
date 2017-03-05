#ifndef H_TIC_TAC_TOE_2
#define H_TIC_TAC_TOE_2
#include "GameBase.hpp"

#define RUNNING_PROPERTY 1
#define BOARD_GAME 2

class TicTacToe2 : public GameBase {
	public:
		TicTacToe2() : GameBase() {
			setProperty(RUNNING_PROPERTY, new GameProperty<bool>(false));
			setProperty(BOARD_GAME, new GamePropertyArray<int, 9>());
			
			GamePropertyArray<int, 9> * p = static_cast<GamePropertyArray<int, 9>*>(getProperty(BOARD_GAME));
			p->setValue(1, 2);
		};
		void start() {};
		void restart() {};

	private:
		//GameProperty<bool> * running = );
		//GamePropertyArray<int, 10> * gameboard = new GamePropertyArray<int, 10>(NULL);

	/*
	 *************************************************
	 *	Properties Game Tic Tac Toe					 *
	 *************************************************
	*/
	// running;
	//GameProperty<int[9]> dashboard;
	
};
#endif