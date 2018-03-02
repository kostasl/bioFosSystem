/*******************MG-811 Gas Sensor Module sourcingmap module with temp. comp. reading*****************************
Author: klagogia

************************************************************************************/

/************************PIN DEFINITIONS************************************/
#define         MG_PIN                       A0     //define which analog input channel you are going to use
#define         TCM_PIN                      A1     //define which analog input channel Reads the temperature Compensation
#define         BOOL_PIN                     2
#define         LED_PIN                      13 //PWR LED Pin
#define         DC_GAIN                      (6.25)   //define the DC gain of amplifier


/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (2)    //define the time interval(in milisecond) between each samples in normal operation
#define         READ_SAMPLE_TIMES            (23)     //define how many samples you are going to take in normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (320.00) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTAGE             (1.0) //define the voltage drop of the sensor when move the sensor from air into 10000ppm CO2



/*****************************Globals***********************************************/
float           CO2Curve[3]  =  {2.60, ZERO_POINT_VOLTAGE, 0.025  };
                                                     //two points are taken from the curve. 
                                                     //with these two points, a line is formed which is
                                                     //"approximately equivalent" to the original curve.
                                                     // data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280) 
                                                     //slope = ( reaction voltage ) / (log400 –log1000) 

void setup()
{
    Serial.begin(9600);                              //UART setup, baudrate = 9600bps
    pinMode(BOOL_PIN, INPUT);                        //set pin to input
    digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors

    pinMode(LED_PIN, OUTPUT); //NANO Switch Off LED
    digitalWrite(LED_PIN, LOW);                    //turn on pullup resistors


    Serial.print("MG-811 Recoding\n");                
    Serial.print("time(ms) Volts CO2 VTemp Temp\n");                
}

void loop()
{
    char buff[110];
    
    long unsigned int ts; //timestamp
           
    long unsigned int percentage;
    float temperature;
    float MG811volts;
    float Tempvolts;

    ts = millis();
    MG811volts = AvgRead(MG_PIN);
    Tempvolts  = AvgRead(TCM_PIN);
    percentage = MGGetPercentage(MG811volts,CO2Curve);
    temperature = GetTemp(Tempvolts);
    
    sprintf(buff,"%ld %d.%d %ld %d.%d %d.%d",ts, (int)MG811volts, (int)(MG811volts*100)%100, percentage,(int)Tempvolts,(int)(Tempvolts*100)%100,(int)temperature,(int)(temperature*10)%10);
    Serial.println( buff );
    
    delay(1);
}

/*****************************  MGRead *********************************************
Input:   mg_pin - analog channel
Remarks: This function reads the analog output of the MG811 Module
************************************************************************************/ 
float AvgRead(int mg_pin)
{
    int i;
    float v=0.0;

    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *5.3/1024.0 ;
    return v;  
}

/*****************************  MQGetPercentage **********************************
Input:   volts   - Sourcingmap module output measured in volts
         pcurve  - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(MG-811 output) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
************************************************************************************/ 
long unsigned int  MGGetPercentage(float volts, float *pcurve)
{
//   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
//      return -1;
//   } else { 
      return pow(10, (pcurve[1]-1000.0*(volts/DC_GAIN))*pcurve[2]+pcurve[0]);
//   }
return volts;
}

float  GetTemp(float volts)
{
//   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
//      return -1;
//   } else { 
      //Slope Measured using external Digital thermometer
      return ((volts)*(30.0-27.0)/(3.65-3.28));
//   }
return volts;
}
