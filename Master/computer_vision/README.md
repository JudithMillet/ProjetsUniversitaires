# M2 - ATIV - Rubik's Cube Project

## Authors
* Sam Bekkis
* Océane Charlery
* Pierre Hubert-Brierre
* Judith Millet

## Demonstration

To launch the demonstration from your computer, you can follow the following instructions :
<br/>


- Install the following dependencies (you may install them with pip, pip3 or homebrew depending on your environment) : tkinter, opencv, cython 
- Open a terminal window located at the root project of the project and run the following commands (you may run "python3" instead of "py" for the first command)  : 
```
python3 setup.py build_ext --inplace
python3 detect2.py
```
- Place your Rubik's Cube in front of your computer's webcam and watch the face and square detection displayed in real time on the interface. 
  

<br/>
Since our program is based on edge detection, watch out for rectangular objects in the background such as ceiling lights, as these could disturb the detection. The Rubik's Cube should be relatively close to the webcam to get the best results.