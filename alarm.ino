// the amount in which a person is in bed or not
#define THRESHOLD 70

// how long should we delay before we
// register an action?
#define DELAY_MIN 900

// EEPROM locations
#define EEPROM_TIMEZONE 0
#define EEPROM_HOUR 1
#define EEPROM_MINUTE 2

// Configure the pins
#define PIN_SPEAKER A1
#define PIN_PRESSURE A0

// time zone
int timezone = 0;

// what time should the alarm be?
int alarm_time = 2561;
int alarm_hour = 25;
int alarm_minute = 61;

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

// time exposure
int current_hour = 0;
int current_minute = 0;

boolean previous_alarm_state = false;
void trigger_alarm(boolean state) {
  if (previous_alarm_state != state) {
    if (state) {
      tone(PIN_SPEAKER, 1908, 0);
    } else {
      noTone(PIN_SPEAKER);
    }
    previous_alarm_state = state;
  }
}

int set_timezone(String zone) {
  timezone = zone.toInt();
  EEPROM.write(EEPROM_TIMEZONE, timezone);
  Time.zone(timezone);
  return 1;
}

int set_alarm(String time) {
  alarm_time = time.toInt();
  alarm_hour = alarm_time / 100;
  alarm_minute = alarm_time % 100;

  EEPROM.write(EEPROM_HOUR, alarm_hour);
  EEPROM.write(EEPROM_MINUTE, alarm_minute);

  return 1;
}

void setup() {
  alarm_hour = EEPROM.read(EEPROM_HOUR);
  alarm_minute = EEPROM.read(EEPROM_MINUTE);
  alarm_time = (alarm_hour * 100) + alarm_minute;

  // funny thing is that the EEPROM doesn't
  // store negative values, so we're gonna
  // convert it here.
  timezone = EEPROM.read(EEPROM_TIMEZONE);
  if (timezone > 13) {
    timezone = (256 - timezone) * -1;
  }

  // set the timezone
  Time.zone(timezone);

  // register variables to be red
  // by the hub
  Spark.variable("pressure", &pressure, INT);
  Spark.variable("in_bed", &in_bed, BOOLEAN);
  Spark.variable("timezone", &timezone, INT);
  Spark.variable("hour", &current_hour, INT);
  Spark.variable("minute", &current_minute, INT);
  Spark.variable("alarm", &alarm_time, INT);

  // register functions callable
  // by the hub
  Spark.function("set_timezone", set_timezone);
  Spark.function("set_alarm", set_alarm);
}

// loop runs about every 5-15ms
void loop() {

  // read the input
  pressure = analogRead(PIN_PRESSURE) / 35;
  current_hour = Time.hour();
  current_minute = Time.minute();

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
  if (in_bed_read && Time.hour() >= alarm_hour && (Time.hour() - alarm_hour > 0 || Time.minute() > alarm_minute)) {
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
