/* Talha Jamal 10138573 Group 2 */

/* Then program only lights up the LED when the Joystick is pressed once only each time it is pressed instead of being continuously lit up. 
When nothing is pressed the RED LED keeps blinking*/

#include "mbed.h"
#include <iostream>
Serial pc(USBTX, USBRX);
class LED {
    protected:
    DigitalOut OutputSignal;
    public:
    LED(PinName pin) : OutputSignal(pin) {}
    void on (void) {OutputSignal = 0;}
    void off(void) {OutputSignal = 1;}
    void toggle(void) {
        if (OutputSignal.read())
          OutputSignal = 0;
      else
          OutputSignal = 1;
          }
          int status(void) {return OutputSignal.read();}
};

class Potentiometer{
    private:
    AnalogIn InputSignal;
    float VDD, currentSamplePC, currentSampleVolts;
    public:
    Potentiometer(PinName pin, float V) : InputSignal(pin), VDD(V){}
    float amplitudeVolts(void) {return (InputSignal.read()*VDD);}
    float amplitudePC(void) {return InputSignal.read();}
    void sample (void){
        currentSamplePC = InputSignal.read();
        currentSampleVolts = currentSamplePC * VDD;
        }
        float getCurrentSampleVolts(void) {return currentSampleVolts;}
        float getCurrentSamplePC(void) {return currentSamplePC;}
};

class Joystick {
private:
    InterruptIn up, down, left;
    typedef enum {Red, Green, Blue} LEDColourType;
    LED red, green, blue;    
public:
    
    Joystick (PinName u, PinName d, PinName l, PinName p1, PinName p2, PinName p3):up(u), down (d), left (l), red(p1), green(p2), blue(p3){
    up.rise(callback(this, &Joystick::upPosition));
    down.rise(callback(this, &Joystick::downPosition));
    left.rise(callback(this, &Joystick::leftPosition));
    up.fall(callback(this, &Joystick::def));
    down.fall(callback(this, &Joystick::def));
    left.fall(callback(this, &Joystick::def));
    

 }
    void def() {
        red.off();
        blue.off();
        green.off();
        }
    void upPosition() {
        red.off();
        blue.off();
        green.on();
        }
    
    void downPosition() {
        blue.on();
        red.off();
        green.off();
        }
    
    void leftPosition() {
        red.on();
        green.off();
        blue.off();
        }
};
                
class RotationRateUP {
    private:
    const float smin, smax, fmin, fmax;
    float gdt, incpt, period;
    Timeout to;
    Joystick* JOYSTICKUP;
    Potentiometer* pot;
    
    float calc_gdt(void) {return (fmax-fmin)/(smax-smin);}
    float calc_incpt(void) {return fmax - gdt*smax;}
    
    public:
    RotationRateUP(Potentiometer* p,
                 Joystick* jsup,
                 float xmn, float xmx, float ymn, float ymx)
                 : pot(p), JOYSTICKUP(jsup), smin(xmn), smax(ymx), fmin(ymn), fmax(ymx) {
             gdt = calc_gdt();
             incpt = calc_incpt();
             to.attach(callback(this, &RotationRateUP::updateRateUP), 0.01);
             pc.printf("RotationRate Constructor\n");
        };     
     void updateRateUP(void)   {
         JOYSTICKUP->upPosition();
         period = 1.0f/(gdt*pot->getCurrentSampleVolts()+incpt);
         to.attach(callback(this, &RotationRateUP::updateRateUP), period/3.0f);
         }
};
             
class RotationRateDOWN {
    private:
    const float smin, smax, fmin, fmax;
    float gdt, incpt, period;
    Timeout to;
    Joystick* JOYSTICKDOWN;
    Potentiometer* pot;
    
    float calc_gdt(void) {return (fmax-fmin)/(smax-smin);}
    float calc_incpt(void) {return fmax - gdt*smax;}
    
    public:
    RotationRateDOWN(Potentiometer* p,
                 Joystick* jsdown,
                 float xmn, float xmx, float ymn, float ymx)
                 : pot(p), JOYSTICKDOWN(jsdown), smin(xmn), smax(ymx), fmin(ymn), fmax(ymx) {
             gdt = calc_gdt();
             incpt = calc_incpt();
             to.attach(callback(this, &RotationRateDOWN::updateRateDOWN), 0.01);
             pc.printf("RotationRate Constructor\n");
       };      
     void updateRateDOWN(void)   {
         JOYSTICKDOWN->downPosition();
         period = 1.0f/(gdt*pot->getCurrentSampleVolts()+incpt);
         to.attach(callback(this, &RotationRateDOWN::updateRateDOWN), period/3.0f);
         }
 };
 
class RotationRateLEFT {
    private:
    const float smin, smax, fmin, fmax;
    float gdt, incpt, period;
    Timeout to;
    Joystick* JOYSTICKLEFT;
    Potentiometer* pot;
    
    float calc_gdt(void) {return (fmax-fmin)/(smax-smin);}
    float calc_incpt(void) {return fmax - gdt*smax;}
    
    public:
    RotationRateLEFT(Potentiometer* p,
                 Joystick* jsleft,
                 float xmn, float xmx, float ymn, float ymx)
                 : pot(p), JOYSTICKLEFT(jsleft), smin(xmn), smax(ymx), fmin(ymn), fmax(ymx) {
             gdt = calc_gdt();
             incpt = calc_incpt();
             to.attach(callback(this, &RotationRateLEFT::updateRateLEFT), 0.01);
             pc.printf("RotationRate Constructor\n");   
           };  
    void updateRateLEFT(void)   {
         JOYSTICKLEFT->leftPosition();
         period = 1.0f/(gdt*pot->getCurrentSampleVolts()+incpt);
         to.attach(callback(this, &RotationRateLEFT::updateRateLEFT), period/3.0f);
         }     
 };            

   int main(void)   {
             LED R(D5);
             LED B(D8);
             LED G(D9);
             
             Joystick* jsup = new Joystick(A2, A3, A4, D5, D9, D8);
             Joystick* jsdown = new Joystick(A2, A3, A4, D5, D9, D8);
             Joystick* jsleft = new Joystick(A2, A3, A4, D5, D9, D8);
             R.off();
             B.off();
             G.off();
             Potentiometer* leftHand = new Potentiometer(A0, 3.3);
             RotationRateUP rrup(leftHand, jsup, 0.0, 3.3, 0.5, 3.0);
             RotationRateDOWN rrdown(leftHand, jsdown, 0.0, 3.3, 0.5, 3.0);
             RotationRateLEFT rrleft(leftHand, jsleft, 0.0, 3.3, 0.5, 3.0);
             while(1) {
                 leftHand->sample();
                 wait(0.01);
             }
     }
     