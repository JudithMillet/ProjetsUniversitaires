# TP Region Growing & Edge Detection - MIF17
### 2021 - 2022


## Authors
- Emie LAFOURCADE 11802635
- Judith MILLET 11811933

<br/>

## How to install

This project run on Linux.

<br/>

### Requirement

You will need to install these packages to execute the project :
- cmake
- OpenCV (please find the instruction on this link  https://opencv.org/)

<br/>

### Compilation

Open a terminal into the project and follow these commands :
```
~$ mkdir build && cd build
~$ cmake ..
~$ make
```

<br/>

### Run part 1

Enter the following command to run :
``` 
~$ ./RegionGrowing <image_data_path> [options]
```
<br/>

There are some parameters in option :
```
- [-nbSeeds N] is the numbers of seeds (100 by default).

- [-tolerance T] is a tolerance rate (80% by default).

- [-threshold H] is the similarity threshold of pixels (5 by default).

- [-smoothing] is to smooth the output image.

- [-threads] to use threads.

- [-outline] to show outlines.
```

### Run part 2

Enter the following command to run :
``` 
~$ ./Contours <image_data_path> [options]
```
<br/>

There are some parameters in option :
```
- [-mask M] is a mask between 0 = Prewitt, 1 = Sobel, 2 = Kirsch (0 by default).

- [-threshold H] is convolution between 0 and 255 (80 by default).

- [-multi] is the multidirectional method used.

- [-global] is the global threshold used.

- [-refining] in the refining used.
```
