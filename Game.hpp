#ifndef H_GAME_TICTTOE
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <conio.h>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n

#define H_GAME_TICTTOE
#define PLAYER_ONE 1
#define PLAYER_TWO 2

std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

namespace TicTacToe {
	
	using json = nlohmann::json;

	class Moves {
		
		public:
			//Property public
			int wins = 0;
			int loses = 0;
			int drawnCount = 0;

			//Constructors and desctructors
			Moves() {};
			Moves(Moves * p) {
				this->_parent = p;
			}
			Moves(json j) {
				this->load(j);
			};
			Moves(Moves * p, json j) {
				this->_parent = p;
				this->load(j);
			};
			~Moves() {
				//delete[] moves;
			};

			//Gettters and Setters
			Moves * getParent() {
				return _parent;
			}
			void setParent(Moves * p) {
				_parent = p;
			}

			//Methods
			Moves * forward(int index) {
				if (moves[index] == 0)
					moves[index] = new Moves(this);
				return moves[index];
			}

			//Output and loaders
			json toJSON() {
				json t;
				t["countWins"] = wins;
				t["countLoses"] = loses;
				t["countDrawns"] = drawnCount;
				t["moves"] = json::array();
				for (int i = 0; i < 9; i++) {
					t["moves"].push_back((moves[i] == 0) ? nullptr : moves[i]->toJSON());
				}
				return t;
			}
			void load(json j) {
				this->loses = j["countLoses"];
				this->wins = j["countWins"];
				this->drawnCount = j["countDrawns"];
				json jmoves = j["moves"], bufferJson;
				if (jmoves.is_array()) {
					for (int i = 0; i < 9; i++) {
						if (!jmoves[i].is_null()) {
							moves[i] = new Moves(this, jmoves[i]);
						}
					}
				}
			}
		private:
			Moves * moves[9] = { 0 };
			Moves * _parent;
	};


	class Memory {
		protected:
			Moves * root;
			Moves * cursor;
			bool autosave = false;
			std::string token;
		public:
			Memory() {
				root = new Moves();
				token = random_string(20);
			}
			Memory(const std::string token) {
				this->token = token;
				std::ifstream i(token + ".json");
				if (i.is_open()) {
					json j;
					i >> j;
					root = new Moves(j);
				} else {
					root = new Moves();
				}				
			}
			virtual ~Memory(void){
				if (autosave)
					this->save();
				delete root;
			}

			bool save() {
				json t = root->toJSON();
				//std::cout << std::setw(4) << t << std::endl;
				std::ofstream o(this->token + ".json");
				o <<  t << std::endl;
				return true;
			}

			void output() {
				json t = root->toJSON();
				std::cout << std::setw(4) << t << std::endl;
			}

			Moves * getCursor() {
				return cursor;
			}

			Memory& forward(const int index) {
				cursor = cursor->forward(index);
				return *this;
			};

			Memory& back() {
				cursor = cursor->getParent();
				//if (cursor == NULL)
				//	cursor = root;
				return *this;
			};

			Memory& goFirst() {
				cursor = root;
				return *this;
			};
	};

	class Player : public Memory {

	public:
		Player() : Memory() {};
		Player(const std::string token) : Memory(token) {};
		virtual int makeMove() = 0;
		virtual void win() {
			Moves * c = cursor;
			do {
				c->wins++;
				c = c->getParent();
			} while (c != 0);
		};

		virtual void drawn() {
			Moves * c = cursor;
			do {
				c->drawnCount++;
				c = c->getParent();
			} while (c != 0);
		};

		virtual void lose() {
			Moves * c = cursor;
			do {
				c->loses++;
				c = c->getParent();
			} while (c != 0);
		};

	protected:
		Player * rival = NULL;
	};

	class COMPlayer : public Player {
		public:
			COMPlayer() : Player() {};
			COMPlayer(const std::string token) : Player(token) {};
			virtual ~COMPlayer() {};
			virtual int makeMove() {
				//Implements memory logic
				//return rand() % 9;
				int pos = rand() % 9 , max = 0, aux = 0;
				Moves * m;
				for (int i = 0; i < 9; i++) {
					m = cursor->forward(i);
					if (m->loses == 0 && m->wins == 0 && m->drawnCount == 0)
						continue;
					aux = (m->wins * 2 + m->drawnCount) / (m->wins + m->drawnCount * 2 + m->loses * 3);
					if (aux > max) {
						max = aux;
						pos = i;
					}
				}
				return pos;
			};
	};

	class UserPlayer : public Player {
		public:
			UserPlayer() : Player() {};
			UserPlayer(std::string token) : Player(token) {};
			virtual ~UserPlayer() {};
			virtual int makeMove() {
				int n = 0;
				char ch;
				printf("\nInput a position (1-9): ");
				do {
					ch = _getch();
				} while ('0' >= ch && ch <= '9');
				return (int) ch - 49;
			};
	};

	struct Collection {
		char layoutDesign[3] = { ' ', 'O', 'X' };
		int possibleWins[8][3] = {
			// 0, 1, 2
			// 3, 4, 5
			// 6, 7, 8

			//Horizontal games
			{0, 1, 2},
			{3, 4, 5},
			{6, 7, 8},

			//Vertical games
			{0, 3, 6},
			{1, 4, 7},
			{2, 5, 8},

			//Diagonal games
			{0, 4, 8},
			{2, 4, 6}
		};
	};

	Collection c;

	class Game {
		public:
		/**
		 * Method to start the game TicTacToe
		 * @arguments:
		 *   p : Player One
		 *   p2 : Player Two
		*/
		void start(Player * p, Player  * p2) {
			//Random seed..
			srand(time(NULL));


			//The player must be setted
			assert(p != nullptr || p2 != nullptr);

			// You cannot start another game in progress
			// If it's necessary to open another game call restart() instead of!
			assert(running == false);
			running = true;
			this->currentPlayer = 0;
			this->players[0] = p;
			this->players[1] = p2;

			//Setting the players
			p->goFirst();
			p2->goFirst();
			drawBoardGame();
		};

		bool isRunning() {
			return running;
		}

		/**
		 * Restart game method
		 * - Reset the Memory curso to top position
		 * - Reset board game
		*/
		void restart() {
			//Random seed..
			srand(time(NULL));
			this->players[0]->goFirst();
			this->players[1]->goFirst();
			currentPlayer = 0;
			for (int i = 0; i < 9; i++) boardGame[i] = 0;
			//winner = NULL;
			running = true;
		}

		void process() {
			
			if (!running)
				return;
			int move = NULL;
			do {
				move = this->players[currentPlayer]->makeMove();
			} while (this->boardGame[move] != 0);

			this->boardGame[move] = (currentPlayer + 1);
			this->players[0]->forward(move);
			this->players[1]->forward(move);
			int result = checkGame();

			if (result == 1 || result == 2) {
				this->players[currentPlayer]->win();
				this->players[(currentPlayer + 1) & 1]->lose();
				//std::cout << "Winner " << (currentPlayer + 1) << std::endl;
				running = false;
			} 
			else if (result == 3) {
				this->players[0]->drawn();
				this->players[1]->drawn();
				//std::cout << "Drawn Game" << std::endl;
				running = false;
			}
			else if (result == 0) {
				currentPlayer = (currentPlayer + 1) & 1;
			}
		}

		int checkGame() {
			//TODO IMPLEMENTS DRAW GAME
			int flagPlayer = 0;
			int i = 0;
			for (i = 0; i < (sizeof(c.possibleWins) / sizeof(*c.possibleWins)); i++){
				if (boardGame[c.possibleWins[i][0]] == 0)
					continue;
				flagPlayer = boardGame[c.possibleWins[i][0]];
				for (int j = 1; j < 3; j++) {
					if (boardGame[c.possibleWins[i][j]] != flagPlayer) {
						flagPlayer = 0;
						break;
					}
				}
				if (flagPlayer != 0) {
					return flagPlayer;
				}
			}
			//Drawn game check..
			for (i = 0; i < 9; i++)
				if (boardGame[i] == 0)
					return 0;
			return 3;
		}

		void drawBoardGame() {
			system("cls");
			
			for (int i = 0; i < 9; i++) {
				if (i % 3 == 0) {
					std::cout << " \n " << c.layoutDesign[boardGame[i]];
				} else {
					std::cout << " | " << c.layoutDesign[boardGame[i]];
				}
			}

			for (int i = 0; i < 9; i++) {
				std::cout << ", " << boardGame[i];
			}
		};

		void close() {

		}

	private:
		void clearGame() {
			for (int i = 0; i < 9; i++) boardGame[i] = 0;
			running = false;
			currentPlayer = 0;
			//nner = NULL;
		};
		int boardGame[9] = { 0 };
		bool running = false;
		int currentPlayer = 0;
		Player * players[2];
	};
}

#endif // !H_GAME_TICTTOE
