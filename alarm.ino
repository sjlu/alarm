// the amount in which a person is in bed or not
int THRESHOLD = 70;

// how long should we delay before we
// register an action?
int DELAY_MIN = 900;

// time zone
int TIMEZONE = -5;

// what time should the alarm be?
int ALARM_HOUR = 9;
int ALARM_MINUTE = 0;

// Configure the pins
int SPEAKER_PIN = A1;
int PRESSURE_PIN = A0;

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

// time since last sync
unsigned long last_sync = millis();

boolean previous_alarm_state = false;
void trigger_alarm(boolean state) {
  if (previous_alarm_state != state) {
    if (state) {
      tone(SPEAKER_PIN, 1908, 0);
    } else {
      noTone(SPEAKER_PIN);
    }
    previous_alarm_state = state;
  }
}

void setup() {
  // register variables to be red
  // by the hub
  Spark.variable("pressure", &pressure, INT);
  Spark.variable("in_bed", &in_bed, BOOLEAN);

  // set the timezone
  // TODO: this should be synced from server
  Time.zone(TIMEZONE);
}

// loop runs about every 5-15ms
void loop() {

  // read the input
  pressure = analogRead(PRESSURE_PIN) / 35;

  // if the input has changed
  // increment the delay timer
  boolean in_bed_read = pressure > THRESHOLD;
  if (in_bed_read != in_bed) {
    delay_time = delay_time + 1;
  } else {
    delay_time = 0;
  }

  // if the delay timer has reached
  // the minimum amount of time to switch
  // then we switch and accounce
  if (delay_time > DELAY_MIN) {
    in_bed = in_bed_read;
    Spark.publish("alarm/is_in_bed", in_bed ? "true" : "false", 60, PRIVATE);
    delay_time = 0;
  }

  // if they are in bed, and we have reached the appropriate
  // time, we want to start the alarm, else we just turn it off
  if (in_bed_read && Time.hour() >= ALARM_HOUR && (Time.hour() - ALARM_HOUR > 0 || Time.minute() > ALARM_MINUTE)) {
    // this causes a delay when activating the alarm
    // but instantly deactivates when out of bed
    if (in_bed) {
      trigger_alarm(true);
    }
  } else {
    trigger_alarm(false);
  }

  // Request time synchronization from the Spark Cloud
  if (millis() - last_sync > 86400000) { // 1 day
    Spark.syncTime();
    last_sync = millis();
  }

}
