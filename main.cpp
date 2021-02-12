#include<p18f458.h>
//#define XTAL_FREQ 4000000
#define ldata PORTB
#define GREEN PORTCbits.RC2
#define BLUE PORTCbits.RC3
#define MOT PORTCbits.RC6
#define mot PORTDbits.RD1
#define RS PORTDbits.RD7
#define E PORTDbits.RD6
void message(const char*s);
void cmd(unsigned char data);
void DATA(unsigned char data);

void Delay(unsigned char itime){
    char a,b
    for(a=kp;a>='0';a--){
        for(b=kp;b>='0';b--)
    
}
void switch_off_cursor()
{
   cmd(0x0c);
}
void clear_lcd()
{
   cmd(0x01);
}
void Initialise_LCD()
{
   cmd(0x38);
   Delay(10);
   cmd(0x0E);
   Delay(10);
  // cmd(0x01);
  // DATA('H');
}
void cmd(unsigned char data)
{
  ldata=data;
  RS=0;
  E=1;
  Delay(1);
  E=0; 
}
void DATA(unsigned char data)
{
  ldata=data;
  RS=1;
  E=1;
  Delay(10);
  E=0; 
}
void Delay(unsigned char itime)
{
   int i,j;
   for(i=0;i<itime;i++)
      for(j=0;j<165;j++);
}
int main()
{
  // Initialise_LCD();
  char  n[13];
  float SensorValue,previous,present;
  TRISC=0;
  TRISB=0x00;
  TRISD=0;
  clear_lcd();
  Initialise_LCD();
  message("something");
  cmd(0xc0);
  message("DESIGN 2019");
  Delay(500);Delay(500);Delay(500);Delay(500);
   
  while(1)
  {
      TRISAbits.TRISA0=1;
      ADCON0=0x81; /*select chanel 0, activate ADC*/
      ADCON1=0xC0; /*use AVDD as Vref(+) and AVSS as Vref(-)*/
   
      Delay(1); //give ADC initialisation time
      ADCON0bits.GO=1;//Start ADC
      while(ADCON0bits.GO_DONE==1);
      //monitor the end of conversion
      SensorValue=(ADRESL*100)/255;
      //store lower byte of ADRES reg
      present=SensorValue;
      sprintf(n, "%.0f", SensorValue);
       
      clear_lcd();
      message("SOILMoisture(%):");
      cmd(0xc0);
      message(n);
      message(" %");
      previous=SensorValue;
      Delay(500);
      if( SensorValue<47)
      {
          //MOT=1;
          clear_lcd();
          message("Soil is dry");
          cmd(0xc0);
          message("Watering......");
          mot=1;GREEN=BLUE=0 ;MOT=1;
          Delay(500);Delay(500);
          clear_lcd();
          message("SOILMoisture(%):");
          cmd(0xc0);
          message(n);
          message(" %");
          previous=SensorValue;
          Delay(500);Delay(500);
          //  MOT=1;
       }
       if(SensorValue>=47&&SensorValue<=54)
       {
           clear_lcd();
           message("Soil is normal");
           cmd(0xc0);
           message("Stop watering");
           mot=0;GREEN=1;BLUE=0;MOT=0;
           Delay(100);Delay(100);Delay(100);Delay(100);Delay(100);Delay(100);
           clear_lcd();
           message("SOILMoisture(%):");
           cmd(0xc0);
           message(n);
           message(" %");
           previous=SensorValue;
           Delay(500);Delay(500);
       }
       if(SensorValue>54)
       {
           clear_lcd();
           message("Soil is wet");
           cmd(0xc0);
           message("Stop watering");
           mot=0;GREEN=0;BLUE=1;MOT=0;
           Delay(500);Delay(500);
           clear_lcd();
           message("SOILMoisture(%):");
           cmd(0xc0);
           message(n);
           message(" %");
           previous=SensorValue;
           Delay(500);Delay(500);
           
       }
    }
    return 0;
}