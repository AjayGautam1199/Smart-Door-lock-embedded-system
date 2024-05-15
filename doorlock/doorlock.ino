#include <Keypad.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9341 _panel_instance; //9341
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = 19;
      cfg.pin_dc = 2;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 5;
      cfg.pin_rst = 4;
      cfg.pin_busy = -1;
      cfg.panel_width = 240;
      cfg.panel_height = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    setPanel(&_panel_instance);
  }
};

static LGFX tft;
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

const byte rows = 4;
const byte cols = 4;

char key[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {13, 12, 14, 27};
byte colPins[cols] = {26, 25, 33, 32};

Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);

char* password = "4567";
int redled = 34;
int greenled = 36;
int buzz = 35;
int invalidcount = 12;

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);

  Serial.begin(9600);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(15, OUTPUT);
}

void loop() {
  if (currentposition == 0) {
    displayscreen();
  }
  int l;
  char code = keypad.getKey();
  if (code != NO_KEY) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.print("PASSWORD:");
    tft.setCursor(10, 120); // Adjusted cursor position
    tft.print(" ");
    tft.setCursor(10, 120); // Adjusted cursor position
    for (l = 0; l <= currentposition; ++l) {
      tft.print("*");
      keypress();
    }

    if (code == password[currentposition]) {
      ++currentposition;
      if (currentposition == 4) {
        unlockdoor();
        currentposition = 0;
      }
    } else {
      ++invalidcount;
      incorrect();
      currentposition = 0;
    }
    if (invalidcount == 5) {
      ++invalidcount;
      torture1();
    }
    if (invalidcount == 8) {
      torture2();
    }
  }
}

void unlockdoor() {
  delay(900);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println(" ");
  tft.setCursor(1, 0);
  tft.print("Access Granted");
  tft.setCursor(4, 10);
  tft.println("WELCOME!!");
  tft.setCursor(15, 6);
  tft.println(" ");
  tft.setCursor(16, 6);
  tft.println(" ");
  tft.setCursor(14, 6);
  tft.println(" ");
  tft.setCursor(13, 6);
  tft.println(" ");
  unlockbuzz();

  digitalWrite(15, HIGH);
  delay(2000);

  delay(1000);
  counterbeep();

  delay(1000);

  digitalWrite(15, HIGH);
  tft.fillScreen(TFT_BLACK);
  displayscreen();
}

void incorrect() {
  delay(500);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(1, 0);
  tft.print("CODE");
  tft.setCursor(6, 0);
  tft.print("INCORRECT");
  tft.setCursor(15, 4);
  tft.println(" ");
  tft.setCursor(4, 1);
  tft.println("GET AWAY!!!");

  tft.setCursor(13, 6);
  tft.println(" ");
  Serial.println("CODE INCORRECT YOU ARE UNAUTHORIZED");
  digitalWrite(redled, HIGH);
  digitalWrite(buzz, HIGH);
  delay(3000);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(redled, LOW);
  digitalWrite(buzz, LOW);
  displayscreen();
}

void clearscreen() {
  tft.setCursor(0, 0);
  tft.println(" ");
  tft.setCursor(0, 1);
  tft.println(" ");
  tft.setCursor(0, 2);
  tft.println(" ");
  tft.setCursor(0, 3);
  tft.println(" ");
}

void keypress() {
  digitalWrite(buzz, HIGH);
  delay(50);
  digitalWrite(buzz, LOW);
}

void displayscreen() {
  tft.setCursor(10, 80); // Adjusted cursor position
  tft.println("*ENTER THE CODE*");
  tft.setCursor(30, 120); // Adjusted cursor position
  tft.println("TO _/_ (OPEN)!!");
}

void unlockbuzz() {
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(200);
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(80);
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
  delay(80);
}

void counterbeep() {
  delay(1200);

  tft.fillScreen(TFT_BLACK);
  digitalWrite(buzz, HIGH);

  tft.setCursor(2, 15);
  tft.println(" ");
  tft.setCursor(2, 14);
  tft.println(" ");
  tft.setCursor(2, 0);
  delay(200);
  tft.println("GET IN WITHIN:::");

  tft.setCursor(4, 1);
  tft.print("5");
  delay(200);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);
  //2
  digitalWrite(buzz, HIGH);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  tft.setCursor(4, 1); //2
  tft.print("4");
  delay(100);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);
  //3
  digitalWrite(buzz, HIGH);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  tft.setCursor(4, 1); //3
  tft.print("3");
  delay(100);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);
  //4
  digitalWrite(buzz, HIGH);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  tft.setCursor(4, 1); //4
  tft.print("2");
  delay(100);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN:");
  digitalWrite(buzz, LOW);
  delay(1000);
  //
  digitalWrite(buzz, HIGH);
  tft.setCursor(4, 1);
  tft.print("1");
  delay(100);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.println("GET IN WITHIN::");
  digitalWrite(buzz, LOW);
  delay(1000);
  //5
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  delay(40);
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  delay(40);
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  delay(40);
  digitalWrite(buzz, HIGH);
  delay(40);
  digitalWrite(buzz, LOW);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.print("RE-LOCKING");
  delay(500);
  tft.setCursor(12, 0);
  tft.print(".");
  delay(500);
  tft.setCursor(13, 0);
  tft.print(".");
  delay(500);
  tft.setCursor(14, 0);
  tft.print(".");
  delay(400);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(4, 0);
  tft.print("LOCKED!");
  delay(440);
}

void torture1() {
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.print("WAIT FOR ");
  tft.setCursor(5, 1);
  tft.print("15 SECONDS");
  digitalWrite(buzz, HIGH);
  delay(15000);
  digitalWrite(buzz, LOW);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.print("LOL..");
  tft.setCursor(1, 1);
  tft.print(" HOW WAS THAT??");
  delay(3500);
  tft.fillScreen(TFT_BLACK);
}

void torture2() {
  delay(1000);
  tft.setCursor(1, 0);
  tft.print(" ");
  tft.setCursor(2, 0);
  tft.print("EAR DRUMS ARE");
  tft.setCursor(0, 1);
  tft.print(" PRECIOUS!! ");
  delay(1500);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(1, 0);
  tft.print(" WAIT FOR");
  tft.setCursor(4, 1);
  tft.print(" 1 MINUTE");
  digitalWrite(buzz, HIGH);
  delay(55000);
  counterbeep();
  tft.fillScreen(TFT_BLACK);
  digitalWrite(buzz, LOW);
  tft.setCursor(2, 0);
  tft.print("WANT ME TO");
  tft.setCursor(1, 1);
  tft.print("REDICULE MORE??");
  delay(2500);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(2, 0);
  tft.print("Ha Ha Ha Ha");
  delay(1700);
  tft.fillScreen(TFT_BLACK);
}
