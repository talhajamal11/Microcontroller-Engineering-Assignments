#include "mbed.h"
#include <iostream>

class Joystick {
private:
    DigitalIn up, down, left, right, fire;     
        
public:
    
    Joystick (PinName u, PinName d, PinName l, PinName r, PinName f): up(u), down (d), left (l), right (r), fire (f) {}
    bool upPosition(void) {return up;}
    bool downPosition(void) {return down;}
    bool leftPosition(void) {return left;}
    bool rightPosition(void) {return right;}
    bool firePosition(void) {return fire;}
};
    
class LED  {
 
private:
    DigitalOut OutputSignal;
 
public: 
    LED(PinName pin) : OutputSignal(pin) {OutputSignal = 1;}
    void ON (void) {OutputSignal = 0;}
    void OFF (void) {OutputSignal = 1;}
};
    
int main () {
    Joystick JS (A2, A3, A4, A5, D4);
    
    LED R(D5);
    LED B(D8);
    LED G(D9);

while(1){
    R.OFF();
    B.OFF();
    G.OFF();
    
    if(JS.upPosition()) {
                   R.OFF();
                   B.OFF();
                   G.ON();
               }
    if(JS.downPosition()) {
                   R.OFF();
                   B.ON();
                   G.OFF();
                   }
    if(JS.leftPosition ()) {
                   R.ON();
                   B.OFF();
                   G.OFF();
                   }
    if(JS.rightPosition ()){
                   R.OFF();
                   B.OFF();
                   G.OFF();    
                   }
    if(JS.firePosition ()){
                   R.OFF();
                   B.OFF();
                   G.OFF();  
           }              
   }
}