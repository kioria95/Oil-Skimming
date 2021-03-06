char state; // To store input character received via BT.
//SKIMMING MOTOR
const int motorC1 =2;
const int motorC2 =3;

//WHIRLPOOL MOTOR
const int motorD1= 4;
const int motorD2= 5;
const int pwm=6;  //Speed control via P.W.M

//THRUSTERS
const int motorA1 =  8;
const int motorA2 =  9;
const int motorB1 =  10;
const int motorB2 =  11;

//ULTRASONIC
const int trigPin = 12;
const int echoPin = 13;
long duration;
float distanceCm;
float real_v;

//PWM VARIABLES
int y=0;
int z=0;
int t=0;

void setup() 
{
  Serial.begin(9600); 
 //MOTORS
 pinMode(motorA1, OUTPUT);
 pinMode(motorA2, OUTPUT); 
 pinMode(motorB1, OUTPUT);
 pinMode(motorB2, OUTPUT);
 pinMode(motorC1, OUTPUT);
 pinMode(motorC2, OUTPUT);
 pinMode(motorD1, OUTPUT);
 pinMode(motorD2, OUTPUT);
 pinMode(pwm, OUTPUT);

  //ULTRASONIC SENSOR
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT); 
}

void loop() 
{
 //Save income data to variable 'state'
    if(Serial.available())
    {     
      state = Serial.read();  
  /***********************THRUSTERS****************************/
  
   /***************************Speed Control*****************/   
    if(state == 'J') 
   {
      y=Serial.parseInt();
  
   } 

    /***********************Forward***************************/
  
  //If state is equal with letter 'F', boat will go forward!
   else if (state == 'F') 
   {
       //Right Propeller
       digitalWrite(motorA1, HIGH);        analogWrite(motorA2, y); 
       //Left Propeller
       digitalWrite(motorB1, HIGH);        analogWrite(motorB2, y); 
       
 
    }
  /***********************Forward Left***************************/
  
  //If state is equal with letter 'F', boat will go forward!
   else if (state == 'M') 
   {
       //Right Propeller
       digitalWrite(motorA1, HIGH);        analogWrite(motorA2, y); 
       //Left Propeller
       digitalWrite(motorB1, HIGH);        analogWrite(motorB2, 30); 
    }

 /***********************Forward Right***************************/
      //If state is equal with letter 'F', boat will go forward!
   else if (state == 'N') 
   {
       //Right Propeller
       digitalWrite(motorA1, HIGH);        analogWrite(motorA2,30); 
       //Left Propeller
       digitalWrite(motorB1, HIGH);        analogWrite(motorB2, y); 
    }
    /***********************Backward***************************/
  
  //If state is equal with letter 'F', boat will go forward!
   else if (state == 'B') 
   {
       //Right Propeller
       digitalWrite(motorA1, LOW);        analogWrite(motorA2, y); 
       //Left Propeller
       digitalWrite(motorB1, LOW);        analogWrite(motorB2, y); 
       
    }


      /***********************Backward Left*************************/
  //If state is equal with letter 'B', boat will go backward
    else if (state == 'O')
    {
          //Right Propeller
         digitalWrite(motorA1, LOW);      analogWrite(motorA2, y);
         //Left Propeller
         digitalWrite(motorB1, LOW);      analogWrite(motorB2,30); 
    }

       /***********************Backward Right*************************/
  //If state is equal with letter 'B', boat will go backward
    else if (state == 'P')
    {
          //Right Propeller
         digitalWrite(motorA1, LOW);      analogWrite(motorA2, 30); 
         //Left Propeller
         digitalWrite(motorB1, LOW);      analogWrite(motorB2, y); 
    }
  

    /***********************Left***************************/
  
  //If state is equal with letter 'F', boat will go forward!
   else if (state == 'L') 
   {
       //Right Propeller
       digitalWrite(motorA1, HIGH);        analogWrite(motorA2, y);    
    }


        /***********************Right***************************/
  
  //If state is equal with letter 'F', boat will go forward!
   else if (state == 'R') 
   {
       //Left Propeller
       digitalWrite(motorB1, HIGH);       analogWrite(motorB2, y); 
       
    }

    /***************************Stop**************************/
  //If state is equal with letter 'S', wheels will turn right
    else if (state == 'S')
    {
        //Right Propeller
       digitalWrite(motorA1, LOW);        digitalWrite(motorA2, LOW); 
        //Left Propeller 
       digitalWrite(motorB1, LOW);        digitalWrite(motorB2, LOW);
    }

/***************************SKIMMER AND WHIRLPOOL MOTOR*****************************/

    else if(state == 'X') 
   {
      z=Serial.parseInt();
   } 


   else if(state=='A')
   
   {
   //Skimmer Motor
 digitalWrite(motorC1, HIGH);
 digitalWrite(motorC2, LOW);

 //Whirlpool Motor
 digitalWrite(motorD1, HIGH);
 digitalWrite(motorD2, LOW);
 analogWrite(pwm,z);
   }

   
   else if(state=='a')
 {
  //Skimmer Motor
 digitalWrite(motorC1, LOW);
 digitalWrite(motorC2, LOW);

 //Whirlpool Motor
 digitalWrite(motorD1, LOW);
 digitalWrite(motorD2, LOW);
 analogWrite(pwm,z);
 }
   /***************************Ultrasonic Sensor*****************************/
   else if(state =='U')
    {
      t=1;
      ultrasonic();                 //Call Ultrasonic function
    }
    }
}


void ultrasonic()
{
  while(t==1)
{
/***************************Receiving data from Ultrasonic Sensor*****************************/
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= (float)duration*0.034/2;

if(distanceCm>4)
{
Serial.print("*LR0G0B0*"); //Turn Ultrasonic LED off
}

/***************************Oil Maximum Capacity Reached*****************************/
if(distanceCm<4)
{
Serial.print("*LR255G0B0*");  //Turn Ultrasonic LED On
 //Skimmer Motor
 digitalWrite(motorC1, LOW);
 digitalWrite(motorC2, LOW);

 //Whirlpool Motor
 digitalWrite(motorD1, LOW);
 digitalWrite(motorD2, LOW);
   
Serial.println("*YSkim Motor OFF* /n"); 
return;                         //Return to Void Loop
}

/***************************Displaying Ultrasonic Data*****************************/
real_v=700-(distanceCm*100);
Serial.println("*T" +String(real_v,1)+"*");

if(Serial.available() > 0)
{     
 state = Serial.read();
 
/***************************If thruster button pressed while skimming*****************************/  
if( state == 'F' | state == 'B' | state == 'R' | state == 'L' )
   {
   Serial.print("*OR255G0B0*");//Turn Motor LED Off
   t=0;  
 //Skimmer Motor
 digitalWrite(motorC1, LOW);
 digitalWrite(motorC2, LOW);

 //Whirlpool Motor
 digitalWrite(motorD1, LOW);
 digitalWrite(motorD2, LOW);
  return;                         //Return to Void Loop
   }
   
  else if(state == 'X') 
   {
      z=Serial.parseInt();
   } 

/***************************Skimming Process*****************************/  
   else if (state == 'K' && distanceCm>4) //collect 300ml
    {
       //Skimmer Motor
      digitalWrite(motorC1, HIGH);
      digitalWrite(motorC2, LOW);

      //Whirlpool Motor
      digitalWrite(motorD1, HIGH);
      digitalWrite(motorD2, LOW); 
      analogWrite(pwm,z); //set speed of whirlpool motor   
      
      Serial.println("*YSkim Motor ON*");
    } 
}
}
}



  
  
