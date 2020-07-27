#include <SoftwareSerial.h>

SoftwareSerial gprs(2, 3); // RX, TX pins of the SIM800 chip

String FirebaseCloudFunctionURL="http://xxxxxx.cloudfunctions.net/write?device=Noun_of_the_Field_you_want_to_write_your_Data_to";


int8_t answer;
int onModulePin= 4;       //PWR key pin for the 800 chip

String Data ;             //Variable to Store sensor data into  



void setup() {
      
      pinMode(onModulePin, OUTPUT);
        digitalWrite(onModulePin,HIGH);
         delay(3000);
        digitalWrite(onModulePin,LOW);
    gprs.begin(9600);

    Serial.begin(9600);
   Serial.println(F("Starting..."));
   gprs.listen();
    power_on();
   gprs.stopListening();  
    delay(3000);
     
    Serial.println(F("Setup Complete"));

}

void loop() {
/*
    
    write some code to read your data from sensors and then put in the data Variable (String)
   
   */
   Transfert_data (Data); //send Data to Firebase
   delay(10000);          
    


}


//############################## functions ##################################
bool Transfert_data (String Data){

   Serial.println(F("Data used for firebase transfert :"));
     Serial.println(Data);

      
  
bailout :
   sendATcommand2("AT+HTTPTERM ",",OK,","ERROR",2000);
    // Selects Single-connection mode
    if (sendATcommand2("AT+CIPMUX=0", "OK", "ERROR", 1000) == 1)
    {
        // Waits for status IP INITIAL
        while(sendATcommand2("AT+CIPSTATUS", "INITIAL", "", 500)  == 0 );
        delay(5000);

        // Sets the APN, user name and password
        if (sendATcommand2("AT+CSTT= “APN”, “User Name”, “PassWord”", "OK",  "ERROR", 30000) == 1)
        {
            // Waits for status IP START
            while(sendATcommand2("AT+CIPSTATUS", "START", "", 500)  == 0 );
            delay(5000);

            // Brings Up Wireless Connection
            if (sendATcommand2("AT+CIICR", "OK", "ERROR", 30000) == 1)
            {
                // Waits for status IP gprsACT
                while(sendATcommand2("AT+CIPSTATUS", "gprsACT", "", 500)  == 0 );
                delay(5000);

                // Gets Local IP Address
                if (sendATcommand2("AT+CIFSR", ".", "ERROR", 10000) == 1)
                {
                    // Waits for status IP STATUS
                    while(sendATcommand2("AT+CIPSTATUS", "IP STATUS", "", 500)  == 0 );
                    delay(5000);
                    Serial.println(F("Openning TCP"));

                  
                   sendATcommand2("AT+SAPBR=1,1","OK","ERROR",2000);
                    
                    sendATcommand2("AT+HTTPINIT","OK","ERROR",2000);
                    
                     if(sendATcommand2("AT+HTTPPARA=\"CID\",1","OK","ERROR",2000)== 1)
                    {
                            
  String                    w="AT+HTTPPARA=""URL"",";
                      w+=FirebaseCloudFunctionURL;
                       w+= Data;
                        char* write=  w.c_str();

                                  
  
                              Serial.println(F("request:"));
                              Serial.println(w);
    
                          
                          sendATcommand2(write,"OK","ERROR",2000);
                          delay(500);
                        if (sendATcommand2("AT+HTTPACTION=0",",200,","ERROR",2000)==1){
                          Serial.println(F("Data was uploaded Seccessfully to Firebase"));
                          
                        }
                          delay(500);
                          sendATcommand2("AT+HTTPREAD","operation successful","ERROR",2000);
                         
                       delay(1500);

                       sendATcommand2("AT+HTTPTERM ",",OK,","ERROR",2000);

                       return 1 ;
                        


                      
                    }
                    else
                    {
                        Serial.println(F("Error openning the connection (Connection to Firebase Failed)"));
                            power_on();
                        goto bailout;
                        return 0 ;
                    }
                }
                else
                {
                    Serial.println(F("Error getting the IP address"));
                        power_on();
                    goto bailout;
                    return 0 ;
                }
            }
            else
            {
                 Serial.println(F("Error bring up wireless connection"));
                     power_on();
                 goto bailout;
                 return 0 ;
            }
        }
        else
        {
              Serial.println(F("Error setting the APN"));
                  power_on();
              goto bailout;
              return 0 ;
        }
    }
    else
    {
        Serial.println(F("Error setting the single connection"));
         sendATcommand2("AT+CIPSHUT", "OK", "ERROR", 10000);
         delay(1000);
            power_on();
        goto bailout;
        return 0 ;
    }

   sendATcommand2("AT+HTTPTERM ",",OK,","ERROR",2000);
    delay(1000);
}
//######################################################################################################################""

void power_on(){

    uint8_t answer=0;

    // checks if the module is started
    answer = sendATcommand2("AT", "OK", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
           digitalWrite(onModulePin,HIGH);
        delay(3000);
           digitalWrite(onModulePin,LOW);

        // waits for an answer from the module
        while(answer == 0){     // Send AT every two seconds and wait for the answer
            answer = sendATcommand2("AT", "OK", "OK", 2000);
        }
    }
  while( sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 1000)== 0 )
    {
      Serial.print(F("."));
    }
    Serial.println(F("Connected to Network (BTS) "));
}

//################################################################################################################################
int8_t sendATcommand2(char* ATcommand, char* expected_answer1,
        char* expected_answer2, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, ' ', 100);    // Initialize the string

    delay(100);

    while( gprs.available() > 0) gprs.read();    // Clean the input buffer

    gprs.println(ATcommand);    // Send the AT command


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(gprs.available() != 0){
            response[x] = gprs.read();
            x++;
            // check if the desired answer 1  is in the response of the module
            if (strstr(response, expected_answer1) != NULL)
            {
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            else if (strstr(response, expected_answer2) != NULL)
            {
                answer = 2;
            }
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));
    
    return answer;
}
