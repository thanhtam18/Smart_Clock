#ifndef MY_BTN_H
#define MY_BTN_H

#include <Arduino.h>
#include <OneButton.h>

class Button{
private:
  OneButton button;
  int value;
  void (*fun1)() = NULL;
  void (*fun2)() = NULL;
public:
  explicit Button(uint8_t pin):button(pin) {
    button.attachClick([](void *scope) { ((Button *) scope)->Clicked();}, this);
    button.attachDoubleClick([](void *scope) { ((Button *) scope)->DoubleClicked();}, this);
  }

  void initFunction(void(*clicked)(), void(*doubleClicked)()){
    this->fun1 = clicked;
    this->fun2 = doubleClicked;
  }

  void Clicked(){
    fun1();
  }

  void DoubleClicked(){

    fun2();
  }

  void handle(){
    button.tick();
  }
};


#endif