#ifndef LCDMenu_h
#define LCDMenu_h

#include "WProgram.h"
#include <LiquidCrystal.h>
#include "DataStorage.cpp"
#include "HelperFuncs.h"

typedef void (*FunctionPointer)();
#define SUB 0
#define OPT 1

void* operator new(size_t size);
void operator delete(void* ptr); 

class MenuItem {
	public:
		MenuItem(char *n, MenuItem *p);
		char* getName();
    	MenuItem* getParent();
    	int getType();
    	void setParent(MenuItem *p);
    	virtual void* children(int);
    	virtual void* children(char*);
    	virtual int size();
    	//operator SubMenu() { return static_cast<SubMenu*>(*this); }
    	//operator OptionsMenu() { return dynamic_cast<OptionsMenu<void*>*>(*this); }
	protected:
    	int type;
		char *name;
	    MenuItem *parentMenu;
};

template <class ArrTemplate>
class MenuItemArray {
	public:
		MenuItemArray();
		~MenuItemArray();
    	int size();
    	ArrTemplate* get(int i);
    	ArrTemplate* add(ArrTemplate* m);
		ArrTemplate* operator[](int i) { return items[i]; }
	private:
    	int len;
    	ArrTemplate** items;
	    void resize(int numrows);
		
};

class Option {
	public:
		char* name;
		int oid;
		~Option() { delete val; }
		template <typename T> void setValue(T v) { 
			T *t = new T;
			*t = v;
			val = t;
		}
		void* value() { return val; }
	private:
		void* val;
};

class OptionsMenu : public MenuItem {
	public:
		template <class T> OptionsMenu(char *n, MenuItem *p, T &s);
		~OptionsMenu() { delete setting; }
		template <class T> void addOption(char* n, T v);
    	Option* children(int i);
    	Option* children(char* n);
    	int size();
    	int getSelected();
		void select(int oid);
	private:
		int selected;
		Setting* setting;
		MenuItemArray<Option> options;
};

class SubMenu : public MenuItem {
	public:	
		SubMenu(char *n, SubMenu *p);
		SubMenu(char *n, SubMenu *p, void(*c)());
    	SubMenu* addSubMenu(char *n);
    	SubMenu* addSubMenu(char *n, void(*c)());
		template <class T> OptionsMenu* addOptionsMenu(char *n, T &s);
    	FunctionPointer getSelect();
    	MenuItem* children(int i);
    	MenuItem* children(char* n);
    	int size();
	protected:
	    MenuItemArray<MenuItem> menuItems;
    	void (*selectOverride)();
};

class MainMenu : public SubMenu {
  public: 
    MainMenu(LiquidCrystal &l, int pin);
    ~MainMenu() { delete password; delete helperfuncs; }
    void up();
    void down();
    void select();
    void back();
    void on();
    void off();
    void lock();
    void unlock();
    void setPassword(char *p);
    void reset();
    void refresh();
    boolean isOn();
    boolean isLocked();
    boolean lockEnabled();
    boolean enterDecision();
    boolean enterDecision(char *h);
    /*char* enterText();
    char* enterText(char *h);
    char* enterPassword();
    char* enterPassword(char *h);*/
    HelperFuncs* helper();
  private:
    int ledPin;
    int curY;
    int curSize;
    MenuItem *curMenu;
    void (*curSelect)();
    int firstItemDisplayed;
    int cursorPos;
    LiquidCrystal *lcd;
    void selectDefault();
    void draw();
    boolean menuIsOn;
    boolean doesLock;
    boolean locked;
    char* password;
    HelperFuncs *helperfuncs;
};

typedef SubMenu (*ChildMenu);
typedef OptionsMenu (*OptionMenu);
typedef MainMenu LCDMenu;

#include "LCDMenu.tem"

#endif

