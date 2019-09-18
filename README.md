# GPRS-Firebase
Send data to firebase cloud using SIM800 (GPRS Module) and Atmega328p (Arduino) .
before using this code , you have to upload cloud functions to your firebase acount , to do so please follow this steps 

1-install node JS on your computer , this is the link for the download "https://nodejs.org/en/"

2-after downloading and installing Node Js , u need to open command prompt and write the following commands 
       *npm install -g firebase-tools
       *firebase login
3-Go to firebase project directory that u created on your computer 

4-Back to command prompt where you will enter the following command and then follow the orders 
       *firebase init functions

5-Now after completing this command, you have to go to the project directory when u will find "index.js" File , 
open it using subleme text or any other IDE, and copy the functions file that you will find in this repository into it .

6-After modifying the file , you need to save it and then go back to the command prompt where you will enter the following command 
        *firebase deploy --only functions

7-Enjoy :p :v
