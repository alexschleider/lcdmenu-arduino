#include "LCDMenu.h"


void* operator new(size_t size) { return malloc(size); }
void operator delete(void* ptr) { free(ptr); }

/*############################################*/
/*########### MenuItem Definitions ###########*/
/*############################################*/

//Public
MenuItem::MenuItem(char *n, MenuItem *p) {
	name = n;
	parentMenu = p;
}

MenuItem* MenuItem::getParent() {
	return parentMenu;
}

char* MenuItem::getName() {
	return name;	
}

int MenuItem::getType() {
	return type;	
}

void MenuItem::setParent(MenuItem *p) {
	parentMenu = p;
}

void* MenuItem::children(int) {
	return NULL;
}

void* MenuItem::children(char*) {
	return NULL;
}

int MenuItem::size() {
	return 0;
}

//Protected

/*############################################*/
/*########### MainMenu Definitions ###########*/
/*############################################*/

//Public
MainMenu::MainMenu(LiquidCrystal &l, int pin) : SubMenu("Main", this) {
	lcd = &l;
	ledPin = pin;
	curY = 0;
	curSize = 0;
	firstItemDisplayed = 0;
	cursorPos = 0;
	curMenu = this;
	menuIsOn = false;
	doesLock = false;
	locked = false;
	helperfuncs = new HelperFuncs(lcd);
	helperfuncs->CreateChars();
}

boolean MainMenu::isOn() {
	return menuIsOn;	
}

boolean MainMenu::isLocked() {
	return locked;	
}

boolean MainMenu::lockEnabled() {
	return doesLock;	
}

void MainMenu::on() {
	digitalWrite(ledPin, HIGH);
	if(doesLock && locked) {
		char *input = helperfuncs->EnterTextHelper("Enter Password", true);
		if(strcmp(password, input) == 0) {
			locked = false;
			on();	
		} else if(strcmp(input, "") == 0) {
			off();
		} else {
			lcd->clear();
			lcd->setCursor(1, 1);
			lcd->print("Incorrect Password");
			delay(2500);
			off();
		}
	} else {
		curSize = curMenu->size();
		menuIsOn = true;
		draw();
	}
}

void MainMenu::off() {
	digitalWrite(ledPin, LOW);
	lcd->clear();
	menuIsOn = false;
}

void MainMenu::lock() {
	if(doesLock) {
		off();
		locked = true;
		reset();
	}
}

void MainMenu::unlock() {
	if(locked) {
		locked = false;
		on();
	}
}

void MainMenu::setPassword(char *p) {
	delete password;
	password = (char*) malloc((strlen(p) +1) * sizeof(char));
	strcpy(password, p);
	if(strcmp(password, "") == 0) doesLock = false;
	else doesLock = true;
}

void MainMenu::up() {
	curY--;
	cursorPos--;
	if(cursorPos < 0) {
		cursorPos = 0;
		firstItemDisplayed--;
	}
	if(curY < 0) {
		curY = curSize - 1;
		cursorPos = 2;
		firstItemDisplayed = curY - 2;
		if(curSize < 3) {
			cursorPos = curY;
			firstItemDisplayed = 0;
		}
	}
	draw();
}

void MainMenu::down() {
	curY++;
	cursorPos++;
	if(cursorPos > 2) {
		cursorPos = 2;
		firstItemDisplayed++;
	}
	if(curY > curSize - 1) {
		curY = 0;
		cursorPos = 0;
		firstItemDisplayed = 0;
	}
	draw();
}

void MainMenu::select() {
	lcd->clear();
	if(curMenu->getType() == SUB){
  		int childType = static_cast<SubMenu*>(curMenu)->children(curY)->getType();
		if(childType == SUB) curSelect = static_cast<SubMenu*>(curMenu->children(curY))->getSelect();
		else curSelect = NULL;
		if (curSelect == NULL) selectDefault();
		else curSelect();
	} else if(curMenu->getType() == OPT) {
		static_cast<OptionsMenu*>(curMenu)->select(curY);
	}
}

void MainMenu::back() {
	curMenu = curMenu->getParent();
	curSize = curMenu->size();
	cursorPos = 0;
	firstItemDisplayed = 0;
	curY = 0;
	draw();
}

void MainMenu::refresh() {
	draw();	
}

void MainMenu::reset() {
	curMenu = this;
	curSize = this->size();
	cursorPos = 0;
	firstItemDisplayed = 0;
	curY = 0;	
}
/*
boolean MainMenu::enterDecision() {
	helperfuncs->setHeader("Are You Sure?");
	return helperfuncs->DecisionHelper();
}

boolean MainMenu::enterDecision(char *h) {
	helperfuncs->setHeader(h);
	return helperfuncs->DecisionHelper();
}

char* MainMenu::enterText() {
	helperfuncs->setHeader("Enter Text");
	return helperfuncs->EnterTextHelper(false);
}

char* MainMenu::enterText(char *h) {
	helperfuncs->setHeader(h);
	return helperfuncs->EnterTextHelper(false);
}

char* MainMenu::enterPassword() {
	helperfuncs->setHeader("Enter Password");
	return helperfuncs->EnterTextHelper(true);
}

char* MainMenu::enterPassword(char *h) {
	helperfuncs->setHeader(h);
	return helperfuncs->EnterTextHelper(true);
}
*/
HelperFuncs* MainMenu::helper() {
	return helperfuncs;	
}
//Private

void MainMenu::draw() {
  lcd->clear();
  for(int i=0; i<3 && i<curSize; i++) {
	  lcd->setCursor(0, i);
	  if(i == cursorPos) lcd->print(">");
	  if(curMenu->getType() == SUB) {
	  	
	  	lcd->print(static_cast<SubMenu*>(curMenu)->children(firstItemDisplayed + i)->getName());
	  	
	  } else if(curMenu->getType() == OPT) {
	  	
	  	if(firstItemDisplayed + i == static_cast<OptionsMenu*>(curMenu)->getSelected()) lcd->print("-");
	  	lcd->print(static_cast<OptionsMenu*>(curMenu)->children(firstItemDisplayed + i)->name);
	  	if(firstItemDisplayed + i == static_cast<OptionsMenu*>(curMenu)->getSelected()) lcd->print("-");
	  }
  }
  lcd->setCursor(0, 3);
  lcd->print(" UP  DOWN ENTER BACK");
  
  //lcd->print(curMenu->getChildByLocation(curY)->getName());
  //lcd->print(curMenu->getChildByLocation(curY)->getSize());
  /*int childType = static_cast<SubMenu*>(curMenu)->children(curY)->getType();
  if(childType == SUB) {
  	lcd->print(static_cast<SubMenu*>(static_cast<SubMenu*>(curMenu)->children(curY))->children(0)->getName());
  } else if(childType == OPT) {
  	lcd->print(static_cast<OptionsMenu<float>*>(static_cast<SubMenu*>(curMenu)->children(curY))->children(0)->getName());
  }*/
}

void MainMenu::selectDefault() {
	if(curMenu->getType() == SUB) {
		curMenu = static_cast<SubMenu*>(curMenu)->children(curY);
	}
	curSize = curMenu->size();
	cursorPos = 0;
	firstItemDisplayed = 0;
	curY = 0;
	draw();
}

/*############################################*/
/*######### LCD Sub Menu Definitions #########*/
/*############################################*/

//Public
SubMenu::SubMenu(char* n, SubMenu *p) : MenuItem(n, p) {
	menuItems = MenuItemArray<MenuItem>();
	selectOverride = NULL;
	type = SUB;
}

SubMenu::SubMenu(char* n, SubMenu *p, void(*c)()) : MenuItem(n, p) {
	menuItems = MenuItemArray<MenuItem>();
	selectOverride = c;
	type = SUB;
}

SubMenu* SubMenu::addSubMenu(char *n) {
	menuItems.add(new SubMenu(n, this));
	return (SubMenu*)(menuItems.get(menuItems.size() - 1));
}

SubMenu* SubMenu::addSubMenu(char *n, void(*c)()) {
	menuItems.add(new SubMenu(n, this, c));
	return (SubMenu*)(menuItems.get(menuItems.size() - 1));
}

MenuItem* SubMenu::children(int i) {
	return menuItems.get(i);
}

MenuItem* SubMenu::children(char* n) {
	int nLen = 0;
	while(n[nLen] != '\0') nLen++;
	for(int i=0; i<size(); i++) {
		boolean match = true;
		MenuItem* s = menuItems.get(i);
		int nameLength = 0;
		while(s->getName()[nameLength] != '\0') nameLength++;
		if(nameLength == nLen) {
			for(int j=0; j<nameLength; j++) {
				if(s->getName()[j] != n[j]) match = false;
			}
			if(match) return s;
		}
	}
	return NULL;
}

int SubMenu::size() {
	return menuItems.size();	
}

FunctionPointer SubMenu::getSelect() {
	return selectOverride;	
}

//Protected

/*############################################*/
/*####### LCD Options Menu Definitions #######*/
/*############################################*/

Option* OptionsMenu::children(int i) {
	return options.get(i);
}

Option* OptionsMenu::children(char* n) {
	int nLen = 0;
	while(n[nLen] != '\0') nLen++;
	for(int i=0; i<size(); i++) {
		boolean match = true;
		Option* s = options.get(i);
		int nameLength = 0;
		while(s->name[nameLength] != '\0') nameLength++;
		if(nameLength == nLen) {
			for(int j=0; j<nameLength; j++) {
				if(s->name[j] != n[j]) match = false;
			}
			if(match) return s;
		}
	}
	return NULL;
}

int OptionsMenu::size() {
	return options.size();	
}

int OptionsMenu::getSelected() {
	return selected;	
}

void OptionsMenu::select(int oid) {
	selected = oid;
	setting->set(options[oid]->value());
}