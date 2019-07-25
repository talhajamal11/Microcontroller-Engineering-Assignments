/* Talha Jamal 10138573 Group 2 */
/* this program is supposed to vary the frequency at which the Blue LED lights up between 0.5Hz and 3Hz. */


#include "mbed.h"
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

class Potentiometer {
    private:
    AnalogIn InputSignal;
    float VDD, currentPC, currentVolts;
    public:
    Potentiometer(PinName pin, float V) : InputSignal(pin), VDD(V){}
    float amplitudeVolts(void) {return (InputSignal.read()*VDD);}
    float amplitudePC(void) {return InputSignal.read();}
    void sample (void){
        currentPC = InputSignal.read();
        currentVolts = currentPC * VDD;
        }
        float getCurrentVolts(void) {return currentVolts;}
        float getCurrentPC(void) {return currentPC;}
};
        
class BLUELED   {
        private: 
        typedef enum {None, Blue} LEDColourType;
        LEDColourType state;
        LED blue;
        
        public:
        BLUELED(PinName p1):blue(p1){
            state = None;
            }
        void BLUE(void)  {
            switch(state){
                case (None)   :
                    blue.on();
                    state = Blue;
                    break;
                case (Blue) :
                    blue.off();
                    state = None;
                    }
                }
};
        
class RotationRate {
    private:
    const float smin, smax, fmin, fmax;
    float gradient, intercept, period;
    Timeout to;
    BLUELED* Bled;
    Potentiometer* pot;
    
    float calc_gradient(void) {return (fmax-fmin)/(smax-smin);}
    float calc_intercept(void) {return fmax - gradient*smax;}
    
    public:
    RotationRate(Potentiometer* p,
                 BLUELED* r,
                 float xmn, float xmx, float ymn, float ymx)
                 : pot(p), Bled(r), smin(xmn), smax(ymx), fmin(ymn), fmax(ymx) {
             gradient = calc_gradient();
             intercept = calc_intercept();
             to.attach(callback(this, &RotationRate::updateRate), 0.01);
             pc.printf("RotationRate Constructor\n");
 };            
void updateRate(void)   {
         Bled->BLUE();
         period = 1.0f/(gradient*pot->getCurrentVolts()+intercept);
         to.attach(callback(this, &RotationRate::updateRate), period/3.0f);
         }
 };
   int main(void)   {
             Potentiometer* leftHand = new Potentiometer(A0, 3.3);
             BLUELED* r = new BLUELED(D8);
             RotationRate rr(leftHand, r, 0.0, 3.3, 0.5, 3.0);
             
             while(1) {
                 leftHand->sample();
                 wait(0.01);
             }
     }