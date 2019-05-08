// Libraries required//////////////////////////////////////////////////

#include "Adafruit_CCS811.h" //Library for the air quality sensor
//#include "RTClib.h" // Be sure you only have RTClib by Neiron Installed. Remove the Adafruit RTClib from your libraries as the call names are the same.

//Variables/////////////////////////////////////////////////////////////

//LEDs //–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––//
//Digital pins connected to transisor base for RGB control
#define REDPIN 5      //red color
#define GREENPIN 6     //green color
#define BLUEPIN 3      //blue color

#define FADESPEED 100     // make this higher to slow down the color change rate

int current_col[3] = {0}; //
int prev_col[3] = {0};

//CCS811 CO2,Temp Sensor //––––––––––––––––––––––––––––––––––––––––––––––//

//Nothing yet defined


//Instances for classes used here///////////////////////////////////////

//Adafruit CO2 sensor ccs definition
Adafruit_CCS811 ccs;

// Timer initiation
//RTC_Millis rtc;

///////////////////////////////////////////////////////////////////////////

void setup() {
  // RTC timer setup
  Serial.begin(57600);
  //  rtc.begin();
  //  if (! rtc.isrunning()) {
  //    Serial.println("RTC is NOT running!");
  //    // following line sets the RTC to the date & time this sketch was compiled
  //    rtc.adjust(DateTime(__DATE__, __TIME__));
  //  }

  //LED pin assignments as outputs
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  Serial.println("CCS811 test");
  if (!ccs.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
  }

  //calibrate temperature sensor in case you'd read it
  while (!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

}

/////////////////////////////////////////////////////////////////////////////

void loop() {

  //Reading the averaged CO2 level––––––––––––––––––––––––––––––––––––––––//

  //Read CO2, Temperature and TVOC averaged by the CS811_Averaged() function
  // You can pass "eCO2", "Temperature" or "TVOC" into CS811_Averaged() function
  //Here we only use CO2 for changing the light colors

  float CO2_read = CS811_Averaged(0);
  //float Temp_read = CS811_Averaged(1);
  //float TVOC_read = CS811_Averaged(2);

  Serial.print("CO2_READ: ");
  Serial.println(CO2_read);


  //Changing LED colors based on avg CO2––––––––––––––––––––––––––––––––//

  //Another way is to define color presets as separate variables and call them based on CO2 level
  int exTreme[3] = {255, 0, 0};
  int veryHigh_CO2 [3] = {255, 120, 0};
  int medHigh_CO2 [3] = {255, 255, 255};
  int medium_CO2 [3] = {0, 255, 0};
  int medLow_CO2 [3] = {0, 120, 0};
  int low_CO2 [3] = {20, 0, 20};
  int veryLow_CO2 [3] = {0, 0, 255};



  // Don't think you can put two arrays equal to each other
  //will need two new functions: 1) comparing arrays; 2) replacing arrays with on another.
  //
  //int current_col[3] = {0}; //
  //int next_col[3] = {0};





  //Decide the color based on CO2 readings
  if (CO2_read < 700) {
    array_swap(veryLow_CO2, current_col);
    Serial.println("very low");

  } else if (700 <= CO2_read && CO2_read < 2000) {
    array_swap(low_CO2, current_col);
    Serial.println("low");

  } else if (2000 <= CO2_read && CO2_read < 4000) {
    array_swap(medLow_CO2, current_col);
    Serial.println("medlow");

  } else if (4000 <= CO2_read && CO2_read < 6000) {
    array_swap(medium_CO2, current_col);
    Serial.println("medium");

  } else if (6000 <= CO2_read && CO2_read < 800 ) {
    array_swap(medHigh_CO2, current_col);
    Serial.println("medium High");

  } else if (8000 <= CO2_read && CO2_read < 10000) {
    array_swap(veryHigh_CO2, current_col);
    Serial.println("very High");

  } else {
    array_swap(exTreme, current_col);
    Serial.println("extreme!!!");
  }


bool result = array_comp(current_col, prev_col);

if (result){
  set_LED_colors(current_col);
} else{
  change_LED_colors (current_col, prev_col);
}

  print_array(prev_col);
  
  array_swap(current_col, prev_col);
  
  print_array(prev_col);

  delay(500);

  ////Passed!! Test with variation in ranges
  //if (CO2_read < 700) {
  //set_LED_colors(veryLow_CO2);
  //  Serial.println("very low");
  //}else if (700 <= CO2_read && CO2_read < 2000) {
  //  set_LED_colors(low_CO2);
  //  Serial.println("low");
  //}else if (2000 <= CO2_read && CO2_read < 4000) {
  //  set_LED_colors(medLow_CO2);
  //  Serial.println("medlow");
  //}else if (4000 <= CO2_read && CO2_read < 6000) {
  //   set_LED_colors(medium_CO2);
  //  Serial.println("medium");
  //}else if (6000 <= CO2_read && CO2_read < 800 ) {
  //   set_LED_colors(medHigh_CO2);
  //  Serial.println("medium High");
  //}else if (8000 <= CO2_read && CO2_read < 10000) {
  //   set_LED_colors(veryHigh_CO2);
  //  Serial.println("very High");
  //}else{
  //   set_LED_colors(exTreme);
  //  Serial.println("extreme!  ");
  //}


  //  change_LED_colors(current_col, low_CO2);

  //Color tests
  //set_LED_colors(veryLow_CO2);
  //  Serial.println("very low");
  //delay(2000);
  //  set_LED_colors(low_CO2);
  //  Serial.println("low");
  //delay(2000);
  //  set_LED_colors(medLow_CO2);
  //  Serial.println("medlow");
  //delay(2000);
  //   set_LED_colors(medium_CO2);
  //  Serial.println("medium");
  //delay(2000);
  //   set_LED_colors(medHigh_CO2);
  //  Serial.println("medium High");
  //delay(2000);
  //   set_LED_colors(veryHigh_CO2);
  //  Serial.println("very High");
  //  delay(2000);
  //   set_LED_colors(exTreme);
  //  Serial.println("High");
  //delay(2000);
  //

}


/////////////////////////////////////////////////////////////////////////////
//functions used for preparing the LED strip in various colors

void set_LED_colors (int color[3]) {
  //variables fed into the function are indexed for R,G,B
  // This makes it easier to feed color combination of interest into the LEDs

  analogWrite(REDPIN, color[0]);
  analogWrite(GREENPIN, color[1]);
  analogWrite(BLUEPIN, color[2]);
}


void change_LED_colors (int color_i[3], int color_f[3]) {
  //This function will help fade from one color preset (i) to another set (f)
  //by slowly fading red, then green and finally blue

  //initial color combination
  int r_i = color_i[0];
  int g_i = color_i[1];
  int b_i = color_i[2];

  //final color combination
  int r_f = color_f[0];
  int g_f = color_f[1];
  int b_f = color_f[2];

  // delay for color transitions
  const int r_delay = 10;
  const int g_delay = 10;
  const int b_delay = 10;

  // fade red pin first
  // the while loop compares the red values, if there is a difference it activates a fade

  while (r_f != r_i) {
    analogWrite(REDPIN, r_i);
    delay(r_delay);
    // if statement determines whether final or initial r value is larger
    // based on the result the r_i value will slowly increase or decrease
    if (r_f > r_i) {
      r_i++;
    } else {
      r_i--;
    }
  }

  // fade green pin second
  // the while loop compares the green values, if there is a difference it activates a fade
  while (g_f != g_i) {
    analogWrite(GREENPIN, g_i);
    delay(g_delay);

    if (g_f > g_i) {
      g_i++;
    } else {
      g_i--;
    }
  }

  // fade blue pin third
  // the while loop compares the blue values, if there is a difference it activates a fade
  while (b_f != b_i) {
    analogWrite(BLUEPIN, b_i);
    delay(b_delay);

    if (b_f > b_i) {
      b_i++;
    } else {
      b_i--;
    }
  }

}



// Function for averaging over the data readings from CCS811 sensor

float CS811_Averaged (int data_request) {

  //variables for turning # of data points and delay in between measurements
  int pt_num = 10; // Determines the number of data points to average over
  int meas_delay = 50; // Delay between each measurement for pt_num number of data points

  //total variables calculated over the measurement period
  float tot_temp = 0;
  float tot_CO2 = 0;
  float tot_TVOC = 0;

  Serial.print("Data Request: ");
  Serial.println(data_request);

  // for loop that averages over pt_num ponits with meas_delay time delay in between each point
  if (ccs.available()) {
    if (!ccs.readData()) {
      for (int i = 0; i <  pt_num; i++) {
        tot_CO2 = tot_CO2 + ccs.geteCO2();
        tot_temp  = tot_temp + ccs.calculateTemperature();
        tot_TVOC = tot_TVOC + ccs.getTVOC();
        delay(meas_delay);
      }
    } else {
      Serial.println("ERROR!");
      while (1);
    }
  }

  // average value calculated based on the measurements and stored in a 3-element array
  float avg_dataset[3] = {tot_CO2 / pt_num, tot_temp / pt_num, tot_TVOC / pt_num};

  Serial.print("Avg CO2: ");
  Serial.println(avg_dataset[0]);


  //average value returned by the function based on the input parameter

  if (data_request == 0) {
    return avg_dataset[0];
  } else if (data_request == 1) {
    return avg_dataset[1];
  } else if (data_request == 2) {
    return avg_dataset[2];
  } else {
    Serial.println("Invalid Data Request!!");
  }

}


//function that takes the elements of one array and replace it with another array's
//basically element swapping function

void array_swap(int arr_1[], int arr_2[]) {

  //arr_1 is the reference, arr_2 is the one to be changed

  for (int i = 0;  i < 3; i++)
  {
    arr_2[i] = arr_1[i];
    //Serial.println(arr_2[i]);
  }

  ////Serial.println("equal!");
  //return arr_2;
  //
}


//prints arrays in between brackets similar to lists in python
void print_array(int arr_0[]) {

  //arr_1 is the reference, arr_2 is the one to be changed

  Serial.print("[ ");

  for (int i = 0;  i < 3; i++)
  {
    Serial.print(arr_0[i]);
    Serial.print(" ");
  }

  Serial.println(" ]");

}


//function that takes two arrays, compares them and return result
// as true/false boolean
bool array_comp(int arr_1[], int arr_2[]) {

  for (int i = 0;  i < 3; i++)
  {
    if ( arr_1[i] != arr_2[i] )
    {
      //Serial.println("not equal");
      //set your boolean flag here
      return false;
    }
  }

  //Serial.println("equal!");
  return true;

}






/// End of the script
