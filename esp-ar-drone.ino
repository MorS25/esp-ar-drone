#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <WiFiUdp.h>
#include "mask.h"

#define DEBUGWIFI 0
#define DEBUG 1

#if DEBUGWIFI == 1
  const char* ssid = "yourssid";
  const char* ssidpass = "yourpassword";
  const IPAddress drone(192, 168, 5, 110);
#else
  char* ssid = "";
  const IPAddress drone(192, 168, 1, 1);
#endif  
int droneSSID = 0;
  
const int navPort = 5554;
const int atPort = 5556;

const int floats[401] = {
0x00000000,0x3b23d70a,0x3ba3d70a,0x3bf5c28f,0x3c23d70a,0x3c4ccccc,0x3c75c28f,0x3c8f5c29,
0x3ca3d70a,0x3cb851eb,0x3ccccccc,0x3ce147ae,0x3cf5c28f,0x3d051eb8,0x3d0f5c29,0x3d199999,
0x3d23d70a,0x3d2e147b,0x3d3851eb,0x3d428f5c,0x3d4ccccc,0x3d570a3d,0x3d6147ae,0x3d6b851e,
0x3d75c28f,0x3d800000,0x3d851eb8,0x3d8a3d70,0x3d8f5c29,0x3d947ae1,0x3d999999,0x3d9eb852,
0x3da3d70a,0x3da8f5c2,0x3dae147b,0x3db33333,0x3db851eb,0x3dbd70a4,0x3dc28f5c,0x3dc7ae14,
0x3dcccccc,0x3dd1eb85,0x3dd70a3d,0x3ddc28f5,0x3de147ae,0x3de66666,0x3deb851e,0x3df0a3d7,
0x3df5c28f,0x3dfae147,0x3e000000,0x3e028f5c,0x3e051eb8,0x3e07ae14,0x3e0a3d70,0x3e0ccccd,
0x3e0f5c29,0x3e11eb85,0x3e147ae1,0x3e170a3d,0x3e199999,0x3e1c28f6,0x3e1eb852,0x3e2147ae,
0x3e23d70a,0x3e266666,0x3e28f5c2,0x3e2b851e,0x3e2e147b,0x3e30a3d7,0x3e333333,0x3e35c28f,
0x3e3851eb,0x3e3ae147,0x3e3d70a4,0x3e400000,0x3e428f5c,0x3e451eb8,0x3e47ae14,0x3e4a3d70,
0x3e4ccccc,0x3e4f5c29,0x3e51eb85,0x3e547ae1,0x3e570a3d,0x3e599999,0x3e5c28f5,0x3e5eb852,
0x3e6147ae,0x3e63d70a,0x3e666666,0x3e68f5c2,0x3e6b851e,0x3e6e147b,0x3e70a3d7,0x3e733333,
0x3e75c28f,0x3e7851eb,0x3e7ae147,0x3e7d70a3,0x3e800000,0x3e8147ae,0x3e828f5c,0x3e83d70a,
0x3e851eb8,0x3e866666,0x3e87ae14,0x3e88f5c2,0x3e8a3d70,0x3e8b851f,0x3e8ccccd,0x3e8e147b,
0x3e8f5c29,0x3e90a3d7,0x3e91eb85,0x3e933333,0x3e947ae1,0x3e95c28f,0x3e970a3d,0x3e9851eb,
0x3e999999,0x3e9ae147,0x3e9c28f6,0x3e9d70a4,0x3e9eb852,0x3ea00000,0x3ea147ae,0x3ea28f5c,
0x3ea3d70a,0x3ea51eb8,0x3ea66666,0x3ea7ae14,0x3ea8f5c2,0x3eaa3d70,0x3eab851e,0x3eaccccd,
0x3eae147b,0x3eaf5c29,0x3eb0a3d7,0x3eb1eb85,0x3eb33333,0x3eb47ae1,0x3eb5c28f,0x3eb70a3d,
0x3eb851eb,0x3eb99999,0x3ebae147,0x3ebc28f5,0x3ebd70a4,0x3ebeb852,0x3ec00000,0x3ec147ae,
0x3ec28f5c,0x3ec3d70a,0x3ec51eb8,0x3ec66666,0x3ec7ae14,0x3ec8f5c2,0x3eca3d70,0x3ecb851e,
0x3ecccccc,0x3ece147b,0x3ecf5c29,0x3ed0a3d7,0x3ed1eb85,0x3ed33333,0x3ed47ae1,0x3ed5c28f,
0x3ed70a3d,0x3ed851eb,0x3ed99999,0x3edae147,0x3edc28f5,0x3edd70a4,0x3edeb852,0x3ee00000,
0x3ee147ae,0x3ee28f5c,0x3ee3d70a,0x3ee51eb8,0x3ee66666,0x3ee7ae14,0x3ee8f5c2,0x3eea3d70,
0x3eeb851e,0x3eeccccc,0x3eee147b,0x3eef5c29,0x3ef0a3d7,0x3ef1eb85,0x3ef33333,0x3ef47ae1,
0x3ef5c28f,0x3ef70a3d,0x3ef851eb,0x3ef99999,0x3efae147,0x3efc28f5,0x3efd70a3,0x3efeb852,
0x3f000000,0x3f00a3d7,0x3f0147ae,0x3f01eb85,0x3f028f5c,0x3f033333,0x3f03d70a,0x3f047ae1,
0x3f051eb8,0x3f05c28f,0x3f066666,0x3f070a3d,0x3f07ae14,0x3f0851eb,0x3f08f5c2,0x3f099999,
0x3f0a3d70,0x3f0ae147,0x3f0b851f,0x3f0c28f6,0x3f0ccccd,0x3f0d70a4,0x3f0e147b,0x3f0eb852,
0x3f0f5c29,0x3f100000,0x3f10a3d7,0x3f1147ae,0x3f11eb85,0x3f128f5c,0x3f133333,0x3f13d70a,
0x3f147ae1,0x3f151eb8,0x3f15c28f,0x3f166666,0x3f170a3d,0x3f17ae14,0x3f1851eb,0x3f18f5c2,
0x3f199999,0x3f1a3d70,0x3f1ae147,0x3f1b851e,0x3f1c28f6,0x3f1ccccd,0x3f1d70a4,0x3f1e147b,
0x3f1eb852,0x3f1f5c29,0x3f200000,0x3f20a3d7,0x3f2147ae,0x3f21eb85,0x3f228f5c,0x3f233333,
0x3f23d70a,0x3f247ae1,0x3f251eb8,0x3f25c28f,0x3f266666,0x3f270a3d,0x3f27ae14,0x3f2851eb,
0x3f28f5c2,0x3f299999,0x3f2a3d70,0x3f2ae147,0x3f2b851e,0x3f2c28f6,0x3f2ccccd,0x3f2d70a4,
0x3f2e147b,0x3f2eb852,0x3f2f5c29,0x3f300000,0x3f30a3d7,0x3f3147ae,0x3f31eb85,0x3f328f5c,
0x3f333333,0x3f33d70a,0x3f347ae1,0x3f351eb8,0x3f35c28f,0x3f366666,0x3f370a3d,0x3f37ae14,
0x3f3851eb,0x3f38f5c2,0x3f399999,0x3f3a3d70,0x3f3ae147,0x3f3b851e,0x3f3c28f5,0x3f3ccccd,
0x3f3d70a4,0x3f3e147b,0x3f3eb852,0x3f3f5c29,0x3f400000,0x3f40a3d7,0x3f4147ae,0x3f41eb85,
0x3f428f5c,0x3f433333,0x3f43d70a,0x3f447ae1,0x3f451eb8,0x3f45c28f,0x3f466666,0x3f470a3d,
0x3f47ae14,0x3f4851eb,0x3f48f5c2,0x3f499999,0x3f4a3d70,0x3f4ae147,0x3f4b851e,0x3f4c28f5,
0x3f4ccccc,0x3f4d70a4,0x3f4e147b,0x3f4eb852,0x3f4f5c29,0x3f500000,0x3f50a3d7,0x3f5147ae,
0x3f51eb85,0x3f528f5c,0x3f533333,0x3f53d70a,0x3f547ae1,0x3f551eb8,0x3f55c28f,0x3f566666,
0x3f570a3d,0x3f57ae14,0x3f5851eb,0x3f58f5c2,0x3f599999,0x3f5a3d70,0x3f5ae147,0x3f5b851e,
0x3f5c28f5,0x3f5ccccc,0x3f5d70a4,0x3f5e147b,0x3f5eb852,0x3f5f5c29,0x3f600000,0x3f60a3d7,
0x3f6147ae,0x3f61eb85,0x3f628f5c,0x3f633333,0x3f63d70a,0x3f647ae1,0x3f651eb8,0x3f65c28f,
0x3f666666,0x3f670a3d,0x3f67ae14,0x3f6851eb,0x3f68f5c2,0x3f699999,0x3f6a3d70,0x3f6ae147,
0x3f6b851e,0x3f6c28f5,0x3f6ccccc,0x3f6d70a3,0x3f6e147b,0x3f6eb852,0x3f6f5c29,0x3f700000,
0x3f70a3d7,0x3f7147ae,0x3f71eb85,0x3f728f5c,0x3f733333,0x3f73d70a,0x3f747ae1,0x3f751eb8,
0x3f75c28f,0x3f766666,0x3f770a3d,0x3f77ae14,0x3f7851eb,0x3f78f5c2,0x3f799999,0x3f7a3d70,
0x3f7ae147,0x3f7b851e,0x3f7c28f5,0x3f7ccccc,0x3f7d70a3,0x3f7e147b,0x3f7eb852,0x3f7f5c29,
0x3f800000
};

#define MAX_NUM_ARGS 7

struct ATCommand {
  String type;
  String cmdargs[MAX_NUM_ARGS];
  boolean blocking; // currently unused
};

// ESP status
unsigned int ESP_status = 0;

// UDP
unsigned int sequence;          // Current client-side sequence number
unsigned int lastNav;           // Last sequence number received from AR Drone
unsigned long lastSend;          // millis since last command packet sent
unsigned long lastReceive;      // millis since last nav packet received
WiFiUDP NavUdp;
WiFiUDP AT;
String sendBuffer;
String inputBuffer;
char incoming[1024];
uint32_t *header;
uint32_t *state;
uint32_t *navSequence;
uint32_t *vision;
uint16_t *id;
uint16_t *siz;
uint32_t *ctrl_state;
uint32_t *battery;

void commander();
void debugcommander();
ATCommand rawCommand(String type, int cmdargs[], boolean blocking);
ATCommand refCommand(boolean takeoff, boolean emergency);
ATCommand pcmdCommand(boolean enable, boolean tilt, int measures[]);
ATCommand ftrimCommand();
ATCommand configCommand(String key, String value);
ATCommand calibCommand(int devicenum);
ATCommand comwdgCommand();
String serialize(ATCommand cmd);
void sendPacket(ATCommand cmd);
void connectToAR();
void setBit(unsigned int& status_var, unsigned int mask);
void clearBit(unsigned int& status_var, unsigned int mask);
void toggleBit(unsigned int& status_var, unsigned int mask);

ATCommand loopCommands[2];
int m[] = {0,0};
char inBuffer[20] = {0};
char *tokenBuffer[4];
int ESPState = MODE_SETUP;

void setup() {
  Serial.begin(115200);

  // Inits
  header = (uint32_t*)&incoming[0];
  state = (uint32_t*)&incoming[4];
  navSequence = (uint32_t*)&incoming[8];
  vision = (uint32_t*)&incoming[12];
  id = (uint16_t*)&incoming[16];
  siz = (uint16_t*)&incoming[18];
  ctrl_state = (uint32_t*)&incoming[20];
  battery = (uint32_t*)&incoming[24];

  lastSend = millis();
}

void loop() {
   
  debugcommander();

  if (WiFi.status() == WL_CONNECTED) {
    if(NavUdp.parsePacket()) {
      lastReceive = millis();
      int len = NavUdp.read(incoming, 1024);
      if (len <  30) return;
      incoming[len] = 0;
      if(lastNav > *navSequence || *header != 0x55667788 || *id != 0 || *siz != 148) return;  // if bad packet, skip everything
  
      lastNav = *navSequence;
      if (*state & batLow_mask) {
        setBit(ESP_status, ESP_LOWBAT);
      } else {
        clearBit(ESP_status, ESP_LOWBAT);
      }
      if (*state & fly_mask) {
        setBit(ESP_status, ESP_FLYING);
      } else {
        clearBit(ESP_status, ESP_FLYING);
      }
  
      if(((*state & emergency_mask) != 0 ||(*state & userEmer_mask) != 0) && !(ESP_status & ESP_EMERGENCY)){
        setBit(ESP_status, ESP_EMERGENCY);
      }
      if(((*state & emergency_mask) == 0 ||(*state & userEmer_mask) == 0) && (ESP_status & ESP_EMERGENCY)) {
        clearBit(ESP_status, ESP_EMERGENCY);
      }
    }
    
    if (millis() - lastSend > 30) {
      sendPacket(loopCommands[0]);
      sendPacket(loopCommands[1]);
      lastSend = millis();
    }
  }
  
  if (ESPState == MODE_SETUP && ssid != "") {
    connectToAR();
  }
}

void debugcommander() {
  if (Serial.available()) {
    char inChar = Serial.read();
    if (inChar != '\n') {
      strncat(inBuffer, &inChar, 1);
    } else { // input complete
      char *token = strtok(inBuffer, ",");
      Serial.println(token);
      int i = 0;
      while( token != NULL )
      {
        tokenBuffer[i] = token;
        i++;
        token = strtok(NULL, ",");
        Serial.println(token);
      }

      m[0] = atoi(tokenBuffer[2]);
      if (m[0] < -400) {
        m[0] = -400;
      } else if (m[0] > 400) {
        m[0] = 400;
      }
      Serial.println(m[0]);
      m[1] = -(atoi(tokenBuffer[3]));
      if (m[1] < -400) {
        m[1] = -400;
      } else if (m[1] > 400) {
        m[1] = 400;
      }
      Serial.println(m[1]);

      if (!strcmp(tokenBuffer[0],BUTTON_CENTER) && !(ESP_status & ESP_FLYING)) {
        sendPacket(ftrimCommand());
      } else if (!strcmp(tokenBuffer[0], BUTTON_START)) {
        if (!(ESP_status & ESP_FLYING) && !(ESP_status & ESP_EMERGENCY)) {
          loopCommands[0] = refCommand(true, false);
        } else if ((ESP_status & ESP_FLYING) || (ESP_status & ESP_EMERGENCY)) {
          loopCommands[0] = refCommand(false, false);
        }
      } else if (!strcmp(tokenBuffer[0],BUTTON_SELECT)) {
        loopCommands[0] = refCommand(false, true);
      } else if (!strcmp(tokenBuffer[0],"0")) {
        ssid = "ardrone2_116276";
        ESPState = MODE_SETUP;
      } else if (!strcmp(tokenBuffer[0],"1")) {
        ssid = "Drone";
        ESPState = MODE_SETUP;
      }

      if (!strcmp(tokenBuffer[1],POSTURE_RELAXED)) {
        memset(m, NULL, 2*sizeof(int));
        loopCommands[1] = pcmdCommand(false, false, m);
      } else if (!strcmp(tokenBuffer[1],POSTURE_FIST)) {
        loopCommands[1] = pcmdCommand(true, true, m);
      } else if (!strcmp(tokenBuffer[1],POSTURE_POINT) && (ESP_status & ESP_FLYING)) {
        loopCommands[1] = pcmdCommand(true, false, m);
      }

      memset(inBuffer, 0, 20);
    }
  }
}

ATCommand rawCommand(String type, int numargs, String *cmdargs, boolean blocking = false) {
  ATCommand newCommand;
  newCommand.type = type;
  for (int i = 0; i < MAX_NUM_ARGS; i++) {
    if (i < numargs) {
      newCommand.cmdargs[i] = cmdargs[i];
    } else {
      newCommand.cmdargs[i] = "";
    }
  }
  newCommand.blocking = blocking;

  return newCommand;
}

ATCommand refCommand(boolean takeoff, boolean emergency) {
  String cmdargs[1];
  if (emergency) {
    cmdargs[0] = "290717952";
  } else {
    if (takeoff) {
      cmdargs[0] = "290718208";
    } else {
      cmdargs[0] = "290717696";
    }
  }
  return rawCommand("REF", 1, cmdargs);
}

ATCommand pcmdCommand(boolean enable, boolean tilt, int *measures) {
  String cmdargs[5] = {"","0","0","0","0"};
  cmdargs[0] = enable ? "1" : "0"; // Enables or diables (hover only) movement
  int offset = tilt ? 1 : 3;  // if we're just pitching and rolling, use args 1 and 2, else 3 and 4
  if (offset == 1) {
    for (int i = 0; i < 2; i++) {
      if (measures[i] < 0) {
        cmdargs[i+offset] = String((signed int)floats[-measures[i]] | 0x80000000); // Use float lookup table and negate in 2's comp
      } else {
        cmdargs[i+offset] = String((signed int)floats[measures[i]]);
      }
    }
  } else {
    if (measures[0] < 0) {
      cmdargs[4] = String((signed int)floats[-measures[0]] | 0x80000000); // Use float lookup table and negate in 2's comp
    } else {
      cmdargs[4] = String((signed int)floats[measures[0]]);
    }
    if (measures[1] < 0) {
      cmdargs[3] = String((signed int)floats[-measures[1]] | 0x80000000); // Use float lookup table and negate in 2's comp
    } else {
      cmdargs[3] = String((signed int)floats[measures[1]]);
    }
  }


  return rawCommand("PCMD", 5, cmdargs);
}

ATCommand ftrimCommand() {
  String cmdargs[] = {};
  return rawCommand("FTRIM", 0, cmdargs);
}

ATCommand configCommand(String key, String value) {
  String cmdargs[2];
  cmdargs[0] = "\"" + key + "\"";
  cmdargs[1] = "\"" + value + "\"";

  return rawCommand("CONFIG", 2, cmdargs, true);
}

ATCommand calibCommand(int devicenum) {
  String cmdargs[] = {String(devicenum)};
  return rawCommand("CALIB", 1, cmdargs);
}

ATCommand comwdgCommand() {
  String cmdargs[] = {};
  return rawCommand("COMWDG", 0, cmdargs);
}

String serialize(ATCommand cmd) {
  String serialized = "AT*" + cmd.type + "=" + String(sequence);
  for (int i = 0; i < MAX_NUM_ARGS; i++) {
    if (cmd.cmdargs[i] != "") {
      serialized += "," + cmd.cmdargs[i];
    }
  }

  sequence++; // packet sequence # incremented for each new command
  return serialized;
}

void sendPacket(ATCommand cmd) {
  String serializedCommand = serialize(cmd);
  char sendChar[serializedCommand.length()+1];
  serializedCommand.toCharArray(sendChar, serializedCommand.length()+1);
  sendChar[serializedCommand.length()] = '\r';
  AT.beginPacket(drone, atPort);
  AT.write(sendChar);
  AT.endPacket();
  lastSend = millis();
}

void connectToAR() {
  // Disconnect from WiFi if connected
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Disconnecting from current WiFi");
    WiFi.disconnect();
    while (WiFi.status() == WL_CONNECTED) {
      delay(200);
    }
  }
  
  // Connect to WiFi network
  Serial.println("connecting to AR WiFi");
  WiFi.mode(WIFI_STA);
#if DEBUGWIFI == 1
  WiFi.begin(ssid, ssidpass);
#else
  WiFi.begin(ssid);
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  Serial.print("Connected, address=");
  Serial.println(WiFi.localIP()); 
  setBit(ESP_status, ESP_CONNECTED);

  // Prep UDP
  Serial.println("Starting UDP...");
  NavUdp.begin(navPort); //Open port for navdata
  NavUdp.flush();
  AT.begin(atPort);
  AT.flush();

  lastNav = 0;
  sequence = 1;
  
  ATCommand setupCommands[2];
  setupCommands[0] = configCommand("general:navdata_demo", "TRUE");
  setupCommands[1] = configCommand("control:altitude_max", "2000");
  while(NavUdp.parsePacket() == 0) {
    delay(10);
    NavUdp.beginPacket(drone, navPort);
    NavUdp.write(0x01);
    NavUdp.endPacket();
    delay(20);
    sendPacket(setupCommands[0]);
    sendPacket(setupCommands[1]);
  }

  // Set emergency bit high for 1 second
  setupCommands[0] = refCommand(false, true);
  unsigned long emergencyInitCounter = millis();
  while (millis() - emergencyInitCounter < 1000) {
    sendPacket(setupCommands[0]);
    delay(30);
  }

  // Starting main loop
  loopCommands[0] = refCommand(false, false);     // Ensure AR is landed...
  loopCommands[1] = pcmdCommand(false, false, m);  // And not trying to move
  ESPState = MODE_NORMAL;
}

void setBit(unsigned int& status_var, unsigned int mask) {
  status_var |= mask;
}

void clearBit(unsigned int& status_var, unsigned int mask) {
  status_var &= ~mask;
}

void toggleBit(unsigned int& status_var, unsigned int mask) {
  status_var ^= mask;
}

