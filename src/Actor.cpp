#include <stdio.h>
#include <math.h>
#include "main.hpp"
 

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) : x(x),y(y),ch(ch), col(col), name(name), blocks(true), attacker(NULL), destructible(NULL), ai(NULL), pickable(NULL), container(NULL), sentient(NULL) {
    
}

//overloaded constructor for sentients
Actor::Actor(int x, int y, int race, int job, int statBlock) : x(x),y(y), blocks(true), attacker(NULL), destructible(NULL), ai(NULL), pickable(NULL), container(NULL), sentient(NULL) {
    
    sentient = new Sentient(race, job, statBlock);
    
    //name
    nameGen->parse("data/names.txt",TCODRandom::getInstance());

    switch (race){
        case(Sentient::RACE_HUMAN):
            name = strdup( TCODNamegen::generate("human", false));
            break;
        case(Sentient::RACE_BULLKAT):
            name = strdup( TCODNamegen::generate("bullkat", false));
            break;
        case(Sentient::RACE_AUTOMA):
            name = strdup( TCODNamegen::generate("automa", false));
            break;
    }
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

bool Actor::interaction(int successRange){
    
}
