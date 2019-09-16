class Ai : public Persistent {
public:

    virtual void update(Actor *owner) = 0;
    static Ai *create(TCODZip &zip);
protected:
    enum AiType{
        MONSTER,CONFUSED_MONSTER,PLAYER,MACHINE
    };

};

class PlayerAi : public Ai{
public:
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
protected:
    bool moveOrAttack(Actor *owner, int targetx, int targety);
    void handleActionKey(Actor *owner, int ascii);
    Actor *chooseFromInventory(Actor *owner);
};
//NPC just whistlin' dixie
class IdleNPCAi : public Ai{
public:
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);        
};

//NPC going somewhere
class TravlingNPCAi : public Ai{
public:
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
private:
    int destinationX;
    int destinationY;
    void moveOrInteract(Actor *owner, int destinationX, int destinationY);
};

class MonsterAi : public Ai{
public:
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
protected:
    void moveOrAttack(Actor *owner, int targetx, int targety);
    int moveCount;
    
};

class ConfusedMonsterAi : public Ai{
    public:
        ConfusedMonsterAi(int nbTurns, Ai *oldAi);
        void update(Actor *owner);
        void load(TCODZip &zip);
        void save(TCODZip &zip);
    protected:
        int nbTurns;
        Ai *oldAi;
};

class MachineAi : public Ai {
public:
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
    void op(Actor *op);
};