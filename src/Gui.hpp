class Gui : public Persistent {
public :
    Menu menu;
    Gui();
    ~Gui();
    void render();
    void message(const TCODColor &col, const char *text, ...);
    void save(TCODZip &zip);
    void load(TCODZip &zip);
    void clear();

protected :
	TCODConsole *con;
	struct Message {
		char *text;
		TCODColor col;
		Message(const char *text, const TCODColor &col);
		~Message();
	};
	TCODList<Message *> log;

	void renderBar(int x, int y, int width, const char *name,
		float value, float maxValue, const TCODColor &barColor,
		const TCODColor &backColor);
	void renderMouseLook();
};

