#include <ResetDetector.h>

#define RD_TIMEOUT 5
#define RD_ADDRESS 0

ResetDetector rd(RD_TIMEOUT,RD_ADDRESS);
bool msgPrinted=false;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("------------------------------------------");
  Serial.println("Reset number: "+String(rd.detectResetCount())); 
  Serial.println("------------------------------------------");

}


void loop()
{
    rd.waitForDetect();

    if (!rd.isWaitingForNextReset() && !msgPrinted){
       Serial.println("------------------------------------------");
       Serial.println("Timeout has expired"); 
       Serial.println("------------------------------------------");   
       msgPrinted=true;
    }
}