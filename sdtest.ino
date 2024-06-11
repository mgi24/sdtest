//SUPER SIMPLE SD TEST BY MMV PROJECT
//free to distribute but DO NOT REMOVE THE LINE 1 CREDIT!!!

#include "SD_MMC.h" //install esp32 library 1st, otherwise install manually!
#include "FS.h"

//Pin connection
//MICROSD (8PIN)
/*NC = Not Connected 
 *    MICROSD(8PIN)(mode 1 bit SDIO)
 *    SD      ESP32-S3
 *
 *    D2       NC
 *    D3       NC
 *    CMD ---  38 ------------------------|
 *    VDD ---  3.3V-----10K ohm resistor--| (connect all pin to pullup resistor)
 *    CLK ---  39 ------------------------|
 *    VSS ---  GND                        |
 *    D0  ---  40 ------------------------|
 *    D1  
 *    
 *    same with sdcard, just add 1 more GND
 */

 //feel free to use other pin, don't be scared! just make sure the pin is available for gpio!

#define SDMMC_CLK   39
#define SDMMC_CMD   38
#define SDMMC_D0    40  

void speedtestio(fs::FS &fs, const char * path){
    File file;
    static uint8_t buf[512];
    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    long start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);//write buffer, 512 is space allocated for that!
        //for more efficient space, plase use ACTUAL buffer size!
    }
    long end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();//dont forget to close file after use
}
 










void setup() {
  Serial.begin(115200);//mulai serial monitor
  SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0);//set custom pin


  if(!SD_MMC.begin("/sdcard", true, false, 20000, 5)){//mount name, run on 1bit?, format if failed?, freq, max file open at once
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  
  //testing card type
  if(cardType == CARD_NONE){
      Serial.println("No SD_MMC card attached");
      return;
  }

  Serial.print("SD_MMC Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }

  //testing card size
  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  //testing available space
  Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

  //test open file
  String filename = "/test.txt";
  SD_MMC.remove(filename);//delete previous file
  File file;
  file = SD_MMC.open(filename, FILE_WRITE);
  if(!file){
    Serial.println("FAILED TO OPEN FILE");
  }
  else{
    Serial.println("file opened succesfully!");
  }
  file.close();//dont forget this

  //speedtest
  speedtestio(SD_MMC, "/test.txt");//code on top!

}

void loop() {
  // put your main code here, to run repeatedly:

}
