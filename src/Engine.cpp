#include "main.hpp"


void Engine::init(){
    player = new Actor(40,25,'@', "Hero",TCODColor::white);
    player->destructible = new PlayerDestructible(30, 2, "your corpse");
    player->attacker = new Attacker(5);
    player->ai = new PlayerAi();
    player->container = new Container(26);
    player->sentient = new Sentient(Sentient::RACE_HUMAN, Sentient::JOB_PILOT, StatBlock::TYPE_GENERAL);
    actors.push(player);
    map = new Map(80,43);
    map->init(true);
    gui->message(TCODColor::red, 
    	"Welcome to Astrocosim");
    gameStatus = STARTUP;
}


Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), player(NULL), map(NULL), fovRadius(10), screenWidth(screenWidth),screenHeight(screenHeight){
    TCODConsole::initRoot(screenWidth, screenHeight, "Astrocosim", false);
    gui = new Gui();
}  
Engine::~Engine(){
    term();
    delete gui;
    
}
void Engine::term(){
    actors.clearAndDelete();
    if (map) delete map;
    gui->clear();
}

void Engine::update() {
    TCOD_key_t key;
        
    if (gameStatus == STARTUP) map->computeFov();
    gameStatus = IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);

    player->update();

    if (gameStatus == NEW_TURN){
        for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++){
            Actor *actor = *iterator;
            if (actor != player) actor->update();
        }
    }

}

void Engine::render() {
	TCODConsole::root->clear();
	// draw the map
	map->render();
	// draw the actors
	for (Actor **iterator=actors.begin();
	    iterator != actors.end(); iterator++) {
		Actor *actor=*iterator;
		if ( actor != player && map->isInFov(actor->x,actor->y) ) {
	        actor->render();
	    }
	}
	player->render();
	// show the player's stats
	gui->render();
}

void Engine::sendToBack(Actor *actor){
    actors.remove(actor);
    actors.insertBefore(actor,0);
}

Actor *Engine::getClosestMonster(int x, int y, float range) const{
    Actor *closest = NULL;
    float bestDistance = 1E6f;
    for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++){
        Actor *actor = *iterator;
        if (actor != player && actor->destructible && !actor->destructible->isDead()){
            float distance = actor->getDistance(x, y);
            if (distance < bestDistance && (distance <= range || range == 0.0f)){
                bestDistance = distance;
                closest = actor;    
            }
        }
    }
    return closest;
}

bool Engine::pickATile(int *x, int *y, float maxRange){
    while(!TCODConsole::isWindowClosed()){
        render();
        //highlight possible range
        for (int cx = 0; cx < map->width; cx++){
            for (int cy = 0; cy < map->height; cy++){
                if (map->isInFov(cx, cy) && (maxRange == 0 || player->getDistance(cx,cy) <= maxRange)){
                    TCODColor col = TCODConsole::root->getCharBackground(cx,cy);
                    col = col * 1.2f;
                    TCODConsole::root->setCharBackground(cx,cy,col);
                }
            }
        }
        
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);
        if (map->isInFov(mouse.cx, mouse.cy) && (maxRange == 0 || player->getDistance(mouse.cx, mouse.cy) <= maxRange)){
            TCODConsole::root->setCharBackground(mouse.cx, mouse.cy, TCODColor::white);
            if (mouse.lbutton_pressed){
                *x = mouse.cx;
                *y = mouse.cy;
                return true;
            }
        }
        if (mouse.rbutton_pressed || lastKey.vk != TCODK_NONE){
            return false;
        }
        TCODConsole::flush();
    }
    return false;   



}


Actor *Engine::getActor(int x, int y) const{
    for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++){
        Actor *actor = *iterator;
        if (actor->x == x && actor->y == y && actor->destructible && !actor->destructible->isDead()){
            return actor;
        }
    }
    return NULL;
}