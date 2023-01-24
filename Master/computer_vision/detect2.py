import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
import time
from tkinter import *
from tkinter import ttk 
from PIL import Image, ImageTk
import expensive_func as exp
import cube_struct

def extract_cluster_features(cluster, error=1.1):
	features = {}
	features["cluster_set"] = cluster
	features["cluster"] = np.array(cluster)

	mean = np.mean(features["cluster"], axis=0)
	features["mean"] = mean
	test = features["cluster"]-mean
	test = test*test
	test = np.sum(test, axis=1)
	i = np.argmax(test)
	pa1 = (features["cluster"][i]-mean)*1.1
	features["PA1"] = pa1
	dest = np.array([10*pa1[1], -10*pa1[0]])+mean
	test = features["cluster"]-dest
	test = test*test
	test = np.sum(test, axis=1)
	i = np.argmin(test)
	pa2 = (features["cluster"][i]-mean)*1.1
	features["PA2"] = pa2
	
	features["side_1"] = [-features["PA1"][0] + features["PA2"][0], -features["PA1"][1] + features["PA2"][1]]
	features["side_2"] = [-features["PA1"][0] + (-features["PA2"][0]), -features["PA1"][1] + (-features["PA2"][1])]

	#random maths
	ac1 = features["PA1"][0]*features["side_1"][0] + features["PA1"][1]*features["side_1"][1]
	bc1 = features["PA2"][0]*features["side_1"][0] + features["PA2"][1]*features["side_1"][1]

	ac2 = features["PA1"][0]*features["side_2"][0] + features["PA1"][1]*features["side_2"][1]
	bc2 = -features["PA2"][0]*features["side_2"][0] - features["PA2"][1]*features["side_2"][1]

	lambda_1 = -bc1/(ac1-bc1)
	lambda_2 = -bc2/(ac2-bc2)

	features["axis_1"] = [features["PA1"][0]*lambda_1 + (1-lambda_1)*features["PA2"][0], features["PA1"][1]*lambda_1 + (1-lambda_1)*features["PA2"][1]]
	features["axis_2"] = [features["PA1"][0]*lambda_2 - (1-lambda_2)*features["PA2"][0], features["PA1"][1]*lambda_2 - (1-lambda_2)*features["PA2"][1]]
	features["error"] = error
	return features

def dot(a, b):
	return sum([a[i]*b[i] for i in range(len(a))])

def length(a):
	return (dot(a, a))**0.5

def normalize(a):
	l = length(a)
	return [c/l for c in a]

def distance_between_clusters(features_1, features_2, error=0.95):

	area1 = abs(features_1["side_1"][0]*features_1["side_2"][1] - features_1["side_1"][1]*features_1["side_2"][0])#cross product of two edges
	l1_1 = (features_1["PA1"][0]*features_1["PA1"][0] + features_1["PA1"][1]*features_1["PA1"][1])**0.5
	pa1_1 = (features_1["PA1"][0]/l1_1, features_1["PA1"][1]/l1_1)
	l2_1 = (features_1["PA2"][0]*features_1["PA2"][0] + features_1["PA2"][1]*features_1["PA2"][1])**0.5
	pa2_1 = (features_1["PA2"][0]/l2_1, features_1["PA2"][1]/l2_1)

	area2 = abs(features_2["side_1"][0]*features_2["side_2"][1] - features_2["side_1"][1]*features_2["side_2"][0])#cross product of two edges
	l1_2 = (features_2["PA1"][0]*features_2["PA1"][0] + features_2["PA1"][1]*features_2["PA1"][1])**0.5
	pa1_2 = (features_2["PA1"][0]/l1_2, features_2["PA1"][1]/l1_2)
	l2_2 = (features_2["PA2"][0]*features_2["PA2"][0] + features_2["PA2"][1]*features_2["PA2"][1])**0.5
	pa2_2 = (features_2["PA2"][0]/l2_2, features_2["PA2"][1]/l2_2)



	# area1 = abs(features_1["side_1"][0]*features_1["side_2"][1] - features_1["side_1"][1]*features_1["side_2"][0])#cross product of two edges
	# l1_1 = (features_1["PA1"][0]*features_1["PA1"][0] + features_1["PA1"][1]*features_1["PA1"][1])**0.5
	# pa1_1 = (features_1["PA1"][0]/l1_1, features_1["PA1"][1]/l1_1)
	# l2_1 = (features_1["PA2"][0]*features_1["PA2"][0] + features_1["PA2"][1]*features_1["PA2"][1])**0.5
	# pa2_1 = (features_1["PA2"][0]/l2_1, features_1["PA2"][1]/l2_1)

	# area2 = abs(features_2["side_1"][0]*features_2["side_2"][1] - features_2["side_1"][1]*features_2["side_2"][0])#cross product of two edges
	# l1_2 = (features_2["PA1"][0]*features_2["PA1"][0] + features_2["PA1"][1]*features_2["PA1"][1])**0.5
	# pa1_2 = (features_2["PA1"][0]/l1_2, features_2["PA1"][1]/l1_2)
	# l2_2 = (features_2["PA2"][0]*features_2["PA2"][0] + features_2["PA2"][1]*features_2["PA2"][1])**0.5
	# pa2_2 = (features_2["PA2"][0]/l2_2, features_2["PA2"][1]/l2_2)




	# return abs(abs(dot(pa1_1, pa2_1)) - abs(dot(pa1_2, pa2_2))) < 1-error

	alpha = (area1/area2)**0.5 #rapport des longeur des axes
	
	if(abs(pa1_1[0]*pa1_2[0] + pa1_1[1]*pa1_2[1]) > error):
		if(abs(pa2_1[0]*pa2_2[0] + pa2_1[1]*pa2_2[1]) > error):
			if(abs(l1_1/l1_2 - alpha) < 1-error and abs(l2_1/l2_2 - alpha) < 1-error):
				return True
		return False

	elif(abs(pa1_1[0]*pa2_2[0] + pa1_1[1]*pa2_2[1]) > error):
		if(abs(pa2_1[0]*pa1_2[0] + pa2_1[1]*pa1_2[1]) > error):
			if(abs(l1_1/l2_2 - alpha) < 1-error and abs(l2_1/l1_2 - alpha) < 1-error):
				return True
		return False

	return False

def compute_averadge_side_length(face, error=0.9):
	l1 =0
	l2 =0

	for features in face:
		l1_ = length(features["side_1"])
		s1_ = normalize(features["side_1"])
		l2_ = length(features["side_2"])
		s2_ = normalize(features["side_2"])

		s1_n = normalize(face[0]["side_1"])
		s2_n = normalize(face[0]["side_2"])

		if(abs(dot(s1_, s1_n)) > error and abs(dot(s2_, s2_n)) > error):
			l1+=l1_
			l2+=l2_
		elif(abs(dot(s1_, s2_n)) > error and abs(dot(s2_, s1_n)) > error):
			l1+=l2_
			l2+=l1_
		else:
			print("PROBLEME, l'ERREUR EST TROP PETITE POUR CALCUER LES COTER MOYEN")
	return l1/len(face), l2/len(face)




def close_to_cube(features):
	paralelepipede_area = abs(features["side_1"][0]*features["side_2"][1] - features["side_1"][1]*features["side_2"][0])#cross product of two edges

	l1 = (features["axis_1"][0]*features["axis_1"][0] + features["axis_1"][1]*features["axis_1"][1])**0.5
	l2 = (features["axis_2"][0]*features["axis_2"][0] + features["axis_2"][1]*features["axis_2"][1])**0.5

	axis1 = [features["axis_1"][0]/l1, features["axis_1"][1]/l1]
	axis2 = [features["axis_2"][0]/l2, features["axis_2"][1]/l2]

	c = features["cluster"] - features["mean"]

	test = np.absolute(np.sum(c*axis1, axis=1))
	A = test <= l1
	test = np.absolute(np.sum(c*axis2, axis=1))
	B = test <= l2
	G = A*B

	good = np.sum(G)
	wrong = G.shape[0] - good

	return max(0, (good-wrong)/paralelepipede_area)

def compute_score_clusters(features_clusters):
	for features in features_clusters:
		features["score"] = close_to_cube(features)

def plot_center(img, features_clusters, show_score = True, show_vector = True):
	for features in features_clusters:

		if(show_score):
			for x, y in features["cluster_set"]:
				img[y][x][0] = features["score"]
				img[y][x][1] = features["score"]
				img[y][x][2] = features["score"]
		
		if(show_vector):
			cv.circle(img, (int(features["mean"][0]), int(features["mean"][1])), 3, (255, 0, 255), 2)

			axis1 = features["axis_1"]
			axis2 = features["axis_2"]
			c1 = features["side_1"]
			c2 = features["side_2"]

			x_mean = features["mean"][0]
			y_mean = features["mean"][1]
			x_far = features["PA1"][0] + x_mean
			y_far = features["PA1"][1] + y_mean
			x_second_far = features["PA2"][0] + x_mean
			y_second_far = features["PA2"][1] + y_mean
			axis1 = features["axis_1"]
			axis2 = features["axis_2"]


			cv.line(img, (int(x_mean), int(y_mean)), (int(x_far), int(y_far)), (0, 255, 0), 1, cv.LINE_AA)
			cv.line(img, (int(x_mean), int(y_mean)), (int(2*x_mean - x_far), int(2*y_mean - y_far)), (0, 255, 0), 1, cv.LINE_AA)
			l = 20
			cv.line(img, (int(x_mean), int(y_mean)), (int(x_second_far), int(y_second_far)), (255, 0, 0), 1, cv.LINE_AA)
			cv.line(img, (int(x_mean), int(y_mean)), (int(2*x_mean - x_second_far), int(2*y_mean - y_second_far)), (255, 0, 0), 1, cv.LINE_AA)
			
			cv.line(img, (int(x_mean), int(y_mean)), (int(x_mean + axis1[0]), int(y_mean + axis1[1])), (255, 0, 255), 1, cv.LINE_AA)
			cv.line(img, (int(x_mean), int(y_mean)), (int(x_mean + axis2[0]), int(y_mean + axis2[1])), (255, 0, 255), 1, cv.LINE_AA)
			
			cv.line(img, (int(x_far), int(y_far)), (int(x_far + c1[0]), int(y_far + c1[1])), (0, 255, 255), 2, cv.LINE_AA)
			cv.line(img, (int(x_far), int(y_far)), (int(x_far + c2[0]), int(y_far + c2[1])), (0, 255, 255), 2, cv.LINE_AA)



def analyse_face(face, img, draw = True):
	ref = 0
	line1 = face[ref]["side_1"]
	line2 = face[ref]["side_2"]
	l1 = length(line1)
	l2 = length(line2)
	line1 = [c/l1 for c in line1]
	line2 = [c/l2 for c in line2]

	l1, l2 = compute_averadge_side_length(face, 0.6)

	rot = np.linalg.inv(np.array([[line1[0], line2[0]], [line1[1], line2[1]]]))


	center = face[ref]["mean"]
	coords = []
	global_coord = []
	if(draw):
		cv.line(img, (int(center[0]), int(center[1])), (int(line1[0]*150 + center[0]), int(line1[1]*150 + center[1])), (1, 0.5, 1), 2, cv.LINE_AA)
		cv.line(img, (int(center[0]), int(center[1])), (int(line2[0]*150 + center[0]), int(line2[1]*150 + center[1])), (1, 0.5, 1), 2, cv.LINE_AA)

	for features in face:
		v = features["mean"] - center
		global_coord.append(features["mean"])
		if(draw):
			cv.line(img, (int(center[0]), int(center[1])), (int(v[0] + center[0]), int(v[1] + center[1])), (0.5, 1, 1), 1, cv.LINE_AA)

		coords.append(rot@v)

	offset = np.array([min(c[0] for c in coords), min(c[1] for c in coords)])
	coords = [c-offset for c in coords]

	ratio = 1.4
	coords_int = [np.array([round(c[0]/(l1*ratio)), round(c[1]/(l2*ratio))], dtype=np.int32) for c in coords]
	

	#bound to avoid crash
	for i in range(len(coords_int)):
		for j in range(2):
			if(coords_int[i][j] < 0):
				return []
			if(coords_int[i][j] > 2):
				return []

	m_x = max(c[0] for c in coords_int)
	m_y = max(c[1] for c in coords_int)

	UNKNOW = 0
	KNOW = 1
	GUESS = 2

	square = [[UNKNOW for _ in range(3)] for _ in range(3)]
	for c in coords_int:
		square[c[0]][c[1]] = KNOW


	#guess along x axis
	if(m_x > 0):
		for i in range(m_x+1):
			for j in range(m_y+1):
				if(square[i][j] == 0 and (square[(i+1)%3][j] != 0 or square[(i+2)%3][j] != 0)):
					square[i][j] = GUESS
	#guess along y axis
	if(m_y > 0):
		for i in range(m_x+1):
			for j in range(m_y+1):
				if(square[i][j] == 0 and (square[i][(j+1)%3] != 0 or square[i][(j+2)%3] != 0)):
					square[i][j] = GUESS

	guess_centers = []
	# RANSAC on global_coord, coords_int
	if(len(coords) >= 4):
		M, mask = cv.findHomography(np.array(coords_int), np.array(global_coord), cv.RANSAC,5.0)

		for i in range(3):
			for j in range(3):
				if(square[i][j] == GUESS):
					v = np.array([i, j, 1])
					v = M@v
					v_x = v[0]/v[2]
					v_y = v[1]/v[2]
					guess_centers.append((v_x, v_y))
					if(draw):
						print("GUESS", v_x, v_y)
						cv.circle(img, (int(v_x), int(v_y)), 7, (200, 0, 200), 7)

				if(square[i][j] == KNOW and draw):
					v = np.array([i, j, 1])
					v = M@v
					v_x = v[0]/v[2]
					v_y = v[1]/v[2]

					cv.circle(img, (int(v_x), int(v_y)), 7, (200, 200, 200), 7)
	else:

		avr_x = 0
		avr_y = 0
		nb_x = 0
		nb_y = 0
		for i in range(len(coords)):
			avr_x+= coords[i][0]
			avr_y+= coords[i][1]

			nb_x += coords_int[i][0]
			nb_y += coords_int[i][1]

		avr_x /= max(1, nb_x)
		avr_y /= max(1, nb_y)

		rot_inv = np.linalg.inv(rot)
		for i in range(3):
			for j in range(3):
				if(square[i][j] == GUESS):
					c = np.array([i*avr_x, j*avr_y])
					c = c + offset
					c = rot_inv@c
					c = c + center
					guess_centers.append(c)
					if(draw):
						cv.circle(img, (int(c[0]), int(c[1])), 7, (200, 0, 200), 7)

				if(square[i][j] == KNOW and draw):
					c = np.array([i*avr_x, j*avr_y])
					c = c + offset
					c = rot_inv@c
					c = c + center

					cv.circle(img, (int(c[0]), int(c[1])), 7, (200, 200, 200), 7)

	for c in guess_centers:
		face.append(c)
	return guess_centers


def resize(img, prop) :
    width, height = img.size
    new_height = prop
    new_width = int(width * new_height / height)
    img = img.resize((new_width,new_height))
    return img

def webcam():
	global fenetre, video
	_, frame = video.read()
	frame = cv.flip(frame, 1)
	cv2image = cv.cvtColor(frame, cv.COLOR_BGR2RGBA)
	img = Image.fromarray(cv2image)
	img = resize(img,300)
	imgtk = ImageTk.PhotoImage(image=img)
	label_input.imgtk = imgtk
	label_input.configure(image=imgtk)
	label_input.after(10, webcam)

########## START ##########

isImage = False
isVideo = False

### Init
if isImage :
	input_path = 'data/m_1_.png'
	total_frames = 10000
	frame_frequency = 2000
else :
	if isVideo :
		input_path = 'data/cube_source.mp4'
		# Open the video
		video = cv.VideoCapture(input_path)
	else :
		# Webcam
		video = cv.VideoCapture(0)

	# Get the total number of frames
	total_frames = int(video.get(cv.CAP_PROP_FRAME_COUNT))

	fps = video.get(cv.CAP_PROP_FPS)
	# Define the extraction frequency
	frame_frequency = int(2*fps)


freq = 0

def main() :
	global video, fenetre, total_frames, frame_frequency, freq, isImage, isVideo, input_path

	output_path = "data/frame_result.jpg"

	if isImage :
		print("load image", end="", flush=True)
		img = cv.imread(input_path, cv.IMREAD_UNCHANGED)
	else :
		input_path = "data/frame.jpg"
		# Define the lecture position
		video.set(cv.CAP_PROP_POS_FRAMES, freq)
		# Play the actual frame
		ret, frame = video.read()
		# Record the frame
		cv.imwrite(input_path, frame)

		total_start = time.time()
		start = time.time()

		print("load image", end="", flush=True)
		img = frame

	height, width, channel = img.shape

	final_image = img.astype(np.float32)
	final_image = final_image/255
	final_image = np.zeros(img.shape, dtype=np.single)

	total_start = time.time()
	start = time.time()

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()


	print("turn gray", end="", flush=True)
	gray = cv.cvtColor(img,cv.COLOR_BGR2GRAY)

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("edge detection", end="", flush=True)
	red = img[:,:,2]
	green = img[:,:,1]
	blue = img[:,:,0]
	m = 11
	red = cv.GaussianBlur(red, (m,m), 0) 
	green = cv.GaussianBlur(green, (m,m), 0) 
	blue = cv.GaussianBlur(blue, (m,m), 0) 

	edges = cv.Canny(image=red, threshold1=0, threshold2=20)
	edges = edges+cv.Canny(image=green, threshold1=0, threshold2=20)
	edges = edges+cv.Canny(image=blue, threshold1=0, threshold2=20)

	kernel = np.ones((2,2),np.uint8)
	n = 5
	edges = cv.dilate(edges,kernel,iterations = n)
	edges = cv.erode(edges,kernel,iterations = n)

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("compute clusters", end="", flush=True)
	clusters = exp.compute_clusters(edges, img, 1000, 20000)

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("analize cluster", end="", flush=True)
	features_clusters = [extract_cluster_features(cluster) for cluster in clusters]

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("compute the proximity with paralelogramme", end="", flush=True)
	compute_score_clusters(features_clusters)

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("draw info on clusters", end="", flush=True)
	plot_center(final_image, features_clusters, True, False)

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("color good clusters", end="", flush=True)

	faces = []
	for features in features_clusters:
		for x, y in features["cluster_set"]:
			final_image[y][x][0] = features["score"]*255
			final_image[y][x][1] = features["score"]*255
			final_image[y][x][2] = features["score"]*255
		if(features["score"] > 0.8):
			if(len(faces) == 0):
				faces.append([features])
			else:
				found = False
				for i in range(len(faces)):
					if(distance_between_clusters(faces[i][0], features, 0.6)):
						faces[i].append(features)
						found = True
						break
				if(not found):
					faces.append([features])
		#remove face with only one square (they are artifact)
	faces = [face for face in faces if len(face)>1]

	colors = [(0, 200, 0), (200, 0, 0), (0, 0, 200), (200, 0, 200), (200, 200, 0), (0, 200, 200), (0.5, 200, 0.5), (200, 0.5, 0.5), (0.5, 0.5, 200)]
	for i in range(len(faces)):
		for features in faces[i]:
			for x, y in features["cluster_set"]:
				final_image[y][x][0] = colors[i][0]
				final_image[y][x][1] = colors[i][1]
				final_image[y][x][2] = colors[i][2]

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	print("draw info on good clusters", end="", flush=True)
	for face in faces:
		plot_center(final_image, face, False)

	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")
	start = time.time()

	# print("Search for hidden squares", end="", flush=True)
	for face in faces:
		analyse_face(face, final_image)


	end = time.time()
	print(" ("+str(int(1000*(end-start)))+"ms)")


	print("Total time of computation : "+str(int(1000*(time.time()-total_start)))+"ms")


	########## Gestion fenetre ##########
	cv.imwrite(output_path, final_image)

	Cube = cube_struct.Cube(faces, input_path, output_path)

	if isImage :
		# Load init image from a file
		img_source = Image.open(Cube.input_path)
		img_source = resize(img_source,300)
		source = ImageTk.PhotoImage(img_source)

		# Diplay the image in the interface window
		label_input.config(image=source)
		label_input.image = source
		
	elif isVideo :
		# Function : to display the video into the window
		def afficher_video():
			vid = cv.VideoCapture(input_path)
			ret, frame = vid.read()
			if ret:
				cv2image = cv.cvtColor(frame, cv.COLOR_BGR2RGBA)
				img = Image.fromarray(cv2image)
				img = resize(img,300)
				imgtk = ImageTk.PhotoImage(image=img)
				label_input.configure(image=imgtk)
				label_input.imgtk = imgtk
			fenetre.after(10, afficher_video)
		# Execute the function to display
		afficher_video()

	# Description
	label_title1.config(text="Image before processing")
	
	# Load the result image from a file
	img_traitee = Image.open(Cube.output_path)
	img_traitee = resize(img_traitee,300)
	traitee = ImageTk.PhotoImage(img_traitee)

	# Description
	label_title2.config(text="Image after processing")

	# Display the image onto the window
	label_output.config(image=traitee)
	label_output.image = traitee

	# Infos
	text = ("Informations : \n"
	        + f"\nNumber visible faces : {Cube.visible_faces}"
	        + f"\nNumber visible squares : {Cube.visible_squares}"
	        + f"\n\nNumber squares per face : ")

	for i in range(Cube.visible_faces) :
		text = text + f"\n - Number of visible squares on the face {i} : {Cube.nb_squares_per_face[i]}"

	# Update texts
	label.config(text=text)

	freq = freq + frame_frequency

	# Planification of the next update each 2 seconds
	fenetre.after(frame_frequency, main)



########## DISPLAY ##########

# Creation and resizing of the window
fenetre = Tk()
fenetre.title("Interface")
fenetre.geometry("1000x700")

frame1 = Frame(fenetre)
frame1.grid(row=0, column=0)

frame2 = Frame(fenetre)
frame2.grid(row=0, column=1)

# Creation labels
label_title1 = Label(frame1, text="Wait...")
label_title1.pack()
label_input = Label(frame1, image=None)
label_input.pack()
label_title2 = Label(frame1, text="Wait...")
label_title2.pack()
label_output = Label(frame1, image=None)
label_output.pack()

label = Label(frame2, text="Wait...")
label.pack()

# Exit button
bouton=Button(frame2, text="Exit", command=fenetre.quit)
bouton.pack()

# Planification of the next update each 2 seconds
fenetre.after(frame_frequency, main)

if not isVideo and not isImage :
	# Display webcam video frames
	label_input.after(10, webcam)


# Start the event window 
fenetre.mainloop()

if not isImage :
	# Close the video
	video.release()