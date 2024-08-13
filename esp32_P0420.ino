#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
#define ELM_PORT SerialBT
#define DEBUG_PORT Serial

esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE;  // or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE to request pincode confirmation
esp_spp_role_t role = ESP_SPP_ROLE_SLAVE;   // or ESP_SPP_ROLE_MASTER

uint8_t Address[6] = { 0x00, 0x10, 0xCC, 0x4F, 0x36, 0x03 };

int HexToDec(char* n);

void setup() {

  delay(2000);

  DEBUG_PORT.begin(115200);
  // SerialBT.setPin("1234");
  ELM_PORT.begin("ESP32", true);

  while(!ELM_PORT.connect(Address, 0, sec_mask, role)) {
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
  }
  DEBUG_PORT.println("Connected to OBDII");
}

void loop() {

  String buff = "";
  delay(4000);
  ELM_PORT.flush();
  Serial.println("tx: 03");
  ELM_PORT.println("03");

  if (ELM_PORT.available()) {

    Serial.print("rx: ");
    while (ELM_PORT.available()) {
      int c = ELM_PORT.read();
      if (c >= 0) {        
        buff += (char)c;     
      }
    }
    Serial.println(buff);
  }

  buff.replace(" ", NULL);
  buff.replace("\r", NULL);
  buff.replace("\n", NULL);
  buff.replace("\t", NULL);
  //010541055A

  Serial.println(buff.c_str()); 
  
  if (buff.indexOf("0420") >= 0) { //count + '\0'
    Serial.println("here");
    delay(1000);
    ELM_PORT.println("04");

    delay(2000);
  }
  buff = "";
}
