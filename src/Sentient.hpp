class StatBlock {
public:
    StatBlock(int type);
    ~StatBlock();
    
    int ship = 0; //Ships
    int pers = 0; //Personal
    int comp = 0; //Computers
    int comb = 0; //Combat
    int merc = 0; //Mercentile
    
    int brav = 0; //Brave
    int tact = 0; //Tactics
    int obsr = 0; //Observation
    int subt = 0; //Subterfuge
    int finn = 0; //Finesse
    
    //Types of generated statblocks
    static const int TYPE_GENERAL = 1;
    static const int TYPE_WEAKROLLS = 2;
    static const int TYPE_STRONGROLLS = 3;
    static const int TYPE_POOL = 4;
    
    //Types of stats
    static const int STAT_SHIP = 1; //Ships
    static const int STAT_PERS = 2; //Personal
    static const int STAT_COMP = 3; //Computers
    static const int STAT_COMB = 4; //Combat
    static const int STAT_MERC = 5; //Mercentile
    
    static const int STAT_BRAV = 6; //Brave
    static const int STAT_TACT = 7; //Tactics
    static const int STAT_OBSR = 8; //Observation
    static const int STAT_SUBT = 9; //Subterfuge
    static const int STAT_FINN = 10; //Finesse
    
    void generate(int type); //roll a statblock by type
};

class Race {
public:
    Race(int type);
    ~Race();
    const char *name;
    int ch; //character
    StatBlock *statBlock;
};

class Job {
public:
    Job(int type);
    ~Job();
    const char *name;
    TCODColor col; // color
    StatBlock *statBlock;
};

class Sentient : public Persistent{
public:
    Sentient(int race, int job, int statBlock);
    ~Sentient();
    Race *race;
    StatBlock *statBlock;
    Job *job;
    
    
    void save(TCODZip &zip);
    void load(TCODZip &zip);
    
    Actor contest(Sentient *a, Sentient *b, int type); //contest between two sentients
    
    //Races
    static const int RACE_HUMAN = 1;
    static const int RACE_AUTOMA = 2;
    static const int RACE_BULLKAT = 3;
    static const int RACE_HY = 4;
    static const int RACE_FYNNI = 5;
    
    //Jobs
    static const int JOB_PILOT = 1;
    static const int JOB_TRADER = 2;
    static const int JOB_ENGINEER = 3;
    static const int JOB_ARISTOCRAT = 4;
    static const int JOB_BOUNTY_HUNTER = 5;
};