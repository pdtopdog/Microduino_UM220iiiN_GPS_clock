#include "U8glib.h"
#include <MemoryFree.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  

String inputString = ""; 
boolean stringComplete = false;
const int LOCAL_AREA = +8;

struct GNZDA{
  String UTC_Time;
  String localTime;
  int hour;
  int minute;
  int second; 
  String year;
  String month;
  String day;
}gps;

     
void setup() { 
  Serial.begin(115200);
  while(!Serial1){
    ;
  }
  Serial1.begin(9600);  
  }

     
void loop() {
        serialEvent();
        if(stringComplete){
        //Serial.println(inputString);            
        if (inputString.startsWith("$GNZDA")){                
         String UTC_Time = inputString.substring(7,17);                                                                        
         String hour=inputString.substring(7,9);                         
         String minute = inputString.substring(9,11);         
         String second = inputString.substring(11,13);
         String day = inputString.substring(18,20);                                 
         String month = inputString.substring(21,23);         
         String year=inputString.substring(24,28);          
         gps.hour = hour.toInt();
         gps.UTC_Time = hour + ':' + minute + ':' + second;            
         int localHour = (gps.hour + LOCAL_AREA) % 24 ; // 转化成区域时间，北京为东八时区，utc小时+8即为区域时间。和24取余数         
         
         String localHourStr = String(00 + localHour);// int 转换成 string 可以写成 string = ""+ int i          
         localHourStr = localHourStr.substring(0, 2); // 区域时间的字符串
         gps.localTime = gps.UTC_Time;
         gps.localTime.replace(gps.localTime.substring(0,2),localHourStr);//区域时间小时字符串替换utc小时字符串
         //Serial.println(gps.localTime);
                             
          u8g.firstPage();            
         do {  
         u8g.setFont(u8g_font_9x18B);
         u8g.setPrintPos(5,15); 
         u8g.print("BEIJING TIME:");        
         u8g.setPrintPos(15,35); 
         u8g.print(day+'-'+month+'-'+year);
         u8g.setFont(u8g_font_fub20);
         u8g.setPrintPos(5,60);          
         u8g.print(gps.localTime);  
         }  
         while( u8g.nextPage() );                                                                                    
     }              
     inputString = "";
     stringComplete = false;
     void free(GNZDA);     
  }
}

void serialEvent() {
   while (Serial1.available()>0){     
     char inChar =(char) Serial1.read();               
               inputString +=inChar;                                       
          if (inChar == '\n') {      
      stringComplete = true;
     }      
   }       
 }
