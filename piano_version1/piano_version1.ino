/***********************************************************/
// Demo for the Serial MP3 Player Catalex (YX5300 chip)
// Hardware: Serial MP3 Player *1
// Board:  Arduino UNO
// http://www.dx.com/p/uart-control-serial-mp3-music-player-module-for-arduino-avr-arm-pic-blue-silver-342439#.VfHyobPh5z0
//
//
//

#define vibration_1 10
#define vibration_2 11
#define vibration_3 12
#define vibration_4 13
int vibration_state = LOW;
// Uncomment SoftwareSerial for Arduino Uno or Nano.
#include <Bounce2.h>
// WE WILL attach() THE BUTTON TO THE FOLLOWING PIN IN setup()
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 8
#define BUTTON4_PIN 9
// INSTANTIATE A Button OBJECT FROM THE Bounce2 NAMESPACE
Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();
Bounce2::Button button3 = Bounce2::Button();
Bounce2::Button button4 = Bounce2::Button();


#include <SoftwareSerial.h>

#define ARDUINO_RX 5  //should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 6  //connect to RX of the module

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
//#define mp3 Serial3    // Connect the MP3 Serial Player to the Arduino MEGA Serial3 (14 TX3 -> RX, 15 RX3 -> TX)

static int8_t Send_buf[8] = { 0 };  // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = { 0 };  // Buffer for the answers.    // BETTER LOCALLY

String mp3Answer;  // Answer from the MP3.

/************ Command byte **************************/
#define CMD_NEXT_SONG 0X01  // Play next song.
#define CMD_PREV_SONG 0X02  // Play previous song.
#define CMD_PLAY_W_INDEX 0X03
#define CMD_VOLUME_UP 0X04
#define CMD_VOLUME_DOWN 0X05
#define CMD_SET_VOLUME 0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV 0X09
#define CMD_SLEEP_MODE 0X0A
#define CMD_WAKE_UP 0X0B
#define CMD_RESET 0X0C
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY 0X16
#define CMD_FOLDER_CYCLE 0X17
#define CMD_SHUFFLE_PLAY 0x18   //
#define CMD_SET_SNGL_CYCL 0X19  // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON 0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL 0X22
#define CMD_PLAYING_N 0x4C
#define CMD_QUERY_STATUS 0x42
#define CMD_QUERY_VOLUME 0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS 0x48
#define CMD_QUERY_FLDR_COUNT 0x4f

/************ Opitons **************************/
#define DEV_TF 0X02


/*********************************************************************/

void setup() {
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);

  sendCommand(CMD_SEL_DEV, DEV_TF);
  delay(500);
  // BUTTON SETUP

  // SELECT ONE OF THE FOLLOWING :
  // 1) IF YOUR BUTTON HAS AN INTERNAL PULL-UP
  // button.attach( BUTTON_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  // 2) IF YOUR BUTTON USES AN EXTERNAL PULL-UP
  button1.attach(BUTTON1_PIN, INPUT);  // USE EXTERNAL PULL-UP
  button2.attach(BUTTON2_PIN, INPUT);
  button3.attach(BUTTON3_PIN, INPUT);
  button4.attach(BUTTON4_PIN, INPUT);
  pinMode(vibration_1, OUTPUT);
  digitalWrite(vibration_1, vibration_state);
  pinMode(vibration_2,OUTPUT);
  digitalWrite(vibration_2, vibration_state);
    pinMode(vibration_3,OUTPUT);
  digitalWrite(vibration_3, vibration_state);
    pinMode(vibration_4,OUTPUT);
  digitalWrite(vibration_4, vibration_state);


  // DEBOUNCE INTERVAL IN MILLISECONDS
  button1.interval(5);
  button2.interval(5);
  button3.interval(5);
  button4.interval(5);

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button1.setPressedState(HIGH);
  button2.setPressedState(HIGH);
  button3.setPressedState(HIGH);
  button4.setPressedState(HIGH);
}


void loop() {
  char c = ' ';
  button1.update();
  button2.update();
  button3.update();
  button4.update();


  // <Button>.rose() RETURNS true IF THE BUTTON TRANSITIONS FROM LOW TO HIGH
  if (button1.rose()) {
    // TOGGLE THE LED STATE : 
    vibration_state = HIGH; // SET ledState TO LOW
    digitalWrite(vibration_1, vibration_state); // WRITE THE NEW ledState
    Serial.println("ON");

  }

  // <Button>.fell() RETURNS true IF THE BUTTON TRANSITIONS FROM HIGH TO LOW
  if (button1.fell()) {
    // TOGGLE THE LED STATE : 
    vibration_state = LOW; // SET ledState TO HIGH
    digitalWrite(vibration_1, vibration_state); // WRITE THE NEW ledState
    Serial.println("OFF");
        c = '1';
    sendMP3Command(c);
  }

  if (button2.rose()) {
    // TOGGLE THE LED STATE : 
    vibration_state = HIGH; // SET ledState TO LOW
    digitalWrite(vibration_2, vibration_state); // WRITE THE NEW ledState
    Serial.println("ON");

  }

  // <Button>.fell() RETURNS true IF THE BUTTON TRANSITIONS FROM HIGH TO LOW
  if (button2.fell()) {
    // TOGGLE THE LED STATE : 
    vibration_state = LOW; // SET ledState TO HIGH
    digitalWrite(vibration_2, vibration_state); // WRITE THE NEW ledState
    Serial.println("OFF");
        c = '2';
    sendMP3Command(c);
  }

  if (button3.rose()) {
    // TOGGLE THE LED STATE : 
    vibration_state = HIGH; // SET ledState TO LOW
    digitalWrite(vibration_3, vibration_state); // WRITE THE NEW ledState
    Serial.println("ON");

  }

  // <Button>.fell() RETURNS true IF THE BUTTON TRANSITIONS FROM HIGH TO LOW
  if (button3.fell()) {
    // TOGGLE THE LED STATE : 
    vibration_state = LOW; // SET ledState TO HIGH
    digitalWrite(vibration_3, vibration_state); // WRITE THE NEW ledState
    Serial.println("OFF");
        c = '3';
    sendMP3Command(c);
  }

  if (button4.rose()) {
    // TOGGLE THE LED STATE : 
    vibration_state = HIGH; // SET ledState TO LOW
    digitalWrite(vibration_4, vibration_state); // WRITE THE NEW ledState
    Serial.println("ON");

  }

  // <Button>.fell() RETURNS true IF THE BUTTON TRANSITIONS FROM HIGH TO LOW
  if (button4.fell()) {
    // TOGGLE THE LED STATE : 
    vibration_state = LOW; // SET ledState TO HIGH
    digitalWrite(vibration_4, vibration_state); // WRITE THE NEW ledState
    Serial.println("OFF");
        c = '4';
    sendMP3Command(c);
  }
  else {
    
  }

    // If there a char on Serial call sendMP3Command to sendCommand
    /*if ( Serial.available() )
  {
    c = Serial.read();
    sendMP3Command(c);
  } */

    // Check for the answer.
    if (mp3.available()) {
      Serial.println(decodeMP3Answer());
    }
  delay(100);
}


/********************************************************************************/
/*Function sendMP3Command: seek for a 'c' command and send it to MP3  */
/*Parameter: c. Code for the MP3 Command, 'h' for help.                                                                                                         */
/*Return:  void                                                                */

void sendMP3Command(char c) {
  switch (c) {
    case '?':
    case 'h':
      Serial.println("HELP  ");
      Serial.println(" p = Play");
      Serial.println(" P = Pause");
      Serial.println(" > = Next");
      Serial.println(" < = Previous");
      Serial.println(" + = Volume UP");
      Serial.println(" - = Volume DOWN");
      Serial.println(" c = Query current file");
      Serial.println(" q = Query status");
      Serial.println(" v = Query volume");
      Serial.println(" x = Query folder count");
      Serial.println(" t = Query total file count");
      Serial.println(" 1 = Play folder 1");
      Serial.println(" 2 = Play folder 2");
      Serial.println(" 3 = Play folder 3");
      Serial.println(" 4 = Play folder 4");
      Serial.println(" 5 = Play folder 5");
      Serial.println(" S = Sleep");
      Serial.println(" W = Wake up");
      Serial.println(" r = Reset");
      break;


    case 'p':
      Serial.println("Play ");
      sendCommand(CMD_PLAY, 0);
      break;

    case 'P':
      Serial.println("Pause");
      sendCommand(CMD_PAUSE, 0);
      break;

    case '>':
      Serial.println("Next");
      sendCommand(CMD_NEXT_SONG, 0);
      sendCommand(CMD_PLAYING_N, 0x0000);  // ask for the number of file is playing
      break;

    case '<':
      Serial.println("Previous");
      sendCommand(CMD_PREV_SONG, 0);
      sendCommand(CMD_PLAYING_N, 0x0000);  // ask for the number of file is playing
      break;

    case '+':
      Serial.println("Volume Up");
      sendCommand(CMD_VOLUME_UP, 0);
      break;

    case '-':
      Serial.println("Volume Down");
      sendCommand(CMD_VOLUME_DOWN, 0);
      break;

    case 'c':
      Serial.println("Query current file");
      sendCommand(CMD_PLAYING_N, 0);
      break;

    case 'q':
      Serial.println("Query status");
      sendCommand(CMD_QUERY_STATUS, 0);
      break;

    case 'v':
      Serial.println("Query volume");
      sendCommand(CMD_QUERY_VOLUME, 0);
      break;

    case 'x':
      Serial.println("Query folder count");
      sendCommand(CMD_QUERY_FLDR_COUNT, 0);
      break;

    case 't':
      Serial.println("Query total file count");
      sendCommand(CMD_QUERY_TOT_TRACKS, 0);
      break;

    case '1':
      Serial.println("Play file 1");
      sendCommand(CMD_PLAY_W_INDEX, 1);
      break;

    case '2':
      Serial.println("Play file 2");
      sendCommand(CMD_PLAY_W_INDEX, 2);
      break;

    case '3':
      Serial.println("Play file 3");
      sendCommand(CMD_PLAY_W_INDEX, 3);
      break;

    case '4':
      Serial.println("Play file 4");
      sendCommand(CMD_PLAY_W_INDEX, 4);
      break;

    case '5':
      Serial.println("Play file 5");
      sendCommand(CMD_PLAY_W_INDEX, 5);
      break;

    case 'S':
      Serial.println("Sleep");
      sendCommand(CMD_SLEEP_MODE, 0x00);
      break;

    case 'W':
      Serial.println("Wake up");
      sendCommand(CMD_WAKE_UP, 0x00);
      break;

    case 'r':
      Serial.println("Reset");
      sendCommand(CMD_RESET, 0x00);
      break;
  }
}



/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */

String decodeMP3Answer() {
  String decodedMP3Answer = "";

  decodedMP3Answer += sanswer();

  switch (ansbuf[3]) {
    case 0x3A:
      decodedMP3Answer += " -> Memory card inserted.";
      break;

    case 0x3D:
      decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
      break;

    case 0x40:
      decodedMP3Answer += " -> Error";
      break;

    case 0x41:
      decodedMP3Answer += " -> Data recived correctly. ";
      break;

    case 0x42:
      decodedMP3Answer += " -> Status playing: " + String(ansbuf[6], DEC);
      break;

    case 0x48:
      decodedMP3Answer += " -> File count: " + String(ansbuf[6], DEC);
      break;

    case 0x4C:
      decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
      break;

    case 0x4E:
      decodedMP3Answer += " -> Folder file count: " + String(ansbuf[6], DEC);
      break;

    case 0x4F:
      decodedMP3Answer += " -> Folder count: " + String(ansbuf[6], DEC);
      break;
  }

  return decodedMP3Answer;
}






/********************************************************************************/
/*Function: Send command to the MP3                                         */
/*Parameter:-int8_t command                                                     */
/*Parameter:-int16_ dat  parameter for the command                              */

void sendCommand(int8_t command, int16_t dat) {
  delay(20);
  Send_buf[0] = 0x7e;                //
  Send_buf[1] = 0xff;                //
  Send_buf[2] = 0x06;                // Len
  Send_buf[3] = command;             //
  Send_buf[4] = 0x01;                // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8);  //datah
  Send_buf[6] = (int8_t)(dat);       //datal
  Send_buf[7] = 0xef;                //
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]);
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}



/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                 */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */


String sbyte2hex(uint8_t b) {
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}




/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.          */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */

String sanswer(void) {
  uint8_t i = 0;
  String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10)) {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  // if the answer format is correct.
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF)) {
    return mp3answer;
  }

  return "???: " + mp3answer;
}
