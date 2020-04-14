#include "mbed.h"

DigitalIn button0(SW2);         // sw2 digital input
DigitalIn button1(SW3);         // sw3 digital input
DigitalOut red_led(LED1);       // red LED digital output 
DigitalOut green_led(LED2);     // green LED digitla output

Thread thread1;                 // parallel execute thread 1 for master
Thread thread2;                 // parallel execute thread 2 for slave

Serial pc(USBTX, USBRX);        // UART tx, rx to pc
Serial device1(D12, D11);       // UART tx, rx of master
Serial device2(D1, D0);         // UART tx, rx of slave

char recv_from_pc;              // data receive by pc
char recv_from_k66f;            // data receive by k66f

/* k66f master send data to pc */
void master_thread() 
{
    while (1) {
        if (pc.readable()) {                // if pc can read data
            recv_from_pc = pc.getc();       // pc get data from K66F master
            device1.putc(recv_from_pc);
        } else {
            device1.putc('0');
        }
        wait(0.05);
    }
}

void slave_thread() 
{
    red_led = 1;                                // turn off the led
    green_led = 1;
    while (1) {
        if (device2.readable()) {               // if slave can read data
            recv_from_k66f = device2.getc();    // slave get data from pc
            if (recv_from_k66f == '1') {        // if data is 1, turn on green
                red_led = 1;
                green_led = 0;
            } else if (recv_from_k66f == '2') { // if data is 2, turn on red
                red_led = 0;
                green_led = 1;
            } else {                            // turn off leds
                red_led = 1;
                green_led =1;
            }
            pc.putc(recv_from_k66f);
        }
    }
}

/* turn on two threads */
int main() 
{
    thread1.start(master_thread);
    thread2.start(slave_thread);
}
