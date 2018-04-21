#include <digitalWriteFast.h>
int midisi=1;
#include <MIDIUSB.h>
 int note=29;  
#define NUM_ROWS 5
#define NUM_COLS 8
int channel=0;
int velocity=127;
#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);
//MIDI baud rate
#define SERIAL_RATE 57600
//#define SERIAL_RATE 31250

// Pin Definitions
int outcol[NUM_COLS]{A0,15,14,16,10,9,8,7};
// Row input pins
const int row1Pin = 2;
const int row2Pin = 3;
const int row3Pin = 4;
const int row4Pin = 5;
const int row5Pin = 6;




// 74HC595 pins


boolean keyPressed[NUM_ROWS][NUM_COLS];
uint8_t keyToMidiMap[NUM_ROWS][NUM_COLS];

// bitmasks for scanning columns
int bits[] =
{ 
  B11111110,
  B11111101,
  B11111011,
  B11110111,
  B11101111,
  B11011111,
  B10111111,
  B01111111
};
 
void setup()
{
 

  for(int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
  {
    for(int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr)
    {
      keyPressed[rowCtr][colCtr] = false;
      keyToMidiMap[rowCtr][colCtr] = note;
      note++;
    }
  }

  // setup pins output/input mode


  pinMode(row1Pin, INPUT_PULLUP);
  pinMode(row2Pin, INPUT_PULLUP);
  pinMode(row3Pin, INPUT_PULLUP);
  pinMode(row4Pin, INPUT_PULLUP);
  pinMode(row5Pin, INPUT_PULLUP);
  
   pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT);
  mySerial.begin(SERIAL_RATE);
  if (digitalRead(A2)==0){
    mySerial.begin(31250);
  }
  pinMode(A1,INPUT);
for(int a; a<NUM_COLS;a++){
  pinModeFast(outcol[a],OUTPUT);
}

 //mySerial.begin(SERIAL_RATE);
}
int note2=29;
int abc;

void instrument(byte channel, byte control, byte value) {
  mySerial.write((0xC0|channel));
  mySerial.write(control);
  mySerial.write(value);
  midiEventPacket_t event = {0x0C, 0xC0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
void controlChange(byte channel, byte control, byte value) {
  mySerial.write((0xB0|channel));
  mySerial.write(control);
  mySerial.write(value);
  
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
int inst;
void loop()
{

  for (int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
  {//delay(10);
    //scan next column
    scanColumn(colCtr);
   
   


if(digitalRead(A2)==0&&note2<100){

  note2=note2+12;
  note=note2;
  for(int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
  {
    for(int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr)
    {
      keyPressed[rowCtr][colCtr] = false;
      keyToMidiMap[rowCtr][colCtr] = note;
      note++;
    }
  }
/*if(abc==1){
    
  controlChange(0,0x40,0x7F);


  }
  if(abc==2){
    if(inst<0x7F){
    inst++;
    instrument(0,inst,0);
    }
  }*/
  delay(200);
}
if(analogRead(A3)<300&&note2>6){
  
  note2=note2-12;
  note=note2;
  for(int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
  {
    for(int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr)
    {
      keyPressed[rowCtr][colCtr] = false;
      keyToMidiMap[rowCtr][colCtr] = note;
      note++;
    }
    
  }
  delay(200);
  }/*
if(abc==1){
    
  controlChange(0,0x40,0x00);


  }*/
  if(analogRead(A1)<600&&analogRead(A1)>400){
    if(inst>0){
   inst--;
    instrument(0,inst,0);
    
    }
  delay(200);
}
 if(digitalRead(A1)==0){
    if(inst<127){
   inst++;
    instrument(0,inst,0);
    
    }
  delay(200);
}
if(analogRead(A3)<600&&analogRead(A3)>400){
  abc=1-abc;
if(abc==0){
  controlChange(0,0x40,0x00);
}
if(abc==1){
  controlChange(0,0x40,0x7f);
}



  delay(500);
}


    //get row values at this column
    int rowValue[NUM_ROWS];
    rowValue[0] = !digitalRead(row1Pin);
    rowValue[1] = !digitalRead(row2Pin);
    rowValue[2] = !digitalRead(row3Pin);
    rowValue[3] = !digitalRead(row4Pin);
    rowValue[4] = !digitalRead(row5Pin);


    // process keys pressed
    for(int rowCtr=0; rowCtr<NUM_ROWS; ++rowCtr)
    {
      if(rowValue[rowCtr] != 0 && !keyPressed[rowCtr][colCtr])
      {
        keyPressed[rowCtr][colCtr] = true;
        noteOn(rowCtr,colCtr);
           if(midisi==1){
          MidiUSB.flush();
          }
      }
    }

    // process keys released
    for(int rowCtr=0; rowCtr<NUM_ROWS; ++rowCtr)
    {
      if(rowValue[rowCtr] == 0 && keyPressed[rowCtr][colCtr])
      {
        keyPressed[rowCtr][colCtr] = false;
        noteOff(rowCtr,colCtr);
           if(midisi==1){
          MidiUSB.flush();
          }
      }
    }
  }
}

void scanColumn(int colNum)
{
for(int a; a<NUM_COLS;a++){
  //digitalWriteFast(outcol[a],1);
  pinModeFast(outcol[a],0);
}
  pinModeFast(outcol[colNum],1);
digitalWriteFast(outcol[colNum],0);


 
}

void noteOn(int row, int col)
{ midiEventPacket_t noteOn = {0x09, 0x90 | channel, keyToMidiMap[row][col], velocity};
    if(midisi==1){
 MidiUSB.sendMIDI(noteOn);
 }
 mySerial.write(NOTE_ON_CMD);
 mySerial.write(keyToMidiMap[row][col]);
 mySerial.write(velocity);
}

void noteOff(int row, int col)
{
 mySerial.write(NOTE_OFF_CMD);
 mySerial.write(keyToMidiMap[row][col]);
 mySerial.write(velocity);
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, keyToMidiMap[row][col], velocity};
  if(midisi==1){
 MidiUSB.sendMIDI(noteOff);
  }
}

