# import the necessary packages
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time
import os

def main():
	# construct the argument parse and parse the arguments
	ap = argparse.ArgumentParser()
	ap.add_argument("-v", "--video",
		help="path to the (optional) video file")
	ap.add_argument("-n", "--name",
		help="name of person in the video")
	args = vars(ap.parse_args())

	# if a video path was not supplied, grab the reference
	# to the webcam
	if not args.get("video", False):
		vs = VideoStream(src=0).start()
	# otherwise, grab a reference to the video file
	else:
		vs = cv2.VideoCapture(args["video"])
	# allow the camera or video file to warm up
	time.sleep(2.0)

	frame_count = 1
	pic_num = 1


	#Create the subdirectory for the new pictures
	print("Arg = ", args.get("name", False))
	home = os.getcwd()
	path = home + "\dataset\\" + args.get("name",False)

	try:
		os.mkdir(path)
	except OSError:
		print ("Creation of the directory %s failed" % path)
	else:
		print ("Successfully created the directory %s " % path)

	while True:
		frame = vs.read()
		# handle the frame from VideoCapture or VideoStream
		#frame = frame[1] if args.get("video", False) else frame
		# if we are viewing a video and we did not grab a frame,
		# then we have reached the end of the video
		cv2.imshow("Frame", frame)
		
		if frame is None:
			break
			
		if frame_count % 60 == 0:
			image_name = path + "\\frame" + str(pic_num) + ".jpg"
			print(image_name)
			cv2.imwrite(image_name, frame)
			pic_num = pic_num + 1
		
		frame_count = frame_count + 1
		key = cv2.waitKey(1) & 0xFF
		# if the 'q' key is pressed, stop the loop
		if key == ord("q"):
			break
		time.sleep(.05)	
		
	# if we are not using a video file, stop the camera video stream
	if not args.get("video", False):
		vs.stop()
	# otherwise, release the camera
	else:
		vs.release()
	# close all windows
	cv2.destroyAllWindows()
	print("Done!, Got: ", str(pic_num), " pictures")
if __name__ == "__main__":
    main()