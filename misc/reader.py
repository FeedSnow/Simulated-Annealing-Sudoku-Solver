import os 
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

import cv2
import numpy as np
from PIL import Image
from tensorflow.keras.models import load_model


def read(filename):
    
    dir = os.path.dirname(os.path.realpath(__file__))

    model = load_model(dir+'/sudoku_model.keras')

    folder=dir+"/images"
    #filename = input("Enter sudoku image name: ")
    sudoku_a = cv2.imread(folder+'/'+filename)

    sudoku_a = cv2.resize(sudoku_a, (450,450))
    
    def preprocess(image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) 
        blur = cv2.GaussianBlur(gray, (3,3),6) 
        threshold_img = cv2.adaptiveThreshold(blur,255,1,1,11,2)
        return threshold_img

    threshold = preprocess(sudoku_a)

    contour_1 = sudoku_a.copy()
    contour_2 = sudoku_a.copy()
    contour, hierarchy = cv2.findContours(threshold,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(contour_1, contour,-1,(0,255,0),3)

    def main_outline(contour):
        biggest = np.array([])
        max_area = 0
        for i in contour:
            area = cv2.contourArea(i)
            if area >50:
                peri = cv2.arcLength(i, True)
                approx = cv2.approxPolyDP(i , 0.02* peri, True)
                if area > max_area and len(approx) ==4:
                    biggest = approx
                    max_area = area
        return biggest ,max_area

    def reframe(points):
        points = points.reshape((4, 2))
        points_new = np.zeros((4,1,2),dtype = np.int32)
        add = points.sum(1)
        points_new[0] = points[np.argmin(add)]
        points_new[3] = points[np.argmax(add)]
        diff = np.diff(points, axis =1)
        points_new[1] = points[np.argmin(diff)]
        points_new[2] = points[np.argmax(diff)]
        return points_new

    def splitcells(img):
        rows = np.vsplit(img,9)
        boxes = []
        for r in rows:
            cols = np.hsplit(r,9)
            for box in cols:
                boxes.append(box)
        return boxes

    black_img = np.zeros((450,450,3), np.uint8)
    biggest, maxArea = main_outline(contour)
    if biggest.size != 0:
        biggest = reframe(biggest)
    cv2.drawContours(contour_2,biggest,-1, (0,255,0),10)
    pts1 = np.float32(biggest)
    pts2 = np.float32([[0,0],[450,0],[0,450],[450,450]])
    matrix = cv2.getPerspectiveTransform(pts1,pts2)
    imagewrap = cv2.warpPerspective(sudoku_a,matrix,(450,450))
    imagewrap =cv2.cvtColor(imagewrap, cv2.COLOR_BGR2GRAY)

    sudoku_cell = splitcells(imagewrap)

    def CropCell(cells):
        Cells_croped = []
        for image in cells:
            img = np.array(image)
            img = img[4:46, 6:46]
            img = Image.fromarray(img)
            Cells_croped.append(img)
        return Cells_croped

    sudoku_cell_croped= CropCell(sudoku_cell)

    sudoku_grid = []

    for cell in sudoku_cell_croped:
        img = np.array(cell)
        img = cv2.resize(img, (32, 32))
        img = img / 255.0
        img = img.reshape(1, 32, 32, 1)

        prediction = model.predict(img)
        predicted_digit = np.argmax(prediction)

        if np.max(img) < 0.1:
            sudoku_grid.append(0)
        else:
            sudoku_grid.append(predicted_digit)

    sudoku_array = np.array(sudoku_grid)
    print(sudoku_array)
    np.savetxt(dir+"/data.txt", sudoku_array, fmt='%d')
    return True