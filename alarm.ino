// beginning of the alarm code
int pressure = 0;

void setup() {
  Spark.variable("pressure", &pressure, INT);
}

void loop() {

  pressure = analogRead(A0);

}
