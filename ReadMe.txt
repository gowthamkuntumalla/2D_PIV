OpenCV library is needed.
image1 & image2 need to be present in the same folder as the executable(.exe) if running that file or it should be in the same folder as the (.cbp) project file while running it through IDE.  

This initial version uses the algorithm mentioned in the presentation "PIV Evaluation - A Brief Review".

The data is obtained in the text file "data.txt" ,Output data file consists of initial point(x1,y1) final point(x2,y2).
The output image is obtained in the "result_img.tif" file.

The data obtained is NOT free of Rogue vectors.(as is obviuos from result_img)
Also FFT is NOT used in this version. 

Open the header file(2dalgo.hpp) and main.cpp to change the window sizes etc. If it crashes revert back to the original data.
NOTE:- Image names in the main.cpp file should be same as names(& image type) present in the folder.