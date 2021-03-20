#include <SoftwareSerial.h>
SoftwareSerial nanoserial(2, 3);//rx tx

#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>

#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
String oke = "Capacity";

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}

void setup()
{ //Serial
  nanoserial.begin(4800);
  Serial.begin(4800);
  
  //DMD
  Timer1.initialize( 3000 );
  Timer1.attachInterrupt( ScanDMD );
  
  //dmd.selectFont(SystemFont5x7);
  dmd.selectFont(Arial_Black_16);
  dmd.clearScreen(true);
  dmd.drawString( 6, 0, "MATIC", 5, GRAPHICS_NORMAL );
 
}
void loop()
{ String recv;
  while (nanoserial.available() > 0) {

    //recv = nanoserial.readStringUntil("\n");
    recv += char(nanoserial.read());
    delay(1);
    recv.trim();
  }
  if (recv.length() > 0) {
    Serial.println(recv);

    int dataku = recv.length();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString( 5, 0, "KAPASITAS", 9, GRAPHICS_NORMAL );

    if (recv == "0") {
      dmd.clearScreen(true);
      dmd.drawString( 5, 0, "KAPASITAS", 9, GRAPHICS_NORMAL );
      dmd.drawString( 18, 9, "PENUH", 5, GRAPHICS_NORMAL );
    } else {
      dmd.clearScreen(true);
      dmd.drawString( 5, 0, "KAPASITAS", 9, GRAPHICS_NORMAL );
      dmd.drawString( 29, 9, recv.c_str(), dataku, GRAPHICS_NORMAL );
    }
    recv = "";
  }
}

void handleSerial(String recv) {
  int maxData = 2, index = 0;
  String dataSerial[2];

  for (int i = 1; i < recv.length(); i++) {
    if (recv[i] == ',') {
      index++;
      continue;
    }
    if (index >= maxData) break;
    dataSerial[index] += recv[i];
  }

  dmd.clearScreen(true);
  //40
  dmd.selectFont(SystemFont5x7);
  dmd.drawString( 0, 0, "LIMIT :", 7, GRAPHICS_NORMAL );

  dmd.drawString( 40, 0, dataSerial[0].c_str(), dataSerial[0].length(), GRAPHICS_NORMAL );
  dmd.drawString( 40, 8, dataSerial[1].c_str(), dataSerial[1].length(), GRAPHICS_NORMAL );
}
