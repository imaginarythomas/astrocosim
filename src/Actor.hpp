class Actor : public Persistent {
public :
    int x,y; // position on map
    int ch; // ascii code
    TCODColor col; // color
    const char *name;  //name
    bool blocks; //can pass throuhgh?
    Attacker *attacker; //can damage
    Destructible *destructible; //can be destroyed
    Ai *ai; //can self update
    Pickable *pickable; //can be picked and used
    Container *container; //can contain actors
    Sentient *sentient; //Can be sentient
    float getDistance(int cx, int cy) const; 
    TCODNamegen *nameGen;
    
    //Generic actor
    Actor(int x, int y, int ch, const char *name, const TCODColor &col);
    //Sentient Actor
    Actor(int x, int y, int race, int job, int statBlock);
    ~Actor();  
    
    bool interaction(int successRange);
    
    void update();
    void render() const;
    void save(TCODZip &zip);
    void load(TCODZip &zip);
};


