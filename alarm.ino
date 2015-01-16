// pressure seems to range anywhere from
// 0 - 3500? Dividing this number by 10
// will give us a range from 0 - 350, giving
// us a more tolerant reading
int pressure = 0;

// the amount in which a person is in bed or not
int threshold = 200;

// this tells us if the person is in
// bed or not
int in_bed = 0;

void setup() {
  Spark.variable("pressure", &pressure, INT);
  Spark.variable("in_bed", &in_bed, INT);
}

void loop() {

  pressure = analogRead(A0) / 10;

  if (pressure > threshold) {
    in_bed = 1;
  } else {
    in_bed = 0;
  }

}
