#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);  // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  if (finger.begin()) {
    Serial.println("Sensor biométrico iniciado.");
  } else {
    Serial.println("Sensor biométrico não encontrado.");
    while (1);
  }
}

void loop() {
  getFingerprintID();
  delay(1000);
}

void getFingerprintID() {
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao capturar digital.");
    return;
  }
  
  p = finger.image2Tz();
  
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao processar digital.");
    return;
  }
  
  p = finger.fingerSearch();
  
  if (p == FINGERPRINT_OK) {
    Serial.println("Digital reconhecida.");
    int id = finger.fingerID;
    sendAttendanceData(id);  // Enviar dados para o navegador
  } else {
    Serial.println("Digital não reconhecida.");
  }
}

void sendAttendanceData(int studentId) {
  // Enviar dados para o computador via Serial (que o HTML pode ler)
  Serial.print("ID: ");
  Serial.println(studentId);
}
