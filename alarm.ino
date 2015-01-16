// pressure seems to range anywhere from
// 0 - 3500? Dividing this number by 35
// will give us a range from 0 - 100, giving
// us a more tolerant reading
int pressure = 0;

// the amount in which a person is in bed or not
int threshold = 80;

// this tells us if the person is in
// bed or not
boolean in_bed = false;

void setup() {
  Spark.variable("pressure", &pressure, INT);
  Spark.variable("in_bed", &in_bed, BOOLEAN);
}

void loop() {

  int pre_pressure = analogRead(A0) / 35;

  if (pressure > threshold) {
    if (!in_bed) {
      Spark.publish("alarm/is_in_bed", "true", 60, PRIVATE);
    }
    in_bed = true;
  } else {
    if (in_bed) {
      Spark.publish("alarm/is_in_bed", "false", 60, PRIVATE);
    }
    in_bed = false;
  }

}
