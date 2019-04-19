#include <SPI.h>
#include <Wire.h>

#include "globals.h"
#include "task.h"
#include "flash.h"

void AdsSetup(Adafruit_ADS1115* ads) {
  ads->setGain(GAIN_TWOTHIRDS);
  ads->begin();
}


Flasher f1(50, 5000);
Flasher f2(200, 3000);

Task* all_tasks[] = {&f1, &f2};


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWR_CONTROL, OUTPUT);
  pinMode(LED_CONTROL, OUTPUT);
  digitalWrite(LED_CONTROL, LOW);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED)) {
    Serial.println("SSD1306 allocation failed");
    for (;;); // Don't proceed, loop forever
  }

  AdsSetup(&ads0);
  AdsSetup(&ads1);

  // Start-up success beep.
  tone(BUZZER_PIN, 440 * 2, 100);
}

// Voltage divider values for the cell sensors.
uint16_t rtop[8] = {570, 1, 1, 1, 1, 1, 1, 1};
uint16_t rbot[8] = {100, 1, 1, 1, 1, 1, 1, 1};

// Reading calibration values, determined by linear interpolation over sampled data.
int16_t cal_l[8] = {69, 0, 0, 0, 0, 0, 0, 0};
int16_t cal_h[8] = {-526, 0, 0, 0, 0, 0, 0, 0};

uint16_t channels_mv[8] = {0};

void readAds(Adafruit_ADS1115* ads, int start, int len, int pos) {
  for (int i = start; i < len; ++i) {
    long val = ads->readADC_SingleEnded(i);
    if (val < 0) {
      val = 0;
    }
    val *= rtop[pos];
    val /= rbot[pos];
    
    // TODO voltage divider compensate
    uint16_t mv = map(val, 0 + cal_l[pos], 32768 + cal_h[pos], 0, 6144);
    channels_mv[pos++] = mv;
  }
}

void printAds() {
  char buf[32];
  char mvb[10];

  for (int i = 0; i < 8; ++i) {
    dtostrf(float(channels_mv[i]) / 1000, 5, 3, mvb);
    sprintf(buf, "C%d: %s", i + 1, mvb);

    if (i % 2 == 0) {
      display.print(buf);
    } else {
      display.setCursor(display.width() / 2, display.getCursorY());
      display.println(buf);
    }
  }
}

void drawScreen() {
  // TODO fix.

  char ts[12] = {0};
  double alive = double(millis()) / 1000;
  dtostrf(alive, 0, 2, ts);

  char buf[32] = {0};
  sprintf(buf, "%s sec", ts);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Alive");
  display.println(buf);


  display.setTextSize(1);

  printAds();



  display.display();
}

bool once = false;

void loop() {

  //  if (millis() < 10000) {
  //    drawScreen();
  //  } else {
  //    if (!once) {
  //      once = true;
  //      display.clearDisplay();
  //      display.display();
  //
  //      digitalWrite(LED_CONTROL, HIGH);  // screen power off!
  //    }
  //  }

  // TODO

  readAds(&ads0, 0, 4, 0);
  readAds(&ads1, 0, 4, 4);


  drawScreen();

  // TODO, does powering off the screen interfere with i2c?

  // TODO heartbeat?
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(25);
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(20);


  unsigned long now = millis();


  if (now > 300000 || channels_mv[0] < 8000) {
    for (int i = 0; i < 5; ++i) {
      tone(BUZZER_PIN, 440 * 2, 300);
      delay(500);
    }
    digitalWrite(PWR_CONTROL, HIGH);
    for (;;);
  }


  for (int i = 0; i < sizeof(all_tasks) / sizeof(Task*); ++i) {
    all_tasks[i]->Tick(now);
  }
}
