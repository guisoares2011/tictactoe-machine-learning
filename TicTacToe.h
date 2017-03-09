#ifndef H_TIC_TAC_TOE_2
#define H_TIC_TAC_TOE_2
#include "GameBase.hpp"

//Properties
#define P_RUNNING_PROPERTY 1
#define P_BOARD_GAME 2
#define P_CURRENT_PLAYER 3
#define P_TYPE_GAME 4

namespace GTicTacToe {
	class InitEvent : public EventInterface {
	public:
		int exec(const GameStatus& status, const GamePlayers players) {
			return EVENT_PROCESS_GAME;
		}
	};

	class MenuEvent : public EventInterface {
	public:
		int exec(const GameStatus& status, const GamePlayers players) {
			return EVENT_INIT;
		}
	};

	class TicTacToe : public GameBase {
	public:
		TicTacToe() : GameBase() {

			//Game Properties
			//setProperty(P_RUNNING_PROPERTY, new GameProperty<bool>(false));
			setProperty(P_BOARD_GAME, new BoardGame());
			setProperty(P_CURRENT_PLAYER, new GameProperty<int>(0));
			setProperty(P_TYPE_GAME, new GameProperty<int>(0));

			//Game Events
			setEvent(EVENT_INIT, new InitEvent());
			setEvent(EVENT_MENU, new MenuEvent());
		};
		~TicTacToe() {};
		void start() {
			int _event = EVENT_MENU;
			do {
				_event = execEvent(_event);
			} while (_event != EMPTY_EVENT && _event != INVALID_EVENT);
		};	

	private:
		typedef GamePropertyArray<int, 9> BoardGame;
	};
}
#endif