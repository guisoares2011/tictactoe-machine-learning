#include "GameBase.hpp"
#include <utility> 

GameBase::GameBase()
{
	events.assign(MAX_EVENTS, &emptyEvent);
}

void GameBase::setProperty(const int i, Property * p) {
	status.insert(std::make_pair(i, p));
}

Property * GameBase::getProperty(const int i) {
	return status.at(i);
}

int GameBase::getLastStatusEvent() {
	return lastStatus;
}

void GameBase::setEvent(int action, EventInterface* _event) {
	events[action] = _event;
}

int GameBase::execEvent(int action) {
	try {
		EventInterface* ev = events.at(action);
		return ev->exec(status, players);
	} catch(...)
	{ return INVALID_EVENT; }
	
}
/*
std::vector<PlayerBase&> GameBase::getPlayers()
{
	return players;
}

void GameBase::setPlayer(PlayerBase& p) {
	players.push_back(p);
};

void GameBase::setPlayer(int i, PlayerBase& p) {
	players[i] = p;
};*/
