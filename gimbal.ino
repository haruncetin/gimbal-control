#include <LedControl.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>

#define PANPOT      0       // analog pin used to connect the pan potentiometer
#define TILTPOT     1       // analog pin used to connect the tilt potentiometer
#define PANSERVO    9       // pan servo pin
#define TILTSERVO   10      // tilt servo pin

#define CLK         5       // dot matrix clock signal pin
#define CS          6       // dot matrix chip select pin
#define DIN         7       // dot matrix data in pin
#define MATRIXCOUNT 1       // dot Matrix count

LedControl disp = LedControl(DIN, CLK, CS, MATRIXCOUNT);
Servo pan, tilt;            // pan & tilt objects
int pan_val, tilt_val;      // variables to read the values from the analog pan & tilt pins

const byte images[][8] =
{{
    0,          //heart
    B01100110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000
  },
  {
    B00111100,  //sad
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100
  },
  {
    B00111100,  //normal
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100
  },
  {
    B00111100,  //smile
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100
  },
  {
    B00111100,  //kiss
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10011001,
    B01000010,
    B00111100
  }
};

/* Task prototypes */
void TaskDisplay( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

void print_to(const byte* ch)
{
  for (int i = 0; i < 8; i++)
  {
    disp.setRow(0, i, ch[i]);
  }
}

void setup() {
  disp.setIntensity(0, 10);         // set dot matrix led brightness
  disp.shutdown(0, false);          // activate dot matrix
  disp.clearDisplay(0);

  pan.attach(PANSERVO);             // attaches the servo on pin 9 to the servo object that drive the pan servo
  tilt.attach(TILTSERVO);           // attaches the servo on pin 10 to the servo object that drive the tilt servo

  xTaskCreate(
    TaskDisplay
    ,  (const portCHAR *)"Display"  // a name just for humans
    ,  128                          // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2                            // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
  );

  xTaskCreate(
    TaskAnalogRead
    ,  (const portCHAR *) "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL
  );
}

void loop() {

}

void TaskDisplay(void *pvParameters)        // Dot matrix display task
{
  (void) pvParameters;
  for (;;) {

    print_to(images[2]);
    vTaskDelay( 2000 / portTICK_PERIOD_MS );
    print_to(images[3]);
    vTaskDelay( 2500 / portTICK_PERIOD_MS );
    print_to(images[4]);
    vTaskDelay( 2500 / portTICK_PERIOD_MS );

    print_to(images[0]);                    // heart beat 7 times
    for (int i = 0; i < 7; i++) {
      for (int i = 10; i < 0; i--)          // decrease brightness
      {
        disp.setIntensity(0, i);
        vTaskDelay( 125 / portTICK_PERIOD_MS );
      }
      for (int i = 0; i < 10; i++)          // increase brightness
      {
        disp.setIntensity(0, i);
        vTaskDelay( 125 / portTICK_PERIOD_MS );
      }
    }
  }
}

void TaskAnalogRead(void *pvParameters)  // analog read task
{
  (void) pvParameters;
  for (;;)
  {
    pan_val = analogRead(PANPOT);             // reads the value of the pan potentiometer (value between 0 and 1023)
    tilt_val = analogRead(TILTPOT);           // reads the value of the tilt potentiometer (value between 0 and 1023)
    pan_val = map(pan_val, 0, 1023, 0, 180);      // scale it to use it with the pan servo (value between 0 and 180)
    pan.write(pan_val);                           // sets the pan servo position according to the scaled value
    tilt_val = map(tilt_val, 0, 1023, 0, 180);    // scale it to use it with the tilt servo (value between 0 and 180)
    tilt.write(tilt_val);                         // sets the tilt servo position according to the scaled value
    vTaskDelay(25 / portTICK_PERIOD_MS );                                    // waits for the servos to get there
  }
}
