
void setup() {

  Serial2.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  int a = random(1, 5);
  int b = random(30, 40);
  String oke = +","+String(a) + ',' + String(b) + ',';
  Serial2.write(oke.c_str());

  delay(4000);

}
