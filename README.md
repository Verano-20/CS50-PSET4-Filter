# CS50-PSET4-Filter
My solution for CS50x2020 problem set 4 - filter.

A program in C to apply filters to an image.

Takes as input a bmp image and one of four filters:

-b  Blur
-g  Grayscale
-r  Reflect
-e  Edges (Highlights edges according to Sobel Operator)

Example usage:

./filter -g images/yard.bmp out.bmp

This applies a grayscale filter to the yard.bmp file and outputs the result in out.bmp.
