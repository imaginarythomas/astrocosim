#include <stdio.h>
#include "main.hpp"

Attacker::Attacker(float power): power(power){

}

void Attacker::attack(Actor *owner, Actor *target){
    if (target->destructible && ! target->destructible->isDead()){
        if (power -target->destructible->defense > 0){
                engine.gui->message(owner==engine.player ? TCODColor::red : TCODColor::lightGrey,"%s attacks %s for %g damage.", owner->name, target->name, power - target->destructible->defense); 
        }else{
                engine.gui->message(TCODColor::lightGrey,"%s attacks %s for 0 damage.", owner->name, target->name); 
        }
        target->destructible->takeDamage(target, power);
    }else{
        engine.gui->message(TCODColor::lightGrey,"%s attacks %s with no effect", owner->name, target->name); 
    }
    
}

