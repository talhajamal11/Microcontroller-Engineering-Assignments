/* Talha Jamal 10138573 Group 2 
In this task I was supposed to use a ticker inside the class to generate a soudn of 8000 Hz frequency*/

#include "mbed.h"

class Speaker {
    private: 
        DigitalOut Spkr;
        Ticker ticker;   
    public:
        Speaker(PinName pin) : Spkr(pin) {Spkr=1;}
        void ON(void) {Spkr = 0;}
        void OFF(void) {Spkr = 1;}       
        
        void input(double frequency)    {
           while(1) {
               double time = ((1.0/frequency)/2.0) ;
           ticker.attach(callback(this, &Speaker::ON), time);
           wait(time);
           ticker.attach(callback(this, &Speaker::OFF), time);
           wait(time);
           }
       }
   };

int main () {
     Speaker sound(D6); 
     sound.input(8000);
    };