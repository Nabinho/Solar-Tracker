#ifndef CARDKB_H
#define CARDKB_H

class CardKB {

  private:
    int address = 0x5F;
  public:
    CardKB();
    void begin();
    char read();

};

#endif
