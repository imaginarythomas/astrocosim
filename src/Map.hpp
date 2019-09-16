
struct Tile {
    bool explored; // have we seen this 
    Tile() : explored(false) {}
};
 
class Map : public Persistent {
public :
    int width,height;
    bool canWalk(int x, int y) const;
    enum type {
        SHIP,
        BAR
    };
    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    void addMonster(int x, int y);
    void addMachine(int x, int y);
    void addControl(int x, int y);
    void render() const;
    bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    void computeFov();
    void addItem(int x, int y);
    void init(bool withActors);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
    
protected :
    TCODMap *map;
    Tile *tiles;
    long seed;
    TCODRandom *rng;
    friend class BspListener;
    void dig (int x1, int y1, int x2, int y2);
    void createRoom (bool first, int x1, int y1, int x2, int y2, bool withActors);
    void setWall(int x, int y);

};





