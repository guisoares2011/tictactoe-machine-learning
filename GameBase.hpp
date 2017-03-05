#ifndef H_GAME_BASE
#define H_GAME_BASE

/******************************
 * Abstract class for games
 *    - start()
 *	  - restart()
 *	  - getLastStatusEvent()
 *	  - setStatusEvent()
 *	  - processEvents()
 *	  - setPlayer()
 *	  - getPlayers()
*/
#include <vector>
#include <map>
#include <string>

class Property {};
template<class T> 
class GameProperty : public Property {
	public:
		GameProperty() { propertyValue = NULL; }
		GameProperty(T value) { propertyValue = value; }
		void setValue(T val) { propertyVal = val; }
		T getValue() { return propertyVal; }
	private:
		T propertyValue;
};

template <class T>
class GamePropertyArray : public Property {
	public:
		GamePropertyArray() {}
		void setValue(int i, T val) {
			propertyArray[i] = val;
		}
		T getValue(int i) {
			return propertyArray[i];
		}
	private:
		std::vector<T> propertyArray;
};

typedef std::map<int, Property*> GameStatus;

class PlayerBase {};
class Event{};


class GameBase
{
	public:
		GameBase();
		virtual void start() = 0;
		virtual void restart() = 0;
		virtual int getLastStatusEvent();
		Property * GameBase::getProperty(const int i);
		virtual void setProperty(const int i, Property * p);
		//virtual std::vector<PlayerBase&> getPlayers();
		virtual ~GameBase(void) {};
		//virtual void setPlayer(PlayerBase&);
		//virtual void setPlayer(int i, PlayerBase&);
		virtual void processEvents();
	
	protected:
		int lastStatus = 0;
		GameStatus status;
		std::map<int, PlayerBase&> players;
};

#endif // H_GAME_BASE

