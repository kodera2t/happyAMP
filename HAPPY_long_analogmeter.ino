/// Happy amplifier firmware
/// input:A0 and A1
/// Dec. 2016 kodera2t

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 7
Adafruit_SSD1306 display(OLED_RESET);

int mode,modeflag,oldxval,oldyval;
unsigned long ct;


void setup()   {
  attachInterrupt(0,mode_setting,CHANGE);
  pinMode(2, INPUT);
  int xpos,ypos;                
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done
  ////////////////////////////////////
//  display.clearDisplay();
  display.setRotation(0);
  display.clearDisplay();
  display.setTextSize(1);
//////draw axis and tics///////
  display.drawCircle(16,16,16,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=13;i<16;i++){
    xpos=16-i*cos(j*3.1415/180);
    ypos=16-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
    display.drawCircle(78,16,16,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=13;i<16;i++){
    xpos=78-i*cos(j*3.1415/180);
    ypos=16-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
////////////////////////////////
  display.display();
  mode=0;
}


void loop() {
  float readvalue1,readvalue2;
  double left,right;
  float voltage;
  left=0;
  right=0;
  for(int i=0;i<100;i++){
  readvalue1=abs((analogRead(A0)-512)*10);
  readvalue2=abs((analogRead(A1)-512)*10);
  voltage=3.3*readvalue1/1024.0;
  left=left+readvalue1;
  right=right+readvalue2;
  }
  readvalue1=left/100;
  readvalue2=right/100;
  switch(mode){
  case 0:
  drawaxis();
  drawneedle(readvalue1,readvalue2);
    eraseneedle(readvalue1,readvalue2);
 // showvalue(readvalue1,readvalue2);
  //delay(200);
  break;
  case 1:
  drawaxis();
  drawneedle(readvalue1,readvalue2);
    eraseneedle(readvalue1,readvalue2);
  showvalue(readvalue1,readvalue2);  
  break;
  case 2:
  showbar(readvalue1,readvalue2);
  break;
  }
}

void drawaxis(){
  int xpos,ypos;
  if(modeflag==1){
      //testscrolltext();
  // Clear the buffer.
  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(32,32);
  display.setTextSize(1);
//////draw axis and tics///////
  display.drawCircle(16,16,16,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=13;i<16;i++){
    xpos=16-i*cos(j*3.1415/180);
    ypos=16-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
    display.drawCircle(78,16,16,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=13;i<16;i++){
    xpos=78-i*cos(j*3.1415/180);
    ypos=16-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
////////////////////////////////
  display.display();

    modeflag=0;
  }

  
}

void drawneedle(int i,int j){
  int xpos2,ypos2;
  xpos2=16-10*cos(i/1024.0*3.1415);
  ypos2=16-10*sin(i/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,16,16,WHITE);
  xpos2=78-10*cos(j/1024.0*3.1415);
  ypos2=16-10*sin(j/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,78,16,WHITE);
  display.display();  
}

void eraseneedle(int i,int j){
  int xpos3,ypos3;
  xpos3=16-10*cos(i/1024.0*3.1415);
  ypos3=16-10*sin(i/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,16,16,BLACK);
  xpos3=78-10*cos(j/1024.0*3.1415);
  ypos3=16-10*sin(j/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,78,16,BLACK);
//  display.display();  
}

void showvalue(int i,int j){
    float voltage=abs(3.3*i/1024.0);
      display.fillRect(36,0,24,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(36,0);
    display.print(voltage); 
    voltage=abs(3.3*j/1024.0);
    display.fillRect(36,8,24,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(36,8);
    display.print(voltage);     
    //display.print("V");
}

void showbar(int i,int j){
  if(modeflag==1){
    display.clearDisplay();
    modeflag=0;
  }
int xval,yval;
  xval=i*96/512;
  yval=j*96/512;
 display.fillRect(0,0,oldxval,8,BLACK);
 display.fillRect(0,8,oldyval,8,BLACK);
 // display.clearDisplay();
  display.fillRect(0,0,xval,8,WHITE);
  display.fillRect(0,8,yval,8,WHITE);
  display.display();
  oldxval=xval;
  oldyval=yval;
  
}



void mode_setting(){
  int sw,k,pt;
  ct=millis();
  sw=digitalRead(2);
if(sw==LOW && (ct-pt)>50){
modeflag=1;
mode=mode+1;
}
  if(mode>2){
    mode=0;
  }
}





