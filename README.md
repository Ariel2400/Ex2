# Ex2

## Part 1 - Matrix
using the implementation of Tal, we delegated the relevant methods to the .so
implementation of the matrix.

## Part 2 - BMP 
we divided all the diffrent parts to 3 struct and a matrix of pixels. then
we read the relevant parts to the structs and with the information we manipulated 
the pixels. for the program to parse different BMP formats, all we need to do is and create the vector 
with the new dimensions( if we want it to support 32-bit per pixel, we need to extend the matrix to 
the matching dimensions and cange the PIXEL_OFFSET to 4)
we aren't really famillier with formats such as jpeg do we don't know how to adjust our project
for parsing these files.