#include "Game.hpp"

int main() {
	
	
	TicTacToe::Player * p1 = new TicTacToe::UserPlayer();
	TicTacToe::Player * p2 = new TicTacToe::COMPlayer("ABCD");
	TicTacToe::Player * p3 = new TicTacToe::COMPlayer("ABCD2");
	TicTacToe::Game game;
	
	game.start(p1, p3);

	while (game.isRunning()) {
		game.process();
		game.drawBoardGame();
	}

	/*
	int countGames = 0;
	do {
	while (game.isRunning()) {
	game.process();
	}

	game.restart();
	countGames++;
	} while (countGames < 600000);
	*/

	//TicTacToe::Player * p1 = new TicTacToe::UserPlayer();



	p1->save();
	p2->save();
	system("pause");
}