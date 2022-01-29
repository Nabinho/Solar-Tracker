#include "CardKB.h"
#include <Wire.h>

CardKB::CardKB(){
  this->address = address;
}

void CardKB::begin(){
  Wire.begin();
  return true;
}

char CardKB::read(){

  char character;
  Wire.requestFrom(address, 1);
  if (Wire.available()) {
    character = Wire.read();
    if(character != 0){
      return character;
    } else {
      return false;
    }
  }

}
