#include <stdio.h>
#include <math.h>
#include "main.hpp"

StatBlock::StatBlock(int type){
    
}

void StatBlock::generate(int type){
    //TODO: Type not used yet
    //Rolls
    int roll[10];
    TCODRandom *rng=TCODRandom::getInstance();

    for (int x = 0; x <=9; x++){
        roll[x] = rng->getInt(1,4);
    }
    /*
    switch(type){
        case(StatBlock::TYPE_GENERAL):
        default:
            roll = {4,3,3,3,3,2,2,2,1,1};
            break;
        case(StatBlock::TYPE_WEAKROLLS):
            roll = {4,3,3,3,2,2,1,1,1,1};
            break;
        case(StatBlock::TYPE_STRONGROLLS):
            roll = {4,4,4,3,3,3,2,2,1,1};
            break;
    }
     */
    //Shuffle the roll
    //std::random_shuffle(&roll[0], &roll[9]);
    
    //Distribute the roll.
    ship = roll[0]; //Ships
    pers = roll[1]; //Personal
    comp = roll[2]; //Computers
    comb = roll[3]; //Combat
    merc = roll[4]; //Mercentile
    
    brav = roll[5]; //Brave
    tact = roll[6]; //Tactics
    obsr = roll[7]; //Observation
    subt = roll[8]; //Subterfuge
    finn = roll[9]; //Finesse
}

Race::Race(int type){
    switch(type){
        case(Sentient::RACE_HUMAN):
        default:
            name = "Human";
            ch = 'h';
            statBlock = new StatBlock(StatBlock::TYPE_GENERAL);
            break;
    }    
}
Job::Job(int type){
    switch(type){
        case(Sentient::JOB_PILOT):
        default:
            name = "Pilot";
            col = TCODColor::amber;
            statBlock = new StatBlock(StatBlock::TYPE_GENERAL);
            break;
    }    
}

Sentient::Sentient(int r, int j, int s){
    
    race = new Race(r);
    job = new Job(j);
    statBlock = new StatBlock(s);
    
}
Sentient::~Sentient(){
    
}
