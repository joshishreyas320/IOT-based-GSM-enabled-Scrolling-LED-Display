#include <MD_MAX72xx.h>
#include <SoftwareSerial.h>

// ── MAX7219 Pins ──
#define MAX_DEVICES  8
#define CLK_PIN      13
#define DATA_PIN     11
#define CS_PIN       10

// ── GSM SIM900D Pins (Proteus COMPIM) ──
#define GSM_RX_PIN   7   // Arduino 7  → SIM900D TX
#define GSM_TX_PIN   8   // Arduino 8  → SIM900D RX

SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);
MD_MAX72XX mx = MD_MAX72XX(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// ── Scroll Settings ──
#define SCROLL_DELAY   150
#define CHAR_SPACING   1
#define BUF_SIZE       100

char curMessage[BUF_SIZE] = "Waiting for SMS...";
char newMessage[BUF_SIZE];
bool newMsgAvailable = false;

// ─────────────────────────────────────────
// GSM Init - AT Commands पाठवणे
// ─────────────────────────────────────────
void gsmInit() {
  delay(2000);
  gsmSerial.println("AT");           // Check communication
  delay(500);
  gsmSerial.println("AT+CMGF=1");   // Text mode set करा
  delay(500);
  gsmSerial.println("AT+CNMI=1,2,0,0,0"); // SMS आल्यावर auto serial वर पाठव
  delay(500);
}

// ─────────────────────────────────────────
// SMS Parse करणे - "+CMT:" नंतर message येतो
// ─────────────────────────────────────────
void parseGSM() {
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("GSM: " + response);  // Debug - Serial Monitor मध्ये बघ

    // "+CMT:" ओळ आली म्हणजे नवीन SMS आला
    if (response.indexOf("+CMT:") != -1) {
      // +CMT: नंतर दुसऱ्या ओळीत actual message असतो
      int msgStart = response.lastIndexOf('\n');
      if (msgStart != -1) {
        String smsText = response.substring(msgStart + 1);
        smsText.trim();  // spaces/newlines काढा

        if (smsText.length() > 0) {
          smsText.toCharArray(newMessage, BUF_SIZE);
          newMsgAvailable = true;
        }
      }
    }
  }
}

// ─────────────────────────────────────────
// Scroll Engine (तुझा original code)
// ─────────────────────────────────────────
uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t) {
  static char    *p = curMessage;
  static uint8_t  state = 0;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  switch (state) {
    case 0:
      showLen = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state++;
      if (*p == '\0') p = curMessage;

    case 1:
      colData = cBuf[curLen++];
      if (curLen == showLen) {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = 2;
      }
      break;

    case 2:
      colData = 0;
      curLen++;
      if (curLen == showLen) state = 0;
      break;

    default:
      state = 0;
  }
  return (colData);
}

void scrollText() {
  static uint32_t prevTime = 0;
  if (millis() - prevTime >= SCROLL_DELAY) {
    mx.transform(MD_MAX72XX::TSR);
    prevTime = millis();
  }
}

// ─────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);  // SIM900D baud rate

  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.control(MD_MAX72XX::INTENSITY, 10);

  gsmInit();  // ← GSM Initialize
}

void loop() {
  // नवीन SMS आला का ते check कर
  parseGSM();

  // नवीन message असेल तर update कर
  if (newMsgAvailable) {
    strcpy(curMessage, newMessage);
    newMsgAvailable = false;
  }

  scrollText();
}