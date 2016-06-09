#include "HelperFuncs.h"
//namespace HelperFuncs {
byte leftArrow[8] = {
	B00000,
	B00100,
	B01000,
	B11111,
	B01000,
	B00100,
	B00000,
	B00000
};
byte rightArrow[8] = {
	B00000,
	B00100,
	B00010,
	B11111,
	B00010,
	B00100,
	B00000,
	B00000
};
byte upArrow[8] = {
	B00000,
	B00100,
	B01110,
	B10101,
	B00100,
	B00100,
	B00100,
	B00000
};
byte downArrow[8] = {
	B00000,
	B00100,
	B00100,
	B00100,
	B10101,
	B01110,
	B00100,
	B00000
};

byte endLetter[8] = {
	B00001,
	B00001,
	B00001,
	B00101,
	B01001,
	B11111,
	B01000,
	B00100
};

#define LEFT_ARROW 0
#define RIGHT_ARROW 1
#define UP_ARROW 2
#define DOWN_ARROW 3
#define END_LETTER 4
/*
char GetCharFromSerial() {
	if(Serial.available() > 0) {
		char inChar = Serial.read();
		return inChar;
	} else return '\r';
}
	*/
void HelperFuncs::CreateChars() {
	lcd->createChar(LEFT_ARROW, leftArrow);
	lcd->createChar(RIGHT_ARROW, rightArrow);
	lcd->createChar(UP_ARROW, upArrow);
	lcd->createChar(DOWN_ARROW, downArrow);
	lcd->createChar(END_LETTER, endLetter);
}

bool HelperFuncs::DecisionHelper(char* header) {
	
	float lastAction = millis();
	float lightsout = 10000;
	float timeout = 20000;
	bool lightsAreOut = false;
	
	boolean decision = false;
	boolean entering = true;
	
	int headerOffset = 10 - (strlen(header) / 2);
	lcd->clear();
	lcd->setCursor(headerOffset, 0);
	lcd->print(header);
	
	lcd->setCursor(0, 1);
	lcd->print(">No");
	lcd->setCursor(0, 2);
	lcd->print("Yes");
	
	lcd->setCursor(0, 3);
	lcd->print("           SEL  BCK ");
	lcd->setCursor(2, 3);
	lcd->write(UP_ARROW);
	lcd->setCursor(7, 3);
	lcd->write(DOWN_ARROW);
	
	while(entering) {
		if(millis() > lastAction + lightsout && !lightsAreOut) {
			lightsAreOut = true;
			digitalWrite(18, LOW);
		}
		
		if(millis() > lastAction + timeout) {
			decision = false;
			entering = false;
		}
		
		if(Serial.available() > 0) {
		//char inChar = GetCharFromSerial();
		//if(inChar != '\r') {
			lastAction = millis();
			if(lightsAreOut) {
				lightsAreOut = false;
				digitalWrite(18, HIGH);
				Serial.flush();
			}
			
			char inChar = Serial.read();
			if(inChar == 'u' || inChar == 'd') {
				decision = !decision;
			}
			if(inChar == 's') {
			entering = false;
			}
			if(inChar == 'b') {
				decision = false;
				entering = false;
			}
			lcd->clear();
			lcd->setCursor(headerOffset, 0);
			lcd->print(header);
			
			if(decision) {
				lcd->setCursor(0, 1);
				lcd->print("No");
				lcd->setCursor(0, 2);
				lcd->print(">Yes");
			} else {
				lcd->setCursor(0, 1);
				lcd->print(">No");
				lcd->setCursor(0, 2);
				lcd->print("Yes");
			}
			
			lcd->setCursor(0, 3);
			lcd->print("           SEL  BCK ");
			lcd->setCursor(2, 3);
			lcd->write(UP_ARROW);
			lcd->setCursor(7, 3);
			lcd->write(DOWN_ARROW);
			
		}
	}
	lcd->clear();
	return decision;
}
	
char* HelperFuncs::EnterTextHelper(String header, boolean isPassword) {
	/*## EXIT CODES ##*/
	#define STILL_ENTERING -1
	#define WITH_RETURN 0
	#define WITH_BACKSPACE 1
	#define WITH_TIMEOUT 2
	
	char head[header.length()+1];
	header.toCharArray(head, header.length()+1);
	
	float lastAction = millis();
	float lightsout = 10000;
	float timeout = 20000;
	boolean lightsAreOut = false;

	int exit_status = STILL_ENTERING;
	char text[] = "__________";
	char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ !";
	int cursorPos = 0;
	int letterPos = 0;
	
	int headerOffset = 10 - (header.length() / 2);
	lcd->clear();
	lcd->setCursor(headerOffset, 0);
	lcd->print(head);
	
	lcd->setCursor(5, 2);
	lcd->print(text);
	lcd->setCursor(5 + cursorPos, 2);
	lcd->print(letters[letterPos]);
	
	lcd->setCursor(0, 3);
	lcd->print("           SEL  DEL ");
	lcd->setCursor(2, 3);
	lcd->write(UP_ARROW);
	lcd->setCursor(7, 3);
	lcd->write(DOWN_ARROW);
	
	/*lcd->setCursor(12, 3);
	lcd->write(LEFT_ARROW);
	lcd->setCursor(17, 3);
	lcd->write(RIGHT_ARROW);*/
	
	while(exit_status == STILL_ENTERING) {
		if(millis() > lastAction + lightsout && !lightsAreOut) {
			lightsAreOut = true;
			digitalWrite(18, LOW);
		}
		
		if(millis() > lastAction + timeout) {
			exit_status = WITH_TIMEOUT;
			text[0] = '_';
		}
		
		//char inChar = GetCharFromSerial();
		//if(inChar != '\r') {
		if(Serial.available() > 0) {
			lastAction = millis();
			if(lightsAreOut) {
				lightsAreOut = false;
				digitalWrite(18, HIGH);
				Serial.flush();
			}
			
			char inChar = Serial.read();
			if(inChar == 'u') {
				letterPos--;
				if(letterPos < 0) letterPos = strlen(letters) -1;
			}
			if(inChar == 'd') {
				letterPos++;
				if(letterPos > strlen(letters) -1) letterPos = 0;
			}
			if(inChar == 's') {
				if(letterPos == strlen(letters) -1) exit_status = WITH_RETURN;
				else {
					if(isPassword) {
						lcd->setCursor(5 + cursorPos, 2);
						lcd->print("*");
					}
					text[cursorPos] = letters[letterPos];
					cursorPos++;
					letterPos = 0;
					if(cursorPos > strlen(text) -1) exit_status = WITH_RETURN;
					
				}
			}
			if(inChar == 'b') {
				lcd->setCursor(5 + cursorPos, 2);
				lcd->print("_");
				cursorPos--;
				letterPos = 0;
				if(cursorPos < 0) exit_status = WITH_BACKSPACE;
				else text[cursorPos] = '_';
			}
			lcd->setCursor(5 + cursorPos, 2);
			if(letterPos == strlen(letters) -1) lcd->write(END_LETTER);
			else lcd->print(letters[letterPos]);
		}
	}
	
	if(exit_status == WITH_TIMEOUT || exit_status == WITH_BACKSPACE) {
		//char newtext[] = "";
		return "";
	}
	
	int strsize;
	for(strsize=0; strsize<strlen(text); strsize++) {
		if(text[strsize] == '_') break;
	}
	
	if(exit_status == WITH_RETURN && strsize == 0) {
		return "#ret";	
	}
	
	char newtext[strsize+1];
	for(int i=0; i<strsize; i++) {
		newtext[i] = text[i];	
	}
	
	newtext[strsize] = '\0';
	return newtext;
}

	
//}