#include "mbed.h"
#include "math.h"

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(){

    int sample = 400, i, index, flag = 1, number;
    float T, f, t1;
    float ADCdata[400];

    wait(5);
    for (i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        Aout = ADCdata[i];
        wait(1./sample);
    }

    for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
    }
    
    for(i = 1; i < sample - 1; i++){
        if(ADCdata[i] > ADCdata[i + 1] && ADCdata[i] > ADCdata[i - 1] && flag){
            index = i;
            flag = 0;
        }
        else if(ADCdata[i] > ADCdata[i + 1] && ADCdata[i] > ADCdata[i - 1]){
            T = (i - index) * 0.0025;
            i = sample;
        }
    }
    f = 1 / T;
    
    while(1){
        
        if( Switch == 1 ){
            greenLED = 0;
            redLED = 1;
            display = 0;
            
        }else{
            number = int(f);
            redLED = 0;
            greenLED = 1;
            display = table[number / 100];
            wait(1);
            number %= 100;
            display = table[number / 10];
            wait(1);
            number %= 10;
            display = table[number] + 128;
            wait(1);
            
        }
        t1 = T / 400;
        for (i = 0; i < sample*4; i++)
        {
            Aout = (sin(2*M_PI*f*i/sample/4))/2+0.5;
            wait(1./sample/4);
        }
    }

}
