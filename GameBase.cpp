#include "GameBase.hpp"
#include <utility> 

GameBase::GameBase()
{

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

void GameBase::processEvents() {

}