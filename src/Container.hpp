class Container : public Persistent{
    
    public:
        int size; //max size of actors
        TCODList<Actor *> inventory;
        
        Container(int size);
        ~Container();
        bool add(Actor *actor);
        void remove(Actor *actor);
        void save(TCODZip &zip);
        void load(TCODZip &zip);
};

