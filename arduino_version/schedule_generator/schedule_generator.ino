#include <time.h>
#include <math.h>

void cool_weather() {
  Serial.println("Weather Cooling...");
}
void heat_weather() {
  Serial.println("Weather Heating...");
}
void turn_off() {
  Serial.println("turned off");
}

#define INITIAL_SLEEP_TEMP 23
#define INITIAL_WAKE_MORNING_TEMP 21
#define INITIAL_WAKE_NIGHT_TEMP 24

#define TEMP_CHANGE_THRESHHOLD 2

#define SCHEDULE_ITEMS 168  //24 * 7

enum user_states {
  USER_OUT = 0,
  USER_SLEEP,
  USER_IN_MORNING,
  USER_IN_NIGHT
};

user_states user_schedule[SCHEDULE_ITEMS];
int user_schedule_temp[SCHEDULE_ITEMS];

bool is_morning(int hour) {
  return hour >= 6 && hour < 18;
}

bool is_night(int hour) {
  return hour >= 18 || hour < 6;
}

String* string_splitor(String str1) {
  static String strs[20];
  int StringCount = 0;
  String str = str1;
  while (str.length() > 0) {
    int index = str.indexOf(' ');
    if (index == -1)  // No space found
    {
      strs[StringCount++] = str;
      break;
    } else {
      strs[StringCount++] = str.substring(0, index);
      str = str.substring(index + 1);
    }
  }
  return strs;
}

void thermo_adjust(int current_temprature, int thermal_comfort) {
  if (thermal_comfort == 0) {
    turn_off();
  } else if (thermal_comfort > current_temprature && thermal_comfort - current_temprature > TEMP_CHANGE_THRESHHOLD) {
    heat_weather();
  } else if (thermal_comfort < current_temprature && thermal_comfort - current_temprature > TEMP_CHANGE_THRESHHOLD) {
    cool_weather();
  }
}

void assign_to_schedule(int start, int end, user_states action) {
  for (int i = start; i < end; i++) {
    user_schedule[i] = action;
  }
}

void assign_to_schedule_temp(int start, int end, int temp) {
  for (int i = start; i < end; i++) {
    user_schedule_temp[i] = temp;
  }
}

void initialize_user_schedule(String user_input) {
  String* user_input_vals = string_splitor(user_input);

  int day1 = user_input_vals[0].toInt();
  int start_hour = user_input_vals[1].toInt();
  int day2 = user_input_vals[2].toInt();
  int end_hour = user_input_vals[3].toInt();
  int action = user_input_vals[4].toInt();

  Serial.println(String(day1) + " " + String(start_hour) + " "+ String(day2) + " " + String(end_hour));

  int start_index = day1 * 24 + start_hour;
  int end_index = day2 * 24 + end_hour;

  switch (action) {
    case 0:
      assign_to_schedule(start_index, end_index, USER_SLEEP);
      assign_to_schedule_temp(start_index, end_index, INITIAL_SLEEP_TEMP);
      break;
    case 1:
      assign_to_schedule(start_index, end_index, USER_OUT);
      assign_to_schedule_temp(start_index, end_index, 0);
      break;
    case 2:
      // if (is_morning(start_hour) && is_morning(end_hour)) {
      assign_to_schedule(start_index, end_index, USER_IN_MORNING);
      assign_to_schedule_temp(start_index, end_index, INITIAL_WAKE_MORNING_TEMP);
      // } else if (is_morning(start_hour) && is_night(end_hour)) {

      // }
      break;
  }
}

void startup_app() {
  
  String input = "";
  do {
    while (!Serial.available())
    ;
    input = Serial.readStringUntil('\n');
    // Serial.println(input);
    initialize_user_schedule(input);
  } while (input!="finish");
  Serial.println("Task finished");
  for (int i = 0; i < SCHEDULE_ITEMS; i++) {
    Serial.print(String(user_schedule_temp[i]) + " ");
    if (i % 24 == 23) Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Program Started");
  startup_app();
}

void loop() {
  Serial.println("Main started");

  int current_temprature = 

  while (!Serial.available())
  ;

  String input = Serial.readStringUntil('\n');

  String* user_input_vals = string_splitor(input);

  int day = user_input_vals[0].toInt();
  int hour = user_input_vals[1].toInt();
  int temp = user_input_vals[2].toInt();
}
