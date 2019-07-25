#include "mbed.h"

class Speaker {
        
        
        public: 
        Speaker(PinName pin) : Spkr(pin) {
            }
            void input(double f) {
                while(1){
                    Spkr = 1;
                    wait((1/f)/2);
                    Spkr = 0;
                    wait((1/f)/2);
                    }
                }
                
        private: 
        DigitalOut Spkr; 
        };
        
int main() {
          Speaker sound(D6);
          sound.input(8000);
          }
          







