#include "mbed.h"
#include "ThisThread.h"
#include "NHD_0216HZ.h"
#include <cstdio>
#include <stdio.h>

//Define the frequency of basic music notes
# define Si     493.88
# define La     440.00
# define So     392.00
# define FaS    369.99
# define Fa     349.23
# define Mi     329.63
# define Re     293.66
# define Do     261.63
# define No     0 
# define Do5    523
# define Re5    587
# define Mi5    659
# define Fa5    698
# define So5    784
//Define the beat length (e.g. whole note, half note) 
# define b1     0.5     //whole
# define b2     0.25    //half
# define b3     0.125   //quarter
# define b4     0.075   //eight

/*
//Jingle Bells                
float note[] = {Mi,No,Mi,No,Mi,No, Mi,No,Mi,No,Mi,No, Mi,No,So,No,Do,No,Re,No,Mi,No, Fa,No,Fa,No,Fa,No,Fa,No, Fa,No,Mi,No,Mi,No,Mi,No,Mi,No, Mi,Re,No,Re,Mi, Re,No,So,No};
float beat[] = {b3,b3,b3,b3,b2,b2, b3,b3,b3,b3,b2,b2, b3,b3,b3,b3,b3,b3,b3,b3,b2,b1, b3,b3,b3,b3,b3,b3,b3,b3, b3,b3,b3,b3,b3,b3,b4,b4,b4,b4, b2,b3,b3,b2,b2, b2,b2,b2,b2};
char* names[] = {"Mi","No","Mi","No","Mi","No", "Mi","No","Mi","No","Mi","No", "Mi","No","So","No","Do","No","Re","No","Mi","No", "Fa","No","Fa","No","Fa","No","Fa","No", "Fa","No","Mi","No","Mi","No","Mi","No","Mi","No", "Mi","Re","No","Re","Mi", "Re","No","So","No"}; 
//int *sequence = new int[(sizeof(note) / sizeof(note[0]))];
int sequence[(sizeof(note) / sizeof(note[0]))];
*/

//Star spangled                                 //11                                     //23                                        //35        
float note[] = {So,No,Mi,No,Do,No,Mi,No,So,No,Do5,No, Mi5,No,Re5,No,Do5,No,Mi,No,FaS,No,So,No, Do5,No,Re5,No,Mi5,No,Fa5,No,Re5,No,Do5,No};
float beat[] = {b3,b3,b3,b3,b2,b2,b2,b2,b2,b2,b1,b1,  b3, b3,b3, b3,b2,b2,b2,b2,b2, b2,b1,b1, b2,b2,b2,b2,b1,b1,b2,b2,b1,b1,b1,b1};
char* names[] = {"So","No","Mi","No","Do","No","Mi","No","So","No","Do5","No", "Mi5","No","Re5","No","Do5","No","Mi","No","FaS","No","So","No", "Do5","No","Re5","No","Mi5","No","Fa5","No","Re5","No","Do5","No"}; 
//int *sequence = new int[(sizeof(note) / sizeof(note[0]))];
int sequence[(sizeof(note) / sizeof(note[0]))];


//Happy Birthday                                 //11                                          //25                                       //37
float note2[] = {So,No,So,No,La,No,So,No,Do5,No,Si,No, So,No,So,No,So5,No,Mi5,No,Do5,No,Si,No,La,No, Fa5,No,Fa5,No,Mi5,No,Do5,No,Re5,No,Do5,No}; 
float beat2[] = {b3,b3,b3,b3,b2,b2,b2,b2,b2,b2,b1,b1,  b3,b3,b3,b3,b2,b2,b2,b2,b2,b2,b2,b2,b1,b1,    b3, b3,b3, b3,b2, b2,b2, b2,b2, b2,b1, b1};
char* names2[] = {"So","No","So","No","La","No","So","No","Do5","No","Si","No","So","No","So","No","So5","No","Mi5","No","Do5","No","Si","No","La","No", "Fa5","No","Fa5","No","Mi5","No","Do5","No","Re5","No","Do5","No"};
//int *sequence2 = new int[(sizeof(note2) / sizeof(note2[0]))];
int sequence2[(sizeof(note2) / sizeof(note2[0]))]; 

//Twinkle Twinkle                    //7               //13                     //21                //27
float note3[] = {Do,No,Do,No,So,No,So,No, La,No,La,No,So,No, Fa,No,Fa,No,Mi,No,Mi,No, Re,No,Re,No,Do,No};
float beat3[] = {b3,b3,b3,b3,b3,b3,b3,b3, b3,b3,b3,b3,b2,b2, b3,b3,b3,b3,b3,b3,b3,b3, b3,b3,b3,b3,b2,b2};
char* names3[] = {"Do","No","Do","No","So","No","So","No", "La","No","La","No","So","No", "Fa","No","Fa","No","Mi","No","Mi","No", "Re","No","Re","No","Do","No"};
//int *sequence3 = new int[(sizeof(note3) / sizeof(note3[0]))];
int sequence3[(sizeof(note3) / sizeof(note3[0]))];

//Failure
float fail[] = {Si,No,Fa,No,So5,No} ;
float fail_beats[] = {b1,b1,b1,b1,b1,b1} ;
char* names4[] = {"Si", "No", "Fa", "No", "So5", "No"};


PwmOut speak(D3);

//Define the time ticker
Ticker T;

//Static variable to iterate through note and beat arrays
static int k;

AnalogIn potentiometer(A5);
DigitalIn button1(D2);
DigitalIn button2(D8);
DigitalIn button3(D9);
DigitalIn button4(D4);
DigitalIn button5(D12);
DigitalOut led1(D5);
DigitalOut led2(D6);
DigitalOut led3(D7);
DigitalOut led4(D15);

float correct = 0.0;
float completion = 0.0;

void starFull();
bool starSections(int kVal, int upperBound);
void birthdayFull();
bool birthdaySections(int kVal, int upperBound);
void twinkleFull();
bool twinkleSections(int kVal, int upperBound);
void failFull();

int main() {
    init_spi();
    init_lcd();

    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;
    

    const char* textArray[] = {"Star-Spangled Banner", "Happy Birthday", "Twinkle Twinkle"};
    int arraySize = sizeof(textArray) / sizeof(textArray[0]);
    int scrollPosition = 0;
    

    while (1) {
        clear_lcd();
        set_cursor(0, 0);
        print_lcd(textArray[scrollPosition]);

        correct = 0.0;
        completion = 0.0;

        wait(0.5);  // Adjust the delay as needed

        // Update scroll position based on potentiometer input
        float potValue = potentiometer.read();
        scrollPosition = static_cast<int>(potValue * arraySize);

        if (scrollPosition >= arraySize) {
            scrollPosition = arraySize - 1;
        }

        // Check if the button is pressed
        if (button1 == 0) {
            // Button is pressed, turn on the corresponding LED
            if (strcmp(textArray[scrollPosition], "Star-Spangled Banner") == 0) {
                int stuf  = 0;
                bool stop = true;
                //led1 = 0;
                switch (stuf) {
                case 0: starFull(); stuf++;
                wait(1);
                case 1: stop = starSections(0, 12);stuf++; if(!stop)break;
                case 2: stop = starSections(12,24);stuf++; if(!stop)break;
                case 3: stop = starSections(24,36);stuf++;if(!stop)break;
                
                //user wins and program plays full song
                clear_lcd();
                print_lcd("You Win");
                starFull();
                }
    
            } else if (strcmp(textArray[scrollPosition], "Happy Birthday") == 0) {
                // Similar logic for led2
                //led2 = 0;
                int stuf = 0;
                bool stop = true;
                switch(stuf){
                case 0: birthdayFull(); stuf++;
                wait(1);
                case 1: stop = birthdaySections(0, 12); stuf++; if(!stop) break;
                case 2: stop = birthdaySections(12, 26); stuf++; if(!stop) break;
                case 3: stop = birthdaySections(26, 38); stuf++; if(!stop) break;
                //user wins and program plays full song
                clear_lcd();
                print_lcd("You Win");
                birthdayFull();
                }


                // Turn off the LED after playing the music
                //led2 = 1;
            } else if (strcmp(textArray[scrollPosition], "Twinkle Twinkle") == 0) {
                //led3 = 0;
                int stuf = 0; 
                bool stop = true;
                switch(stuf){
                case 0: twinkleFull(); stuf++;
                wait(1);
                case 1: stop = twinkleSections(0, 14); stuf++; if(!stop) break;
                case 2: stop = twinkleSections(14, 28); stuf++; if(!stop) break;
                //user wins and program plays full song
                clear_lcd();
                print_lcd("You Win");
                twinkleFull();
            }
            }

            // Wait for a moment (adjust as needed)
            wait(2);

            // Turn off all LEDs
            led1 = 1;
            led2 = 1;
            led3 = 1;
            led4 = 1;
        }
    }
}

void starFull(){
    k = 0;  // start by considering the first music note
            
                while (k < sizeof(note) / sizeof(note[0])) {
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;
                    if (note[k] == No) {
                        speak.write(0);
                    } else {
                        if (names[k] == "Do" || names[k] == "Do5") {
                            led1 = 0;
                            sequence[k] = 1;
                        } else if (names[k] == "Mi" || names[k] == "Re5") {
                            led2 = 0;
                            sequence[k] = 2;
                        } else if (names[k] == "FaS" || names[k] == "Mi5") {
                            led3 = 0;
                            sequence[k] = 3;
                        } else if (names[k] == "So" || names[k] == "Fa5") {
                            led4 = 0;
                            sequence[k] = 4;
                        }

                        float frequency = note[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);
                    }

                    wait(beat[k]);
                    k++;
                }
                for (int i = 0; i < sizeof(note) / sizeof(note[0]); i++){
                    printf("%d ", sequence[i]);
                }
                printf("\n");
}

bool starSections(int kVal, int upperBound){
    k = kVal; //Note interator   PATAMETER.
    float length = sizeof(note) / sizeof(note[0]);
    

                //Playing 1st section
                for (int i = k; i < upperBound; i++){  //PARAMETER modify upper bound to match song section size 
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;
                    if (note[k] == No) {
                        speak.write(0);
                    } else {                              //mapping note names to the corresponding LEDs to be turned on as the song is playing
                         if (names[k] == "Do" || names[k] == "Do5") {
                            led1 = 0;
                        } else if (names[k] == "Mi" || names[k] == "Re5") {
                            led2 = 0;
                        } else if (names[k] == "FaS" || names[k] == "Mi5") {
                            led3 = 0;
                        } else if (names[k] == "So" || names[k] == "Fa5") {
                            led4 = 0;
                        }

                        float frequency = note[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);
                    }

                    wait(beat[k]);
                    k++;
                }
                k = kVal;   //PARAMETER. Same as above
                
                //asking user for input
                for (int i = k; i < upperBound; i+=2){  //PATAMETER: Sectio upper bound
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;

                    ThisThread::sleep_for(10);
                    while(button2 == 1 && button3 == 1 && button4 == 1 && button5 == 1){}
                    int buttonPressed = 0;
                    if(button2 == 0){          //maping button2 as button 1 (since it signifies the first led)

                        buttonPressed = 1;
                    } else if(button3 == 0){
                        buttonPressed = 2;

                    }else if(button4  == 0){
                        buttonPressed = 3;

                    }else if(button5 == 0){
                        buttonPressed = 4;

                    }
                    
                    if (buttonPressed == sequence[i]){
                        if(buttonPressed == 1){
                            led1 = 0;
                            led2 = 1;
                            led3 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 2){
                            led2 = 0;
                            led1 = 1;
                            led3 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 3){
                            led3 = 0;
                            led1 = 1;
                            led2 = 1;
                            led4 = 1;
                            
                        }
                        else if(buttonPressed == 4){
                            led4 = 0;
                            led1 = 1;
                            led2 = 1;
                            led3 = 1;
                            
                        }
                        float frequency = note[i];
                            speak.period(1 / frequency);
                            speak.write(0.05);
                            ThisThread::sleep_for(100);
                            speak.write(0);
                            correct = correct + 1;
                    
                    }
                    else{
                            completion = (correct / (length / 2)) * 100;
                            char completionString[20];
                            sprintf(completionString, "%.2f%% Completed :(", completion);
                            printf("Game Over\n");
                            failFull();
                            clear_lcd();
                            print_lcd("You Lose");
                            set_cursor(0, 1);
                            print_lcd(completionString);
                            wait(3);
                            return false;
                        }
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;    
                    ThisThread::sleep_for(500);
                }
                return true;
}

void birthdayFull(){
    k = 0;  // start by considering the first music note

                while (k < sizeof(note2) / sizeof(note2[0])) {
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;
                    if (note2[k] == No) {
                        speak.write(0);
                        sequence2[k] = 0;
                    } else {
                        if (names2[k] == "Do" || names2[k] == "Do5" || names2[k] == "So" || names2[k] == "Mi5" || names2[k] == "Fa5") {
                            led1 = 0;
                            sequence2[k] = 1;
                        } else if (names2[k] == "Re" || names2[k] == "La") {
                            led2 = 0;
                            sequence2[k] = 2;
                        } else if (names2[k] == "Mi" || names2[k] == "Si") {
                            led3 = 0;
                            sequence2[k] = 3;
                        } else if (names2[k] == "Fa"|| names2[k] == "Do5" || names2[k] == "Re5" || names2[k] == "So5") {
                            led4 = 0;
                            sequence2[k] = 4;
                        }

                        float frequency = note2[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);
                    }

                    wait(beat2[k]);
                    k++;
                }
                for (int i = 0; i < sizeof(sequence2) / sizeof(sequence2[0]); i++){
                    printf("%d ", sequence2[i]);
                }
                printf("\n");
}

bool birthdaySections(int kVal, int upperBound){  //THIS MAY CAUSE ISSUES
    k = kVal; //Note interator   PATAMETER. 
    float length = sizeof(note) / sizeof(note[0]);

                //Playing 1st section
                for (int i = k; i < upperBound; i++){  //PARAMETER modify upper bound to match song section size 
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;
                    if (note2[k] == No) {
                        speak.write(0);
                    } else {
                        if (names2[k] == "Do" || names2[k] == "Do5" || names2[k] == "So" || names2[k] == "Mi5" || names2[k] == "Fa5") {
                            led1 = 0;
                        } else if (names2[k] == "Re" || names2[k] == "La") {
                            led2 = 0;
                        } else if (names2[k] == "Mi" || names2[k] == "Si") {
                            led3 = 0;
                        } else if (names2[k] == "Fa"|| names2[k] == "Do5" || names2[k] == "Re5" || names2[k] == "So5") {
                            led4 = 0;
                        }

                        float frequency = note2[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);

                    }

                    wait(beat2[k]);
                    k++;
                }
                k = kVal;   //PARAMETER. Same as above
                
                //asking user for input
                for (int i = k; i < upperBound; i+=2){  //PATAMETER: Sectio upper bound
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;

                    ThisThread::sleep_for(10);
                    while(button2 == 1 && button3 == 1 && button4 == 1 && button5 == 1){}
                    int buttonPressed = 0;
                    if(button2 == 0){          //maping button2 as button 1 (since it signified the first led)

                        buttonPressed = 1;
                    } else if(button3 == 0){
                        buttonPressed = 2;

                    }else if(button4  == 0){
                        buttonPressed = 3;

                    }else if(button5 == 0){
                        buttonPressed = 4;

                    }
                                        
                    if (buttonPressed == sequence2[i]){
                        if(buttonPressed == 1){
                            led1 = 0;
                            led2 = 1;
                            led3 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 2){
                            led2 = 0;
                            led1 = 1;
                            led3 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 3){
                            led3 = 0;
                            led1 = 1;
                            led2 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 4){
                            led4 = 0;
                            led1 = 1;
                            led2 = 1;
                            led3 = 1;
                        }
                        float frequency = note2[i];
                            speak.period(1 / frequency);
                            speak.write(0.05);
                            ThisThread::sleep_for(100);
                            speak.write(0);
                            correct = correct + 1;
                    }
                    else{
                            completion = (correct / (length / 2)) * 100;
                            char completionString[20];
                            sprintf(completionString, "%.2f%% Completed :(", completion);
                            printf("Game Over\n");
                            failFull();
                            clear_lcd();
                            print_lcd("You Lose");
                            set_cursor(0, 1);
                            print_lcd(completionString);
                            wait(3);
                            return false;
                        }
                    ThisThread::sleep_for(500);
                }
                k = 0;
                return true;
}

void twinkleFull(){
        k = 0;  // start by considering the first music note
            
                while (k < sizeof(note3) / sizeof(note3[0])) {
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;
                    if (note3[k] == No) {
                        speak.write(0);
                    } else {
                        if (names3[k] == "Do" || names3[k] == "Do5" || names3[k] == "So") {
                            led1 = 0;
                            sequence3[k] = 1;
                        } else if (names3[k] == "Re" || names3[k] == "La") {
                            led2 = 0;
                            sequence3[k] = 2;
                        } else if (names3[k] == "Mi" || names3[k] == "Si") {
                            led3 = 0;
                            sequence3[k] = 3;
                        } else if (names3[k] == "Fa") {
                            led4 = 0;
                            sequence3[k] = 4;
                        }

                        float frequency = note3[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);
                    }

                    wait(beat3[k]);
                    k++;
                }
                for (int i = 0; i < sizeof(sequence3) / sizeof(sequence3[0]); i++){
                    printf("%d ", sequence3[i]);
                }
                printf("\n");
}

bool twinkleSections(int kVal, int upperBound){  
    k = kVal; //Note interator   PATAMETER. 
    float length = sizeof(note) / sizeof(note[0]);

                //Playing 1st section
                for (int i = k; i < upperBound; i++){  //PARAMETER modify upper bound to match song section size 
                    //printf("k =  %d\n", k);
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;
                    if (note3[k] == No) {
                        speak.write(0);
                    } else {
                        if (names3[k] == "Do" || names3[k] == "Do5" || names3[k] == "So" || names3[k] == "Mi5" || names3[k] == "Fa5") {
                            led1 = 0;
                        } else if (names3[k] == "Re" || names3[k] == "La") {
                            led2 = 0;
                        } else if (names3[k] == "Mi" || names3[k] == "Si") {
                            led3 = 0;
                        } else if (names3[k] == "Fa"|| names3[k] == "Do5" || names3[k] == "Re5" || names3[k] == "So5") {
                            led4 = 0;
                        }

                        float frequency = note3[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);

                    }

                    wait(beat3[k]);
                    k++;
                }
                k = kVal;   //PARAMETER. Same as above
                
                //asking user for input
                for (int i = k; i < upperBound; i+=2){  //PATAMETER: Section upper bound
                    led1 = 1;
                    led2 = 1;
                    led3 = 1;
                    led4 = 1;

                    ThisThread::sleep_for(10);
                    while(button2 == 1 && button3 == 1 && button4 == 1 && button5 == 1){}
                    int buttonPressed = 0;
                    if(button2 == 0){          //maping button2 as button 1 (since it signified the first led)

                        buttonPressed = 1;
                    } else if(button3 == 0){
                        buttonPressed = 2;

                    }else if(button4  == 0){
                        buttonPressed = 3;

                    }else if(button5 == 0){
                        buttonPressed = 4;
                    }
                                        
                    if (buttonPressed == sequence3[i]){
                        if(buttonPressed == 1){
                            led1 = 0;
                            led2 = 1;
                            led3 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 2){
                            led2 = 0;
                            led1 = 1;
                            led3 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 3){
                            led3 = 0;
                            led1 = 1;
                            led2 = 1;
                            led4 = 1;
                        }
                        else if(buttonPressed == 4){
                            led4 = 0;
                            led1 = 1;
                            led2 = 1;
                            led3 = 1;
                        }
                        float frequency = note3[i];
                        speak.period(1 / frequency);
                        speak.write(0.05);
                        ThisThread::sleep_for(50);
                        speak.write(0);
                        correct = correct + 1;
                        
                    }
                    else{
                            completion = (correct / (length / 2)) * 100;
                            char completionString[20];
                            sprintf(completionString, "%.2f%% Completed :(", completion);
                            printf("Game Over\n");
                            failFull();
                            clear_lcd();
                            print_lcd("You Lose");
                            set_cursor(0, 1);
                            print_lcd(completionString);
                            wait(3);
                            return false;
                        }
                    ThisThread::sleep_for(500);
                }
                k = 0;
                return true;
}

void failFull(){ //play failure sequence
     k = 0;  // start by considering the first music note
            
                while (k < sizeof(fail) / sizeof(fail[0])) {
                    led1 = 0;
                    led2 = 0;
                    led3 = 0;
                    led4 = 0;
                    if (fail[k] == No) {
                        speak.write(0);
                    } 
                    else {
                        float frequency = fail[k];
                        speak.period(1 / frequency);
                        speak.write(0.05);
                    }

                    wait(fail_beats[k]);
                    k++;
                }
                led1 = 1;
                led2 = 1;
                led3 = 1;
                led4 = 1;

}




