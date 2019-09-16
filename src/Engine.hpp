class Engine {
public :
    TCOD_key_t lastKey;
    TCOD_mouse_t mouse;

    enum GameStatus{
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;
    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
    int fovRadius;
    int screenWidth;
    int screenHeight;
    Gui *gui;
    
    Actor *getActor(int x, int y) const;
    
    Actor *getClosestMonster(int x, int y, float range) const;
 
    Engine( int screenWidth, int screenHeight);
    ~Engine();
    void update();
    void render();
    void sendToBack(Actor *actor);
    bool pickATile(int *x, int *y, float range = 0.0f);   
    void init();
    void term();
    void load();
    void save();
};
 


extern Engine engine;
