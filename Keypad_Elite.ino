#include <Keypad.h>

const byte ROWS = 5; //number of rows an columns
const byte COLS = 5; //on the matrix

//define the symbols on the buttons of the keypads
//we'll use an array of indexes, then use them to 
//fetch the actual key from key_map
uint8_t key_idx_map[ROWS][COLS] = {
  { 0,  1,  2,  3, -1},
  { 4,  5,  6,  7, -2},
  { 8,  9, 10, 11, -3},
  {12, 13, 14, 15, -4},
  {16, 17, 18, 19,  0}
};
//key map. this allows to have six different mappings.
//I'm only using 4 switches for this, so I have 5 lists here
uint8_t key_map[5][20] = { {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 30, 31, 32, 33},
                           {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 30, 31, 32, 33},
                           {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 30, 31, 32, 33},
                           {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 30, 31, 32, 33},
                           {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 30, 31, 32, 33}
                         };

byte rowPins[ROWS] = {13, 12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4, 3}; //connect to the column pinouts of the keypad
uint8_t buf[8] = { 0 }; // USB key buffer. byte 0 is a modifier (bitmap for shift, ctrl, etc.)
                        // byte 1 is reserverd. the remaining are the detected keys.
uint8_t idx = 2;

bool repeat = false;
bool pressed = false;
uint8_t table = 0;
//initialize an instance of class NewKeypad
//it'll map physical rows and columns to the array given
Keypad kpd = Keypad( makeKeymap(key_idx_map), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);
  kpd.setDebounceTime(30);
  kpd.setHoldTime(800); //without this we risk having multiple presses sent to quickly 
  delay(2000);
}

void loop(){
    kpd.getKeys();
    for (int i=0; i<LIST_MAX; i++) {
        if ( (kpd.key[i].kchar <= -1) && (kpd.key[i].kchar >= -4) && kpd.key[i].stateChanged ) { 
            //Serial.println("switch loop");
            switch (kpd.key[i].kstate) {
                case PRESSED:
                case HOLD:
                    {
                        //Serial.println("Switch " + String(kpd.key[i].kchar*-1,DEC) + " turned on");
                        table = kpd.key[i].kchar * -1;
                        clear_buf;
                    }
                    break;
                case RELEASED:
                    {
                        //Serial.println("Switch " + String(kpd.key[i].kchar*-1,DEC) + " turned off");
                        table = 0;
                        clear_buf;
                    }
                    break;
                default:
                    {
                        //Serial.println("Switch " + String(kpd.key[i].kchar*-1,DEC) + " idle");
                        table = 0;
                        clear_buf;
                    }
                    break;
            }
            delay(100);
            return;
        } else if (kpd.key[i].kchar >= 0) {
            //Serial.println("index " + String(i,DEC) + " " +String(kpd.key[i].kchar,DEC) + String(kpd.key[i].kstate,DEC));
            switch (kpd.key[i].kstate) {
                case PRESSED:
                    {
                       //Serial.println("\nkey " + String(kpd.key[i].kchar,DEC) + " pressed " + String(kpd.key[i].kstate,DEC));
                        buf[idx]=key_map[table][kpd.key[i].kchar];
                        pressed=true;
                        repeat=false;
                        idx++;
                        continue;
                    }
                    break;
                case HOLD:
                    {
                       //Serial.println("\nkey " + String(kpd.key[i].kchar,DEC) + " held" + String(kpd.key[i].kstate,DEC));
                        buf[idx]=key_map[table][kpd.key[i].kchar];
                        pressed=true;
                        repeat=true;
                        idx++;
                        continue;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    //Serial.print("\nend of key loop: ");
    if (pressed) {
       //Serial.print("\rpressed: " +String(pressed,DEC)+" repeat "+String(repeat,DEC) );
        for (int i=2;i<8;i++) {
           //Serial.print(String(buf[i],DEC)+" ");
        }
       //Serial.println("");
        Serial.write(buf,8);
        clear_buf();
        if (repeat) {
            delay(100);
        } else {
            delay(700);
        }
        pressed=false;
    }
}

void clear_buf() 
{
    for (int i=0; i<8; i++)
    {
        buf[i]=0;
    }
    pressed=false;
    idx=2;
    Serial.write(buf, 8);	// Release key  
}
