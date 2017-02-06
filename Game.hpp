#ifndef H_GAME_TICTTOE
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
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
			int drawns = 0;

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
				delete[] moves;
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
				t["countDrawns"] = loses;
				t["moves"] = json::array();
				for (int i = 0; i < 9; i++) {
					t["moves"].push_back((moves[i] == 0) ? nullptr : moves[i]->toJSON());
				}
				return t;
			}
			void load(json j) {
				this->loses = j["countLoses"];
				this->wins = j["countWins"];
				this->drawns = j["countDrawns"];
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
				root->setParent(root);
				token = random_string(20);
			}
			Memory(std::string token) {
				std::ifstream i(token + ".json");
				json j;
				i >> j;
				root = new Moves(j);
				root->setParent(root);
				this->token = token;
			}
			~Memory(){
				if (autosave)
					this->save();
				delete root;
			}

			bool save() {
				json t = root->toJSON();
				//std::cout << std::setw(4) << t << std::endl;
				std::ofstream o(this->token + ".json");
				o << std::setw(4) << t << std::endl;
				return true;
			}

			void output() {
				json t = root->toJSON();
				std::cout << std::setw(4) << t << std::endl;
			}

			Moves * getCursor() {
				return cursor;
			}

			Memory& forward(int index) {
				cursor = cursor->forward(index);
				return *this;
			};

			Memory& back() {
				cursor = cursor->getParent();
				if (cursor == NULL)
					cursor = root;
				return *this;
			};

			Memory& goFirst() {
				cursor = root;
				return *this;
			};
	};

	class Player : public Memory {

	public:
		void win() {
			Moves * c = cursor;
			do {
				c->wins++;
				c = c->getParent();
			} while (c != 0);
		}

		void win() {
			Moves * c = cursor;
			do {
				c->drawn++;
				c = c->getParent();
			} while (c != 0);
		}

		void lose() {
			Moves * c = cursor;
			do {
				c->loses++;
				c = c->getParent();
			} while (c != 0);
		}
	};
	class COMPlayer : public Player {};
	class UserPlayer : public Player {};

	struct Collection {
		char layoutDesign[3] = { ' ', 'O', 'X' };
		int possibleWins[8][3] = {
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
		/**
		 * Method to start the game TicTacToe
		 * @arguments:
		 *   p : Player One
		 *   p2 : Player Two
		*/
		void start(Player * p, Player  * p2) {

			//The player must be setted
			assert(p != nullptr || p != nullptr);

			// You cannot start another game in progress
			// If it's necessary to open another game call restart() instead of!
			assert(running == true);

			//Setting the players
			this->currentPlayer = p;
			this->p1 = p;
			this->p2 = p2;
			this->p1->goFirst();
			this->p2->goFirst();
		};

		void restart() {

			this->p1->goFirst();
			this->p2->goFirst();
			currentPlayer = p1;
			for (int i = 0; i < 9; i++) boardGame[i] = 0;
			winner = NULL;
		}

		void process() {
			if (!running)
				return;
		}

		unsigned char checkGame() {
			int flagPlayer = 0;
			for (int i = 0; i < (sizeof(c.possibleWins) / sizeof(c.possibleWins)); i++){
				if (boardGame[c.possibleWins[i][0]] == 0)
					continue;
				flagPlayer = c.possibleWins[i][0];
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
			return '0';
		}

		void drawBoardGame() {
			system("cls");
			for (int i = 0; i < 3; i++) {
				std::cout << "\n\t" << c.layoutDesign[boardGame[i + i * 3]];
				for(int j = i * 3 + 1; j < (i * 3) + 3; j++){
					std::cout << " | " << c.layoutDesign[boardGame[j]];
				}
			}
		};

		void close() {

		}

	private:
		void clearGame() {
			for (int i = 0; i < 9; i++) boardGame[i] = 0;
			winner = NULL;
		};
		int boardGame[9] = { 0 };
		bool running = false;
		Player * currentPlayer;
		Player * p1;
		Player * p2;
		Player * winner;
		Player * loser;
	};
}

#endif // !H_GAME_TICTTOE
