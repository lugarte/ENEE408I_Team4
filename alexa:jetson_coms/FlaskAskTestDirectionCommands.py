# Listens to commands from Alexa, sends command to robot over serial
# Test version of code that sends serial command 'n' over serial to arduino, prompting robot to navigate around the room

import serial
#import queue as qu
#import threading as th
#import multiprocessing as mp
import time


from flask import Flask
from flask_ask import Ask, statement

app = Flask(__name__)
ask = Ask(app, '/')

#cvQueue = mp.JoinableQueue()


@ask.intent('MoveIntent')
def navigate():
	speech_text = 'Navigating...'
#        cvQueue.put("Move");
        ser.write(b'n')
	return statement(speech_text).simple_card('bay max', speech_text)


@ask.intent('HaltIntent')
def halt():
	speech_text = 'Halting...'
	ser.write(b'h')
	return statement(speech_text).simple_card('bay max', speech_text)

@ask.intent('FollowIntent')
def follow():
	speech_text = 'Following Ball...'
#	ser.write(b'h')
	return statement(speech_text).simple_card('bay max', speech_text)

@ask.intent('IdentifyIntent')
def identify():
	speech_text = 'Identifying Person...'
#	ser.write(b'd')
	return statement(speech_text).simple_card('bay max', speech_text)

#@ask.intent('OrderGroceries')
#def order_groceries():
#	speech_text = 'Ordering groceries...'
#	ser.write(b'o')
#	return statement(speech_text).simple_card('Mary', speech_text)

#@ask.intent('EnforceSocialDistancing')
#def enforce_quarantine():
#	speech_text = 'Enforcing social distancing...'
#	ser.write(b's')
#	return statement(speech_text).simple_card('Mary', speech_text)

if __name__ == '__main__':
	ser = serial.Serial('/dev/ttyACM0', 9600)
	app.run()
