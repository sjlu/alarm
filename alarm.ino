// the amount in which a person is in bed or not
int THRESHOLD = 70;

// how long should we delay before we
// register an action?
int DELAY_MIN = 900;

// pressure seems to range anywhere from
// 0 - 3500? Dividing this number by 35
// will give us a range from 0 - 100, giving
// us a more tolerant reading
int pressure = 0;

// this tells us if the person is in
// bed or not
boolean in_bed = false;

// the loop is too fast and if someone
// moves in the bed, it'll cause a momentary
// on/off in in_bed and send junk signals,
// therefore we will do a momentary delay
// to see if they cross the threshold again
int delay_time = 0;

void setup() {
  // register variables to be red
  // by the hub
  Spark.variable("pressure", &pressure, INT);
  Spark.variable("in_bed", &in_bed, BOOLEAN);
}

// loop runs about every 5-15ms
void loop() {

  // read the input
  pressure = analogRead(A0) / 35;

  boolean in_bed_read = pressure > THRESHOLD;
  if (in_bed_read != in_bed) {
    delay_time = delay_time + 1;
  } else {
    delay_time = 0;
  }

  if (delay_time > DELAY_MIN) {
    in_bed = in_bed_read;
    Spark.publish("alarm/is_in_bed", in_bed ? "true" : "false", 60, PRIVATE);
    delay_time = 0;
  }

}
