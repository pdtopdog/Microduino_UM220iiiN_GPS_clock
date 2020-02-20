#include "U8glib.h"
#include <MemoryFree.h>;   //https://github.com/sudar/MemoryFree.git


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  

String inputString = ""; 
boolean stringComplete = false;
const int LOCAL_AREA = 8;

struct GNZDA{
  int Hour;
  int Year;
  int Month;
  int Day;
  String UTC_Time;
  String localTime;
  String hour;                         
  String minute;         
  String second;    
  String year;
  String month;
  String day;      
}gps;

     
void setup() { 
  Serial.begin(115200);
  while(!Serial1){           //必须这样写否则oled不显示。
    ;
  }
  Serial1.begin(9600);  
  }

     
void loop() {
        serialEvent();
        if(stringComplete){
       // Serial.println(inputString);            
        if (inputString.startsWith("$GNZDA")){                
         String UTC_Time = inputString.substring(7,17);                                                                        
         String hour=inputString.substring(7,9);                         
         String minute = inputString.substring(9,11);         
         String second = inputString.substring(11,13);
                 
         gps.Hour = hour.toInt();                
         gps.UTC_Time = hour + ':' + minute + ':' + second;            
         int localHour = (gps.Hour + LOCAL_AREA) % 24 ; // 转化成区域时间，北京为东八时区，utc小时+8即为区域时间。和24取余数         
         
         String localHourStr = String(00 + localHour);// int 转换成 string 可以写成 string = ""+ int i          
         localHourStr = localHourStr.substring(0, 2); // 区域时间的字符串
         gps.localTime = gps.UTC_Time;
         gps.localTime.replace(gps.localTime.substring(0,2),localHourStr);//区域时间小时字符串替换utc小时字符串
         //Serial.println(gps.localTime);

         String day = inputString.substring(18,20);                                 
         String month = inputString.substring(21,23);         
         String year=inputString.substring(24,28);  
         gps.Day = day.toInt(); 
         gps.Month = month.toInt();         
         gps.Year = year.toInt(); 
         
        boolean LEAP_YEAR = gps.Year % 400 == 0 || gps.Year % 100 != 0 && gps.Year % 4 == 0; //闰年标志位
         
         int GPS_Hour_Tmp = gps.Hour + LOCAL_AREA;
         
         switch(gps.Month)
         {
            case 1:
                if(GPS_Hour_Tmp >= 24)
              {                   
                   gps.Day += 1;
                   if(gps.Day > 31)
                {
                   gps.Day -= 31;
                   gps.Month += 1;
                 }
              }
            break;
            case 2:
                if(GPS_Hour_Tmp >= 24)
             {                  
                   gps.Day += 1;
                   if( LEAP_YEAR = 1)
                 { 
                   if(gps.Day > 29)
                 {
                   gps.Day -= 29;
                   gps.Month += 1;  
                  }
               }
               else
              {
                 
                 if(gps.Day > 28)
                 {
                    gps.Day -= 28;
                    gps.Month += 1;
                  }
            
               }
             }
             break;      
             case 3:
                   if(GPS_Hour_Tmp >= 24)
                 {                     
                      gps.Day += 1;
                      if(gps.Day > 31)
                    {
                       gps.Day -= 31;
                       gps.Month += 1;
                     }
                  }
               break;
               case 4:
               if(GPS_Hour_Tmp >= 24)
              {                   
                    gps.Day += 1;
               if(gps.Day > 30)
                {
                      gps.Day -= 30;
                      gps.Month += 1;
                 }
               }
               break;

               case 5:
                if(GPS_Hour_Tmp >= 24)
              {                
                  gps.Day += 1;
                  if(gps.Day > 31)
                     {
                         gps.Day -= 31;
                         gps.Month += 1;
                      }
                }
                break;

               case 6:
                 if(GPS_Hour_Tmp >= 24)
               {                 
                  gps.Day += 1;
                  if(gps.Day > 30)
                  {
                   gps.Day -= 30;
                   gps.Month += 1;
                  }
                }
                break;

                case 7:
                if(GPS_Hour_Tmp >= 24)
               {               
                 gps.Day += 1;
                 if(gps.Day > 31)
                  {
                    gps.Day -= 31;
                    gps.Month += 1;
                   }
                 }
                 break;

                case 8:
                if(GPS_Hour_Tmp >= 24)
               {                   
                   gps.Day += 1;
                 if(gps.Day > 31)
                 {
                    gps.Day -= 31;
                    gps.Month += 1;
                  }
                 }
                break;

                case 9:
                if(GPS_Hour_Tmp >= 24)
                {                  
                   gps.Day += 1;
                   if(gps.Day > 30)
                   {
                      gps.Day -= 30;
                      gps.Month += 1;
                    }
                   }
                  break;

                  case 10:
                  if(GPS_Hour_Tmp >= 24)
                  {                     
                     gps.Day += 1;
                   if(gps.Day > 31)
                   {
                      gps.Day -= 31;
                      gps.Month += 1;
                    }
                   }
                   break;
      
                   case 11:
                  if(GPS_Hour_Tmp >= 24)
                  {                    
                     gps.Day += 1;
                   if(gps.Day > 30)
                   {
                     gps.Day -= 30;
                     gps.Month += 1;
                    }
                   }
                   break;

                   case 12:
                   if(GPS_Hour_Tmp >= 24)
                   {                     
                      gps.Day += 1;
                    if(gps.Day > 31)
                    {
                       gps.Day -= 31;
                       gps.Month = 1;
                       gps.Year += 1;
                     }
                    }
                    break;
            default:
            break;
            }

            /* 
            * 基姆拉尔森计算公式
            * W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
            */
            if(gps.Month ==1||gps.Month ==2) {
             gps.Month +=12;
             gps.Year--;
             }
             int Weekday=(gps.Day+2*gps.Month +3*(gps.Month +1)/5+gps.Year+gps.Year/4-gps.Year/100+gps.Year/400)%7;  
              
          u8g.firstPage();            
         do {  
         u8g.setFont(u8g_font_9x18B);
         u8g.setPrintPos(5,15); 
         u8g.print("BEIJING TIME:");        
         u8g.setPrintPos(5,35);    //年显示定位，先水平，后垂直。
         u8g.print(gps.Year);
         u8g.setPrintPos(50,35); 
         u8g.print(gps.Month);
         u8g.setPrintPos(70,35); 
         u8g.print(gps.Day);        
                   
          u8g.setPrintPos(90,35);    
          switch(Weekday){
          case 0: u8g.print( "MON");  break;
          case 1: u8g.print( "TUE");  break;
          case 2: u8g.print( "WED");  break;
          case 3: u8g.print( "THUR"); break;
          case 4: u8g.print( "FRI");  break;
          case 5: u8g.print( "SAT");  break;
          case 6: u8g.print( "SUN");  break;
          default: break;
         }   

          u8g.setFont(u8g_font_fub20);
          u8g.setPrintPos(8,60);          
          u8g.print(gps.localTime);
            
         }  
         while( u8g.nextPage() );                                                                                    
     } 
                  
     inputString = "";
     stringComplete = false;
     boolean LEAP_YEAR = false;
     int GPS_Hour_Tmp = 0;
     memoryCheck();
     //Serial.println(getFreeMemory());        
  }
}

void serialEvent() {
   while (Serial1.available()>0){     
     char inChar =(char) Serial1.read();               
               inputString +=inChar;                                       
          if (inChar == '\n') {      //等价于，不是等于，否则后续不能分离字符串。         
      stringComplete = true;      
     }      
   }       
 }
