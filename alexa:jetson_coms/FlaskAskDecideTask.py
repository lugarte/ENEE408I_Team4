# Listens to commands from Alexa, sends command to robot over serial

import serial

from flask import Flask
from flask_ask import Ask, statement

app = Flask(__name__)
ask = Ask(app, '/')

@ask.intent('EnforceQuarantine')
def enforce_quarantine():
	speech_text = 'Enforcing Quarantine'
	ser.write(b'e')
	return statement(speech_text).simple_card('Mary', speech_text)

@ask.intent('HandWashReminder')
def hand_wash_reminder():
	speech_text = 'Setting handwashing reminder...'
	ser.write(b'h')
	return statement(speech_text).simple_card('Mary', speech_text)

@ask.intent('DetectFall')
def detect_fall():
	speech_text = 'Looking out for falls...'
	ser.write(b'd')
	return statement(speech_text).simple_card('Mary', speech_text)

@ask.intent('OrderGroceries')
def order_groceries():
	speech_text = 'Ordering groceries...'
	ser.write(b'o')
	return statement(speech_text).simple_card('Mary', speech_text)

@ask.intent('EnforceSocialDistancing')
def enforce_quarantine():
	speech_text = 'Enforcing social distancing...'
	ser.write(b's')
	return statement(speech_text).simple_card('Mary', speech_text)

if __name__ == '__main__':
	ser = serial.Serial('/dev/cu.usbmodem1411', 9600)
	app.run()
