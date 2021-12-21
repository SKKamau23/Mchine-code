# Mchine-code
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include <LCDWIKI_TOUCH.h> //touch screen library

//define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
LCDWIKI_KBV mylcd(ILI9488,40,38,39,44,41);//width,height,cs,cd,wr,rd,reset
LCDWIKI_TOUCH mytouch(53,52,50,51,44); //tcs,tclk,tdout,tdin,tirq
int current_page = 0;
uint8_t val;

uint16_t px,py;

//Clock parameters
#include <Wire.h>
#include "RTClib.h" //wire.h & RTClib.h libraries for communicating with the module.

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //2D character array to store days information.
bool yesTime = false;
String Day, MMonth, YYear, HHour, MMinute, Second, quote, space, colon;
String Day_ICU, MMonth_ICU, YYear_ICU, HHour_ICU, MMinute_ICU; //For ICU mode
String clockstring1, clockstring2, clockstring7, clockstring8, clockstring9;
//End of clock parameters

//Start of keypad initialization code
#include <Keypad.h>
const byte Rows= 4; //number of rows on the keypad i.e. 4
const byte Cols= 3; //number of columns on the keypad i,e, 3
char keymap[Rows][Cols]=
{
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};
byte rPins[Rows]= {A6,A5,A4,A3}; //Rows 0 to 3
byte cPins[Cols]= {A2,A1,A0}; //Columns 0 to 2
Keypad kpd= Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);
bool runAgain = false;
String inputString, clockstringCombined1, clockstringCombined2;
//Row 1
String medicineCode, code1, code2, date, date1, date2, Month, Month1, Month2, Year, Year1, Year2, Year3, Year4, Hour, Hour1, Hour2, Minute, Minute1, Minute2, clockstring3, clockstring4;
String Flow, Flow1, Flow2;
int IMedicineCode, IDate, IMonth, IYear, IHour, IMinute, IFlow;
//End of Row 1
//Row 2
String medicineCode3, code12, code22, date3, date12, date22, Month3, Month12, Month22, Year5, Year12, Year22, Year32, Year42, Hour3, Hour12, Hour22, Minute3, Minute12, Minute22, clockstring5, clockstring6;
String Flow3, Flow12, Flow22;
int IMedicineCode2, IDate2, IMonth2, IYear2, IHour2, IMinute2, IFlow2;
int flowRate1, flowRate2, medcode1, medcode2;
//End of Row 2
bool truth = false;
bool runOnceICU1 = false;
bool runOnceICU2 = false;
bool one = false;
//End of keypad initialization code


//Surgical mode parameters
#define knob A8 //potentiometer
#define led_surgery 12 //blue led
#define machine_LED 19 //blue led
#define buzzer 11 //speaker
#define button 18 //push button
int machine_status;
int value;
int PUL=7; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=5; //define Enable Pin
int i;
bool serial = false;
int t;
bool runallowed = false;
bool reset1 = false;
bool Once1 = false;
bool Once2 = false;
//End of surgical mode parameters

//ICU pump parameters 1
int pulse = 4; //define Pulse pin
int dir = 3; //define Direction pin
int enable = 2; //define Enable Pin
//End of ICU pump parameters

//ICU pump parameters 2
int p = 8; //define Pulse pin
int d = 9; //define Direction pin
int e = 10; //define Enable Pin
bool just_once = false;
//End of ICU pump parameters

boolean is_pressed(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t px,int16_t py)
{
    if((px > x1 && px < x2) && (py > y1 && py < y2))
    {
        return true;  
    } 
    else
    {
        return false;  
    }
 }

 void menu_button()
{
  mylcd.Draw_Rectangle(360, 265, 475, 315);
  mylcd.Set_Text_Size(4.0);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Print_String("MENU", 370, 275);
}

void startup()
{
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(4.0);
  mylcd.Print_String("WELCOME", 160, 140);
  menu_button();
}

void menu1()
{
  mylcd.Fill_Screen(BLACK);
  mylcd.Draw_Rectangle(5, 5, 255, 45);
  mylcd.Set_Text_Size(2.8);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Print_String("OPERATING MODES", 10, 15);
  mylcd.Draw_Rectangle(5, 65, 255, 105);
  mylcd.Print_String("ALARMS", 10, 75);
  mylcd.Draw_Rectangle(5, 125, 255, 165);
  mylcd.Print_String("LOG FIEES", 10, 135);
}

void operating_modes()
{
  mylcd.Fill_Screen(BLACK);
  mylcd.Draw_Rectangle(5, 5, 123, 65);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(2.8);
  mylcd.Print_String("ICU MODE", 10, 15);

  mylcd.Draw_Rectangle(5, 80, 123, 140);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(2.8);
  mylcd.Print_String("SURGERY", 10, 90);
  mylcd.Print_String("MODE", 10, 115);
}

void back_button()
{
  mylcd.Fill_Triangle(420, 290, 440, 270, 440, 310);
  mylcd.Fill_Rectangle(440, 280, 470, 300);
  mylcd.Draw_Rectangle(415, 265, 475, 315);
}

void start_button()
{
  mylcd.Set_Text_Size(4.0);
  mylcd.Set_Text_Back_colour(GREEN);
  mylcd.Draw_Rectangle(10, 275, 128, 306);
  mylcd.Print_String("START", 10, 275);
}

void icu_mode()
{
  mylcd.Draw_Rectangle(5, 5, 475, 215);
  mylcd.Draw_Fast_HLine(5, 70, 470);
  mylcd.Draw_Fast_HLine(5, 140, 470);
  mylcd.Draw_Fast_VLine(156.7, 5, 210);
  mylcd.Draw_Fast_VLine(313.3, 5, 210);
  mylcd.Set_Text_Size(2.0);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Print_String("Medicine", 10, 10);
  mylcd.Print_String("code", 10, 40);
  mylcd.Print_String("Time to", 161.7, 10);
  mylcd.Print_String("administer", 161.7, 40);
  mylcd.Print_String("Flow rate", 318.3, 10);
  mylcd.Print_String("(ml/hr)", 318.3, 40);
  start_button();
}

void surgery_mode()
{
  mylcd.Draw_Rectangle(5, 5, 475, 140);
  mylcd.Draw_Fast_HLine(5, 70, 470);
  mylcd.Draw_Fast_HLine(5, 140, 470);
  mylcd.Draw_Fast_VLine(156.7, 5, 135);
  mylcd.Draw_Fast_VLine(313.3, 5, 135);
  mylcd.Set_Text_Size(2.0);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Print_String("Medicine", 10, 10);
  mylcd.Print_String("name", 10, 40);
  mylcd.Print_String("Vasopressor", 15, 100);
  mylcd.Print_String("Time", 161.7, 10);
  mylcd.Print_String("administered", 161.7, 40);
  mylcd.Print_String("Flow rate", 318.3, 10);
  mylcd.Print_String("(ml/hr)", 318.3, 40);
  mylcd.Print_String("20ml/hr", 323.3, 100);
  mylcd.Draw_Rectangle(5, 155, 475, 225);
  mylcd.Draw_Fast_VLine(240, 155, 70);
  mylcd.Print_String("Blood pressure", 15, 170);   
  Clock();
}

void startWindow()
{
  mylcd.Fill_Screen(BLACK);
  mylcd.Set_Text_Size(2.0);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Draw_Rectangle(5, 5, 350, 95);
  mylcd.Draw_Rectangle(5, 110, 350, 180);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mylcd.Init_LCD();
  mylcd.Set_Rotation(1);
  mytouch.TP_Init(mylcd.Get_Rotation(),mylcd.Get_Display_Width(),mylcd.Get_Display_Height()); 
  mylcd.Fill_Screen(BLACK);
  startup();
  
  //Surgery mode setup
  pinMode(buzzer, OUTPUT);
  pinMode (led_surgery, OUTPUT);
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (pulse, OUTPUT);
  pinMode (dir, OUTPUT);
  pinMode (enable, OUTPUT);
  pinMode (p, OUTPUT);
  pinMode (d, OUTPUT);
  pinMode (e, OUTPUT);
  pinMode (knob, INPUT);
  //End of surgery mode setup

  //Clock setup parameters
  Serial.begin(9600);
  delay(3000); // wait for console opening
  if (! rtc.begin())
   { 
    while (1);
   }
  if (rtc.lostPower()) 
   { //lostPower() function reads the DS3231’s internal I2C registers to check if the chip has lost track of time. If the function returns true, we can then set the date & time.
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
  //End of clock setup parameters
}

void loop() {
  // put your main code here, to run repeatedly:
    px = 0;
    py = 0;
    mytouch.TP_Scan(0);
    KeyPad();
    if (mytouch.TP_Get_State()&TP_PRES_DOWN) 
    {
      px = mytouch.x;
      py = mytouch.y;
    }  
  
    if ((current_page == 0) && (is_pressed(5, 265, 120, 315, px, py))) //  if(is_pressed(215,5,315,60,px,py))
     {
       current_page = 1;
       menu1();
     }
  
     else if ((current_page == 1) && (is_pressed(362, 5, 475, 65, px, py))) // These are the operating modes of the machine which include ICU mode and Surgery mode. Operating mode is page2.
     {
        current_page = 2;
        operating_modes();
        menu_button();
     }
  
     else if ((current_page == 2) && (is_pressed(362, 80, 475, 140, px, py))) //Initializing Surgery mode. Surgery page is 3.
     { 
       current_page = 3;
       mylcd.Fill_Screen(BLACK);
       surgery_mode();
       back_button();
     }
  
     else if ((current_page == 2) && (is_pressed(362, 5, 475, 65, px, py))) //Initializing ICU mode. ICU page is 4.
     {
       current_page = 4;
       mylcd.Fill_Screen(BLACK);
       icu_mode();
       back_button();
     }
  
     else if ((current_page == 2) && (is_pressed(5, 265, 120, 315, px, py))) //If you're in Operating modes page and you want to go back to the main menu(LOG FILES, ALARMS, OPERATING MODES), press the MENU button.
     {
       current_page = 1;
       menu1();
     }
  
     else if ((current_page == 3) && (is_pressed(5, 265, 65, 315, px, py))) // When the back button is pressed in surgery mode, go back to operating modes(ICU MODE & SURGERY MODE).
     {
       current_page = 2;
       operating_modes();
       menu_button();
     }
  
     else if (current_page == 3)
     {
       if (serial == false)
       {
        serial = true;
        Surgery();
       }
       
     }
  
     else if ((current_page == 4) && (is_pressed(5, 265, 65, 315, px, py))) //When the back button is pressed in ICU mode, go back to operating modes(ICU MODE & SURGERY MODE).
     {
       current_page = 2;
       operating_modes();
       menu_button();
     }
  
     else if ((current_page == 4) && (is_pressed(5, 70, 475, 215, px, py))) // Displaying ICU parameters
     {
       Print();
     }
  
  
     else if ((current_page == 4) && (is_pressed(345, 270, 475, 310, px, py))) //When the START BUTTON is pressed in the ICU MODE page, print the Time. START BUTTON page is 5.
     {
       current_page = 5;
       startWindow();
       back_button();
     }
  
     else if (current_page == 5) //Once the START BUTTON is pressed, deliver the medicine to the patients and if the back button is pressed
     {
      deliverMedicine();
      if (is_pressed(5, 265, 65, 315, px, py))
      {
        mylcd.Fill_Screen(BLACK);
        current_page = 4;
        icu_mode();
        menu_button();
      }
     }
  
    else if ((current_page == 1) && (is_pressed(362, 65, 475, 105, px, py))) //When the ALARMS button is pressed, Print the Alarms. The page for Alarms is 6.
     {
       current_page = 6;
       mylcd.Fill_Screen(BLACK);
       mylcd.Set_Text_Size(3.0);
       mylcd.Print_String("ALARMS", 160, 140);
       back_button();
     }
  
    else if ((current_page == 6) && (is_pressed(5, 265, 65, 315, px, py))) //If the back button is pressed in the alarms page, go back to the main menu.
     {
       current_page = 1;
       menu1();
     }
  
    else if ((current_page == 1) && (is_pressed(362, 125, 475, 165, px, py)))
     {
       current_page = 7;
       mylcd.Fill_Screen(BLACK);
       mylcd.Set_Text_Size(3.0);
       mylcd.Print_String("LOG FILE", 160, 140);
       back_button();
     }
  
    else if ((current_page == 7) && (is_pressed(5, 265, 65, 315, px, py))) //If the back button is pressed in the Log file page, go back to the main menu.
     {
       current_page = 1;
       menu1();
     }
  
}

void Surgery()
{ 
  value = analogRead(knob);
  value = map(value, 0, 1023, 0, 255);
  if (value <= 126)
   {
    mylcd.Print_String("Hypotension", 255, 170);
    digitalWrite(led_surgery, HIGH);
    buzz();
    
    for (int i=0; i<32000; i++)    
    {
      digitalWrite(DIR,LOW);
      digitalWrite(ENA,HIGH);
      digitalWrite(PUL,HIGH);
      delayMicroseconds(50);
      digitalWrite(PUL,LOW);
      delayMicroseconds(50);
    }

    delay(3000);

    for (int i=0; i<32000; i++)    
    {
      digitalWrite(DIR,LOW);
      digitalWrite(ENA,HIGH);
      digitalWrite(PUL,HIGH);
      delayMicroseconds(50);
      digitalWrite(PUL,LOW);
      delayMicroseconds(50);
    }
    end_buzz();
   }

  else
  {
    digitalWrite(led_surgery, LOW);
  }
}

void Clock()
{
  DateTime now = rtc.now();

  if (yesTime == false)
   {
    yesTime = true;
   }

    Day = now.day();
    MMonth = now.month();
    YYear = now.year();
    HHour = now.hour();
    MMinute = now.minute();
    Second = now.second();
    quote = "/";
    space = " ";
    colon = ":";

    clockstring1 = Day + quote + MMonth + quote + YYear;
    clockstring2 = HHour + colon + MMinute + colon + Second;
   
    mylcd.Print_String(clockstring1, 166.7, 80);
    mylcd.Print_String(clockstring2, 166.7, 120);
    
}

void KeyPad() {
 char Key = kpd.getKey();
 if (Key != NO_KEY)  
 {
  inputString += Key;
 }

 if ((inputString.length() == 32) && (truth == false))
  {
    //Row1
    code1 = inputString.charAt(0);
    code2 = inputString.charAt(1);
    medicineCode = code1 + code2;
    IMedicineCode = medicineCode.toInt();
    date1 = inputString.charAt(2);
    date2 = inputString.charAt(3);
    date = date1 + date2;
    IDate = date.toInt();
    Month1 = inputString.charAt(4);
    Month2 = inputString.charAt(5);
    Month = Month1 + Month2;
    IMonth = Month.toInt();
    Year1 = inputString.charAt(6);
    Year2 = inputString.charAt(7);
    Year3 = inputString.charAt(8);
    Year4 = inputString.charAt(9);
    Year = Year1 + Year2 + Year3 + Year4;
    IYear = Year.toInt();
    Hour1 = inputString.charAt(10);
    Hour2 = inputString.charAt(11);
    Hour = Hour1 + Hour2;
    IHour = Hour.toInt();
    Minute1 = inputString.charAt(12);
    Minute2 = inputString.charAt(13);
    Minute = Minute1 + Minute2;
    IMinute = Minute.toInt();
    Flow1 = inputString.charAt(14);
    Flow2 = inputString.charAt(15);
    Flow = Flow1 + Flow2;
    IFlow = Flow.toInt();
    //End of Row1

    //Row2
    code12 = inputString.charAt(16);
    code22 = inputString.charAt(17);
    medicineCode3 = code12 + code22;
    IMedicineCode2 = medicineCode3.toInt();
    date12 = inputString.charAt(18);
    date22 = inputString.charAt(19);
    date3 = date12 + date22;
    IDate2 = date3.toInt();
    Month12 = inputString.charAt(20);
    Month22 = inputString.charAt(21);
    Month3 = Month12 + Month22;
    IMonth2 = Month3.toInt();
    Year12 = inputString.charAt(22);
    Year22 = inputString.charAt(23);
    Year32 = inputString.charAt(24);
    Year42 = inputString.charAt(25);
    Year5 = Year12 + Year22 + Year32 + Year42;
    IYear2 = Year5.toInt();
    Hour12 = inputString.charAt(26);
    Hour22 = inputString.charAt(27);
    Hour3 = Hour12 + Hour22;
    IHour2 = Hour3.toInt();
    Minute12 = inputString.charAt(28);
    Minute22 = inputString.charAt(29);
    Minute3 = Minute12 + Minute22;
    IMinute2 = Minute3.toInt();
    Flow12 = inputString.charAt(30);
    Flow22 = inputString.charAt(31);
    Flow3 = Flow12 + Flow22;
    IFlow2 = Flow3.toInt();
    //End of Row2

    quote = "/";
    space = " ";
    colon = ":";

    clockstring3 = date + quote + Month + quote + Year;
    clockstring4 = Hour + colon + Minute;
    clockstring5 = date3 + quote + Month3 + quote + Year5;
    clockstring6 = Hour3 + colon + Minute3;
    
    truth = true;
  }
}

void deliverMedicine()
{
  DateTime now = rtc.now(); 
  Day_ICU = now.day();
  MMonth_ICU = now.month();
  YYear_ICU = now.year();    
  HHour_ICU = now.hour();
  MMinute_ICU = now.minute();
  quote = "/";
  space = " ";
  colon = ":";

  clockstring7 = Day_ICU + quote + MMonth_ICU + quote + YYear_ICU;
  clockstring8 = HHour_ICU + colon + MMinute_ICU;
  
  if ((now.day() == IDate) && (now.month() == IMonth) && (now.year() == IYear) && (now.hour() == IHour) && (now.minute() == IMinute))
  {
    flowRate1 = IFlow;
    medcode1 = IMedicineCode;
    motion_ICU1();
    mylcd.Print_String(clockstring7, 15, 15);
    mylcd.Print_String(clockstring8, 15, 55);
    space = " ";
    clockstringCombined1 = clockstring7 + space + clockstring8;
    if (just_once == false)
     {
      Serial.println(clockstringCombined1);
      just_once = true;
     }
    
  }

 if ((now.day() == IDate2) && (now.month() == IMonth2) && (now.year() == IYear2) && (now.hour() == IHour2) && (now.minute() == IMinute2))
  {
    flowRate2 = IFlow2;
    medcode1 = IMedicineCode2;
    motion_ICU2();
    mylcd.Print_String(clockstring7, 15, 125);
    mylcd.Print_String(clockstring8, 15, 155);  // mylcd.Draw_Rectangle(5, 110, 350, 180);
    clockstringCombined1 = clockstring7 + space + clockstring8;
    if (just_once == false)
     {
      Serial.println(clockstringCombined1);
      just_once = true;
     }
  }
}

void motion_ICU1()
{ 
  if (Once1 == false)
  {
    buzz();
    delay(100);
    for (int i=0; i<32000; i++)    
     {
      digitalWrite(d,LOW);
      digitalWrite(e,HIGH);
      digitalWrite(p,HIGH);
      delayMicroseconds(50);
      digitalWrite(p,LOW);
      delayMicroseconds(50);
     }
  
    delay(3000);
  
    for (int i=0; i<32000; i++)    
     {
      digitalWrite(d,LOW);
      digitalWrite(e,HIGH);
      digitalWrite(p,HIGH);
      delayMicroseconds(50);
      digitalWrite(p,LOW);
      delayMicroseconds(50);
     }
    
     digitalWrite(led_surgery, LOW);
     end_buzz();
     Once1 = true;
  }
}

void motion_ICU2()
{ 
  if (Once2 == false)
  {
    digitalWrite(led_surgery, HIGH);
    buzz();
    for (int i=0; i<32000; i++)    
     {
      digitalWrite(dir,LOW);
      digitalWrite(enable,HIGH);
      digitalWrite(pulse,HIGH);
      delayMicroseconds(50);
      digitalWrite(pulse,LOW);
      delayMicroseconds(50);
     }
  
    delay(300);
  
    for (int i=0; i<32000; i++)    
     {
      digitalWrite(dir,LOW);
      digitalWrite(enable,HIGH);
      digitalWrite(pulse,HIGH);
      delayMicroseconds(50);
      digitalWrite(pulse,LOW);
      delayMicroseconds(50);
     }
    
     digitalWrite(led_surgery, LOW);
     end_buzz();
     Once2 = true;
  }
}

void Print()
{
  mylcd.Set_Text_Size(2.0);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Draw_Rectangle(5, 225, 400, 260);
  //Printing on TFT screen
  mylcd.Print_String(inputString, 10, 235);
  //Row1
  mylcd.Print_String(medicineCode, 50, 95);
  mylcd.Print_String(clockstring3, 165, 85);
  mylcd.Print_String(clockstring4, 165, 115);
  mylcd.Print_String(Flow, 380, 95);
  //Row2
  mylcd.Print_String(medicineCode3, 50, 165);
  mylcd.Print_String(clockstring5, 165, 165);
  mylcd.Print_String(clockstring6, 165, 185);
  mylcd.Print_String(Flow3, 380, 165);
}

void buzz()
{
  tone(buzzer, 100); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
}

void end_buzz ()
{
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(100);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(100);        // ...for 1sec
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(100);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(100);        // ...for 1sec
}
