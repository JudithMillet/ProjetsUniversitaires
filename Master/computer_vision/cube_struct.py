import cv2 as cv

class Cube :
    visible_faces = 0
    visible_squares = 0
    nb_squares_per_face = [0, 0, 0, 0, 0, 0] # nombre de cubes par face
    faces = []
    color_faces = {0: [[0,0,0],[0,0,0]]}

    input_path = ""
    output_path = ""

    def find_color(self, pixel) :
        img = cv.imread(self.input_path, cv.IMREAD_UNCHANGED)

        red = img[pixel[1]][pixel[0]][0]
        green = img[pixel[1]][pixel[0]][1]
        blue = img[pixel[1]][pixel[0]][2]

        print("not implemented")

    def __init__(self, faces, input_path, output_path) -> None:
        self.visible_faces = len(faces)
        self.faces = faces
        self.input_path = input_path
        self.output_path = output_path

        for i in range(len(faces)) :
            self.nb_squares_per_face[i] = len(faces[i])
        
        self.visible_squares = sum(self.nb_squares_per_face)