#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  mySerial.begin(57600);

  if (finger.begin()) {
    Serial.println("Leitor biométrico inicializado com sucesso!");
  } else {
    Serial.println("Erro ao inicializar o leitor biométrico.");
    while (1);
  }

  if (finger.verifyPassword()) {
    Serial.println("Senha do leitor biométrico confirmada.");
  } else {
    Serial.println("Senha do leitor biométrico incorreta.");
    while (1);
  }
}

void loop() {
  Serial.println("Aguardando digital...");
  int id = getFingerprintID();
  if (id != -1) {
    Serial.print("REGISTERED_ID:");
    Serial.println(id);
  }
  delay(2000);
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}
