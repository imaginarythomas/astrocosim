#include <stdio.h>
#include <math.h>
#include "main.hpp"
 

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) : x(x),y(y),ch(ch), col(col), name(name), blocks(true), attacker(NULL), destructible(NULL), ai(NULL), pickable(NULL), container(NULL), sentient(NULL) {
    
}

//overloaded constructor for sentients
Actor::Actor(int x, int y, int race, int job, int statBlock) : x(x),y(y), blocks(true), attacker(NULL), destructible(NULL), ai(NULL), pickable(NULL), container(NULL), sentient(NULL) {
    
    sentient = new Sentient(race, job, statBlock);
    
    //name
    TCODNamegen::parse("data/names.txt",TCODRandom::getInstance());
    name = TCODNamegen::generate("king", false);
    TCODNamegen::destroy();
    
    ch = sentient->race->ch;
    col = sentient->job->col;
}

Actor::~Actor(){
    if (attacker) delete attacker;
    if (destructible) delete destructible;
    if (ai) delete ai;
    if (pickable) delete pickable;
    if (container) delete container;
    if (sentient) delete sentient;
}
 
void Actor::render() const {
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update(){
    if (ai) ai->update(this);
}

float Actor::getDistance(int cx, int cy) const{
    int dx = x - cx;
    int dy = y - cy;
    return sqrtf(dx*dx+dy*dy);
}
