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

class Property {
public:
	virtual ~Property() {};
};
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

template <class T, size_t t = 10, T defaultValue = NULL>
class GamePropertyArray : public Property {
	public:
		GamePropertyArray(void) : propertyArray(t){
			if (defaultValue)
				propertyArray.assign(t, defaultValue);
		}
		virtual ~GamePropertyArray(void) {};
		void setValue(int i, T val) {
			propertyArray[i] = val;
		}
		T getValue(int i) {
			return propertyArray[i];
		}
	private:
		std::vector<T> propertyArray;
};

#define EVENT_INIT 1
#define EVENT_MENU 2
#define EVENT_PROCESS_GAME 3
#define EVENT_DRAW_GAME_BOARD 4
#define EVENT_END_GAME 5
#define EMPTY_EVENT 0
#define INVALID_EVENT -1



class PlayerBase {};

typedef std::map<int, Property*> GameStatus;
typedef std::vector<PlayerBase*> GamePlayers;

class EventInterface{
public:
	virtual int exec(const GameStatus&, const GamePlayers players) = 0;
};

class EmptyEvent : public EventInterface{
public:
	int exec(const GameStatus&, const GamePlayers players) {
		return EMPTY_EVENT;
	}
};


class GameBase
{
	public:
	
		GameBase();
		virtual void start() = 0;
		//virtual void restart() = 0;
		virtual int getLastStatusEvent();
		Property * GameBase::getProperty(const int i);
		virtual void setProperty(const int i, Property * p);
		//virtual std::vector<PlayerBase&> getPlayers();
		virtual ~GameBase(void) {};
		//virtual void setPlayer(PlayerBase*);
		//virtual void setPlayer(int i, PlayerBase*);
		//virtual int processEvents();
	
	protected:
		int MAX_EVENTS = 10;
		typedef std::vector<EventInterface*> GameEvents;
		int lastStatus = 0;
		GameStatus status;
		GameEvents events;
		GamePlayers players;
		int execEvent(int action);
		void setEvent(int action, EventInterface*);
	private:
		EmptyEvent emptyEvent;
};

#endif // H_GAME_BASE

