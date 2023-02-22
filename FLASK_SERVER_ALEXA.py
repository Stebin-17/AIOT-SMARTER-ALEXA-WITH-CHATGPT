import telegram
from telegram.ext import Updater,MessageHandler
import logging
import os
import paho.mqtt.client as mqtt
from flask import Flask
from flask_ask import Ask, request, session, question, statement
import RPi.GPIO as GPIO
import os
import openai
import json
import urllib.request
import requests
import urllib
from urllib.request import urlopen
from datetime import datetime
import threading


app = Flask(__name__)
ask = Ask(app, "/")
logging.getLogger('flask_ask').setLevel(logging.DEBUG)

@ask.intent('chatgpt',mapping={'user_question':'question'})
def chatgpt(user_question,room):
    first_client = mqtt.Client()
    first_client.connect('54.87.92.106',1883)

    if session.attributes.get('stop'):
        session.attributes['stop'] = False
        return statement("Goodbye")
    else:
        openai.api_key = "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"
        chat=user_question
        print(chat)
        if 'turn on lights' in chat.lower():
            first_client.publish("home/alexa","TURN ON LIGHTS")
            return statement("turning on lights")

        elif 'turn  off light' in chat.lower():
            first_client.publish("home/alexa","TURN OFF LIGHTS")
            return statement("turning off lights")
        respond = openai.Completion.create(model="text-davinci-003",
                                           prompt="Convert \"" + chat + "\" to any one among below commands :\n- Turn off light  \n- Turn on light\n",
                                           temperature=0,max_tokens=100,
                                           top_p=1,
                                           frequency_penalty=0.2,
                                           presence_penalty=0 )
        chat1="Convert \"" + chat + "\" to any one among below commands :\n- Turn off light  \n- Turn on light"
        print(chat1)
        print(respond)
        text=respond['choices'][0]['text']
        print("the response from chatgpt is {}".format(text))
        
        if "Turn on light" in text or "brightness" in text:
            first_client.publish("home/alexa","TURN ON LIGHTS")
            return statement("turning on lights")
        
        elif "Turn off light" in text or "dimmer" in text or "Dim" in text or "off" in text:
            first_client.publish("home/alexa","TURN OFF LIGHTS")
            return statement("turning off lights")
        
        elif "stop" in text or "exit" in text:
            return statement("Exiting the ChatGPT Skill")


@ask.intent("AMAZON.YesIntent")
def continue_chatgpt():
    return chatgpt(None, None)

@ask.intent("AMAZON.NoIntent")
def stop_chatgpt():
    return statement("Exiting the ChatGPT Skill")

@ask.launch
def launch():
    speech_text = 'Welcome to Innovation chat Automation'
    return question(speech_text).reprompt(speech_text).simple_card(speech_text)

 
@ask.intent('AMAZON.HelpIntent')
def help():
    speech_text = 'You can say hello to me!'
    return question(speech_text).reprompt(speech_text).simple_card('HelloWorld', speech_text)


@ask.intent('AMAZON.StopIntent')
def stop():
    return statement ("Exiting the skill")

@ask.session_ended
def session_ended():
    return "{}", 200

    
def start_flask_server():
    app.run(debug=True)

if __name__ == '__main__':
    if 'ASK_VERIFY_REQUESTS' in os.environ:
        verify = str(os.environ.get('ASK_VERIFY_REQUESTS', '')).lower()
    
        if verify == 'false':
            app.config['ASK_VERIFY_REQUESTS'] = False
    app.run(debug=True)










