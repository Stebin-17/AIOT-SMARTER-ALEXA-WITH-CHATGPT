# **AIOT-SMARTER-ALEXA-WITH-CHATGPT**

## **INTRODUCTION:**

Imagine a world where you don't need to lift a finger to control the ambiance of your home. With the help of cutting-edge technology, this dream is now a reality. Introducing "Smarter Alexa with ChatGPT for Controlling LED Lights," a project that revolutionizes the way we interact with our smart homes. This project combines the power of Amazon's Alexa and ChatGPT, a state-of-the-art language model, to create a seamless and intuitive experience for controlling LED lights. By using natural language processing, this project allows users to simply speak their commands to Alexa, who then communicates with ChatGPT to accurately interpret and execute the desired actions.

To implement this project, a Raspberry Pi 4 is used as a server to communicate with the Alexa Skill Set. The Raspberry Pi 4 is a powerful microcomputer capable of running multiple programs simultaneously, making it an ideal choice for handling the complex communication between Alexa and ChatGPT. Additionally, the project utilizes the Wiznet-EVB-PICO-5100s board, which is connected to both the LED lights and an 8*8 dot matrix. This board acts as a bridge between the Raspberry Pi and the LED lights, allowing for easy control and manipulation of the light's color and intensity. With the Wiznet-EVB-PICO-5100s board, the LED lights can be synced to music, set to change color depending on the time of day, or even programmed to display scrolling messages on the dot matrix.

## **HARDWARES USED:**

- [RASPBERRY PI 4 MODEL B](https://www.hackster.io/raspberry-pi/products/raspberry-pi-4-model-b)
- [W5100S-EVB-PICO](https://www.hackster.io/wiznet/products/w5100s-evb-pico1)
- [JUMPER WIRES](https://www.hackster.io/diyables/products/jumper-wires)
- [LED](https://www.hackster.io/search?q=led&i=parts)
- [AMAZON ECHO-DOT](https://www.hackster.io/amazon-alexa/products/echo-dot)
- [LED DOT-MATRIX](https://www.hackster.io/diyables/products/led-matrix-4-in-1-32x8)

## **SOFTWARE AND SERVICES USED:**

- [AMAZON ALEXA SKILLS KIT](https://developer.amazon.com/en-US/alexa)
- [FLASK-ASK](https://github.com/johnwheeler/flask-ask)
- [MQTT SERVICE](https://mqtt.org/)
- [NGROK](https://ngrok.com/)

## **PROGRAMMING LANGUAGES**
- C
- PYTHON

## **WORKFLOW**

The workflow of the project "Smarter Alexa with ChatGPT for turning on and off the LED and displaying the same in 8*8 dot matrix" is an impressive combination of advanced technologies and cutting-edge solutions. The project begins when the user operates Alexa using the invocation name "device." The commands "Alexa ask device"/"Alexa open device" will trigger the subsequent intent block that has been designed using the Alexa Skill Set.

There is a dedicated Flask server for handling the requests from Alexa, and the endpoint from Alexa is tunneled to the server running the Raspberry Pi 4 using the ngrok service. The message or phrase that is similar or sentimentally close to either "turn on" or "turn off" commands will be sent to the particular intent block, which will have a block of ChatGPT API for converting the phrase into either ON/OFF. The message from the user will be mapped to either turn on or turn off based on the polarity and sentiment of the statement. The response obtained from the ChatGPT will be in the form of a JSON file, which will be parsed, and the text value will be obtained. The resultant text will be sent to the MQTT broker with a dedicated subscription. The message from ChatGPT will be published on the server.

Here, an online broker with port:1883 and HOST:'54.87.92.106' will be used. The endpoint connection will be a Wiznet board attached with Ethernet and LED, and dot matrix. The board will be subscribed to the same topic as the message is being published from the Flask server in Raspberry Pi. Based on the message, the LED will turn on and off, and the text will be displayed on the dot matrix from the MQTT server. Alexa will respond back with turning on or off based on the action performed in the end device, which is the Wiznet board. The entire workflow is smooth and seamless, allowing users to control their smart home devices with just their voice. The integration of Alexa, ChatGPT, Flask, Raspberry Pi, MQTT, and Wiznet board creates an intelligent and responsive system that makes our lives easier and more comfortable.

### **FLOWCHART**

<p align="center">
  <img src="https://user-images.githubusercontent.com/114398468/220548143-fc611e24-86da-42d5-9f46-214e4612127e.jpg" />
</p>

## **STEPS**
1. 





