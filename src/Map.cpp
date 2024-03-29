#include "main.hpp"

static const int ROOM_MAX_SIZE = 16;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;

class BspListener : public ITCODBspCallback{
    private:
        Map &map;
        bool mirror;
        int roomNum, mirRoomNum; //room number
        int lastx, lasty, mirlastx, mirlasty; //center of last roon

    public:
        BspListener(Map &map, bool mirror) : map(map), mirror(mirror), roomNum(0){}

        bool visitNode(TCODBsp *node, void *userData) {
                if ( node->isLeaf() ) {
                    bool withActors = (bool)userData;
                    int x,y,w,h;
                    // dig a room
                    //TCODRandom *rng=TCODRandom::getInstance(); 
                    w=map.rng->getInt(ROOM_MIN_SIZE, node->w-2);
                    h=map.rng->getInt(ROOM_MIN_SIZE, node->h-2);
                    x=map.rng->getInt(node->x+1, node->x+node->w-w-1);
                    y=map.rng->getInt(node->y+1, node->y+node->h-h-1);
                    
                    map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1, withActors);

                    if ( roomNum != 0 ) {
                        // dig a corridor from last room
                        map.dig(lastx,lasty,x+w/2,lasty);
                        map.dig(x+w/2,lasty,x+w/2,y+h/2);
                    }
                    lastx=x+w/2;
                    lasty=y+h/2;
                    roomNum++;
                    
                    //Mirror
                    x = (map.width - x - w);
                    
                    map.createRoom(mirRoomNum == 0, x, y, x+w-1, y+h-1, withActors);

                    if ( mirRoomNum != 0 ) {
                        // dig a corridor from last room
                        map.dig(mirlastx,mirlasty,x+w/2,mirlasty);
                        map.dig(x+w/2,mirlasty,x+w/2,y+h/2);
                    }
                    mirlastx=x+w/2;
                    mirlasty=y+h/2;
                    mirRoomNum++;
                }
                return true;
        }
};

Map::Map(int width, int height) : width(width),height(height) {
    seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
}
void Map::init(bool withActors){
    rng = new TCODRandom(seed, TCOD_RNG_CMWC);
    tiles=new Tile[width*height];
    map = new TCODMap(width,height);
    TCODBsp bsp(15,0,((width - 15)/2),height);
    bsp.splitRecursive(rng, 18, ROOM_MIN_SIZE, ROOM_MIN_SIZE, 1.5f, 1.5f);
    BspListener listener(*this, false);
    bsp.traverseInvertedLevelOrder(&listener, (void *)withActors);
}

Map::~Map() {
    delete [] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const {
    return !map->isWalkable(x,y);
}
 
bool Map::isExplored(int x, int y) const {
    //TODO: turn back on
    return true;//tiles[x+y*width].explored;
}

bool Map::isInFov(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height){
        return false;
    }
    if (map->isInFov(x,y)){
        tiles[x+y*width].explored = true;
        return true;
    }
    return false;
}

void Map::computeFov(){
    map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}


void Map::render() const {
    static const TCODColor darkWall(0,0,100);
    static const TCODColor darkGround(50,50,150);

    static const TCODColor lightWall(130,110,50);
    static const TCODColor lightGround(200,180,50);

    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {

            if (isInFov(x,y)){
                    TCODConsole::root->setCharBackground( x,y, isWall(x,y) ? lightWall : lightGround );
            }else if (isExplored(x,y)){
                    TCODConsole::root->setCharBackground( x,y, isWall(x,y) ? darkWall : darkGround );
            }
        }
    }
}

void Map::dig( int x1, int y1, int x2, int y2){

    if (x2 < x1){
        int tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    if (y2 < y1){
        int tmp = y2;
        y2 = y1;
        y1 = tmp;
    }

    for (int tilex = x1;tilex <= x2; tilex++){
        for (int tiley = y1; tiley <= y2; tiley++){
            map->setProperties(tilex, tiley, true, true);
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors){
    dig(x1,y1,x2,y2);
    if (!withActors){
        return;
    }
    if (first){
        //put player here
        engine.player->x=(x1+x2)/2;
        engine.player->y=(y1+y2)/2;
    }else{
        TCODRandom *rng=TCODRandom::getInstance();
        int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (nbMonsters > 0){
            int x=rng->getInt(x1,x2);
            int y=rng->getInt(y1,y2);
            if (canWalk(x,y)){
                addMonster(x,y);
            }
            nbMonsters--;

        }
        int nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
        while(nbItems > 0){
            int x = rng->getInt(x1,x2);
            int y = rng->getInt(y1,y2);
            if (canWalk(x,y)){
                addItem(x,y);
            }
            nbItems--;
        }
    }
}

bool Map::canWalk(int x, int y) const{

    if (isWall(x,y)){
        return false;
    }

    for (Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++){
        Actor *actor = *iterator;
        if (actor->blocks && actor->x == x && actor->y == y){
            return false;
        }
    }
    return true;
}

void Map::addMachine(int x, int y){
    //TODO
        Actor *eng = new Actor (x, y, '=', "Engine", TCODColor::desaturatedGreen);
        eng->destructible = new MachineDestructible(10,0,"Destroyed Engine");
        eng->ai = new MachineAi();
        engine.actors.push(eng);
}

void Map::addMonster(int x, int y){
    TCODRandom *rng=TCODRandom::getInstance();
    //TODO Randomize race
    int randRace = rng->getInt(1,3);
    //TODO Randomize job
    int randJob = rng->getInt(1,5);
    //TODO Randomize statblock
    int randBlock = rng->getInt(1,4);
    
    Actor *npc = new Actor(x,y,randRace, randJob, randBlock);
    npc->destructible = new MonsterDestructible(10,0,"corpse");
    npc->attacker = new Attacker(3);
    npc->ai = new IdleNPCAi();    
    engine.actors.push(npc);
    /*
    if (rng->getInt(0,100) < 80) {
        //add enemy
        Actor *orc = new Actor (x, y, 'o', "orc", TCODColor::desaturatedGreen);
        orc->destructible = new MonsterDestructible(10,0,"orc corpse");
        orc->attacker = new Attacker(3);
        orc->ai = new MonsterAi();
        engine.actors.push(orc);


    }else{
        Actor *troll = new Actor (x, y, 'T', "troll", TCODColor::darkerGreen);
        troll->destructible = new MonsterDestructible(16,1,"troll corpse");
        troll->attacker = new Attacker(5);
        troll->ai = new MonsterAi();
        engine.actors.push(troll);

    }
     */
}

void Map::addItem(int x, int y){
    
    TCODRandom *rng = TCODRandom::getInstance();
    int dice = rng->getInt(1,100);
    
    if (dice < 7){
        //HEalth Potion
        Actor *healthPotion = new Actor(x,y, '!', "Health Potion", TCODColor::violet);
        healthPotion->blocks = false;
        healthPotion->pickable = new Healer(4);
        engine.actors.push(healthPotion);
    }else if (dice < 7+10){
        //lightbning bolt
        Actor *scrollOfLightningBolt = new Actor(x,y, 'S',"Scroll of Lightning bolt", TCODColor::lightYellow);
        scrollOfLightningBolt->blocks = false;
        scrollOfLightningBolt->pickable = new LightningBolt(5,20);
        engine.actors.push(scrollOfLightningBolt);
    }else if (dice < 70+10+10){
        //fireball
        Actor *scrollOfFireball = new Actor(x, y, 'S', "Scroll of fireball", TCODColor::red);
        scrollOfFireball->blocks = false;
        scrollOfFireball->pickable = new Fireball(3, 12);
        engine.actors.push(scrollOfFireball);
    }else{
        Actor *scrollOfConfusion = new Actor(x, y, 'S', "Scroll of Confusion", TCODColor::cyan);
        scrollOfConfusion->blocks = false;
        scrollOfConfusion->pickable = new Confuser(10,8);
        engine.actors.push(scrollOfConfusion);
    }
    
}

