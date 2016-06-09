#ifndef LCDMenu_HelperFuncs
#define LCDMenu_HelperFuncs
#include "WProgram.h"
#include <LiquidCrystal.h>

class HelperFuncs {
	public:
		HelperFuncs(LiquidCrystal *l) { lcd = l; }
		void CreateChars();
		bool DecisionHelper(char* header);
		char* EnterTextHelper(String header, boolean isPassword);
	private:
		LiquidCrystal *lcd;
};

#endif