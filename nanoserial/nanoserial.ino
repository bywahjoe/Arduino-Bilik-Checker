#include <SoftwareSerial.h>
SoftwareSerial nanoserial(2, 3);

#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"

#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
String oke = "Capacity";

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}

void setup(void)
{ //Serial
  nanoserial.begin(9600);
  Serial.begin(9600);
  //DMD
  Timer1.initialize( 3000 );
  Timer1.attachInterrupt( ScanDMD );

  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen(true);

}
void loop(void)
{ String recv;
  if (nanoserial.available() > 0) {
    delay(100);
    while (nanoserial.available() > 0) {

      //recv = nanoserial.readStringUntil("\n");
      recv += char(nanoserial.read());
      delay(1);
      recv.trim();
    }
    if (recv.length() > 0) {

      //Serial.println(recv);
      handleSerial(recv);
      //delay(500);
      recv = "";

    }
  }
  //  int len=strlen(oke);
  //dmd.drawString( 0,0, oke.c_str(), 8, GRAPHICS_NORMAL );
  //dmd.drawString( 0, 0, "LIMIT :", 7, GRAPHICS_NORMAL );
  //dmd.drawString( 0, 8, "TEMP:", 5, GRAPHICS_NORMAL );
}
void handleSerial(String recv) {
  int maxData = 2, index = 0;
  String dataSerial[2];

  for (int i = 0; i < recv.length(); i++) {
    if (recv[i] == ',') {
      index++;
      continue;
    }
    if (index >= maxData) break;
    dataSerial[index] += recv[i];
  }

  dmd.clearScreen(true);
  //  //40
  dmd.drawString( 0, 0, dataSerial[0].c_str(), dataSerial[0].length(), GRAPHICS_NORMAL );
  dmd.drawString( 0, 8, dataSerial[1].c_str(), dataSerial[1].length(), GRAPHICS_NORMAL );
  //
}
