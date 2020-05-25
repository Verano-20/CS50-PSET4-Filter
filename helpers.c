#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Prototype functions
void SobelCheck(int *Sobel);
int Sobel(char colour, int i, int j, int height, int width, RGBTRIPLE copy[height][width]);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, av;
    float average;
    // Loop through each row
    for (i = 0; i < height; i++)
    {
        // Loop through each pixel in the row
        for (j = 0; j < width; j++)
        {
            // Calculate the average of the RGB values
            average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            // Assign each colour to be the average
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = round(average);
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    RGBTRIPLE temp;

    // Loop through each row
    for (i = 0; i < height; i++)
    {
        // For each pixel in the first half of the row (avoid swapping twice)
        for (j = 0; j < width / 2; j++)
        {
            // Assign the left pixel values to temp
            temp.rgbtBlue = image[i][j].rgbtBlue;
            temp.rgbtGreen = image[i][j].rgbtGreen;
            temp.rgbtRed = image[i][j].rgbtRed;

            // Assign the corresponding right pixel values to the left pixel
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;

            //Assign the temp values to the corresponding right pixel
            image[i][width - 1 - j].rgbtBlue = temp.rgbtBlue;
            image[i][width - 1 - j].rgbtGreen = temp.rgbtGreen;
            image[i][width - 1 - j].rgbtRed = temp.rgbtRed;
        }
    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, x, y;
    float tempR, tempG, tempB;

    // Allocate memory for copy of image
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    // check there is enough memory for copy
    if (copy == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }

    // For each row
    for (i = 0; i < height; i++)
    {
        //For each pixel
        for (j = 0; j < width; j++)
        {
            //Reset all temps
            tempB = 0;
            tempG = 0;
            tempR = 0;

            // If corner pixel
            if ((i == 0 && j == 0) || (i == height - 1 && j == 0) || (i == height - 1 && j == width - 1) || (i == 0 && j == width - 1))
            {
                // Loop through each pixel in the 9 x 9 grid
                for (x = j - 1; x <= j + 1; x++)
                {
                    for (y = i - 1; y <= i + 1; y++)
                    {
                        // If in the bounds of the image
                        if (x >= 0 && x < width && y >= 0 && y < height)
                        {
                            // Add colour values to temp
                            tempB += image[y][x].rgbtBlue;
                            tempG += image[y][x].rgbtGreen;
                            tempR += image[y][x].rgbtRed;
                        }
                    }
                }

                //Average the temp values
                tempB = tempB / 4.0;
                tempG = tempG / 4.0;
                tempR = tempR / 4.0;
            }

            // If edge pixel but not corner
            else if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                {
                    // Loop through each pixel in the 9 x 9 grid
                    for (x = j - 1; x <= j + 1; x++)
                    {
                        for (y = i - 1; y <= i + 1; y++)
                        {
                            // If in the bounds of the image
                            if (x >= 0 && x < width && y >= 0 && y < height)
                            {
                                // Add colour values to temp
                                tempB += image[y][x].rgbtBlue;
                                tempG += image[y][x].rgbtGreen;
                                tempR += image[y][x].rgbtRed;
                            }
                        }
                    }
                }
                //Average the temp values
                tempB = tempB / 6.0;
                tempG = tempG / 6.0;
                tempR = tempR / 6.0;
            }

            // If normal pixel
            else if (i > 0 && i < height - 1 && j > 0 && j < width - 1)
            {
                // Loop through each pixel in the 9 x 9 grid
                for (x = j - 1; x <= j + 1; x++)
                {
                    for (y = i - 1; y <= i + 1; y++)
                    {
                        // Add colour values to temp
                        tempB += image[y][x].rgbtBlue;
                        tempG += image[y][x].rgbtGreen;
                        tempR += image[y][x].rgbtRed;
                    }
                }
                // Average the temp values
                tempB = tempB / 9.0;
                tempG = tempG / 9.0;
                tempR = tempR / 9.0;
            }
            // Assign the colour values to a copy to keep clean reference
            copy[i][j].rgbtBlue = round(tempB), copy[i][j].rgbtGreen = round(tempG), copy[i][j].rgbtRed = round(tempR);
        }
    }

    // Copy all pixels in the copied image back into the original
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    //free memory for copy
    free(copy);

    return;
}



void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;

    // create a copy of image with a border of 1 black pixel around the whole image

    // Allocate memory for copy of image
    RGBTRIPLE(*copy)[width + 2] = calloc(height + 2, (width + 2) * sizeof(RGBTRIPLE));

    for (i = 0; i < height + 2; i++)
    {
        for (j = 0; j < width + 2; j++)
        {
            // if edge pixel, set to black
            if (i == 0 || j == 0 || i == height + 1 || j == width + 1)
            {
                copy[i][j].rgbtRed = copy[i][j].rgbtGreen = copy[i][j].rgbtBlue = 0;
            }
            //else, copy pixel from image[][]
            else
            {
                copy[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;
                copy[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
                copy[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
            }
        }
    }

    // apply sobel operator on each non-edge pixel in the copy and apply result to original image
    for (i = 1; i < height + 1; i++)
    {
        for (j = 1; j < width + 1; j++)
        {
            image[i - 1][j - 1].rgbtRed = Sobel('r', i, j, height + 2, width + 2, copy);
            image[i - 1][j - 1].rgbtGreen = Sobel('g', i, j, height + 2, width + 2, copy);
            image[i - 1][j - 1].rgbtBlue = Sobel('b', i, j, height + 2, width + 2, copy);
        }
    }

    // free memory for copy
    free(copy);

    return;
}

//Function to return result of sobel operator on a pixel
int Sobel(char colour, int i, int j, int height, int width, RGBTRIPLE copy[height][width])
{
    int GxR, GxG, GxB, GyR, GyG, GyB, SobelR, SobelG, SobelB;
    float R, G, B;
    GxR = GxB = GxG = GyR = GyG = GyB = 0;

    switch (colour)
    {
        //Red
        case 'r':
            GxR += copy[i - 1][j - 1].rgbtRed * (-1);
            GxR += copy[i - 1][j + 1].rgbtRed;
            GxR += copy[i][j - 1].rgbtRed * (-2);
            GxR += copy[i][j + 1].rgbtRed * 2;
            GxR += copy[i + 1][j - 1].rgbtRed * (-1);
            GxR += copy[i + 1][j + 1].rgbtRed;

            GyR += copy[i - 1][j - 1].rgbtRed * (-1);
            GyR += copy[i - 1][j].rgbtRed * (-2);
            GyR += copy[i - 1][j + 1].rgbtRed * (-1);
            GyR += copy[i + 1][j - 1].rgbtRed;
            GyR += copy[i + 1][j].rgbtRed * 2;
            GyR += copy[i + 1][j + 1].rgbtRed;

            R = sqrt((GxR * GxR) + (GyR * GyR));

            SobelR = round(R);

            //check sobel is between 0 and 255
            SobelCheck(&SobelR);

            return SobelR;
            break;

        case 'g':
            GxG += copy[i - 1][j - 1].rgbtGreen * (-1);
            GxG += copy[i - 1][j + 1].rgbtGreen;
            GxG += copy[i][j - 1].rgbtGreen * (-2);
            GxG += copy[i][j + 1].rgbtGreen * 2;
            GxG += copy[i + 1][j - 1].rgbtGreen * (-1);
            GxG += copy[i + 1][j + 1].rgbtGreen;

            GyG += copy[i - 1][j - 1].rgbtGreen * (-1);
            GyG += copy[i - 1][j].rgbtGreen * (-2);
            GyG += copy[i - 1][j + 1].rgbtGreen * (-1);
            GyG += copy[i + 1][j - 1].rgbtGreen;
            GyG += copy[i + 1][j].rgbtGreen * 2;
            GyG += copy[i + 1][j + 1].rgbtGreen;

            G = sqrt((GxG * GxG) + (GyG * GyG));

            SobelG = round(G);

            //check sobel is between 0 and 255
            SobelCheck(&SobelG);

            return SobelG;
            break;

        case 'b':
            GxB += copy[i - 1][j - 1].rgbtBlue * (-1);
            GxB += copy[i - 1][j + 1].rgbtBlue;
            GxB += copy[i][j - 1].rgbtBlue * (-2);
            GxB += copy[i][j + 1].rgbtBlue * 2;
            GxB += copy[i + 1][j - 1].rgbtBlue * (-1);
            GxB += copy[i + 1][j + 1].rgbtBlue;

            GyB += copy[i - 1][j - 1].rgbtBlue * (-1);
            GyB += copy[i - 1][j].rgbtBlue * (-2);
            GyB += copy[i - 1][j + 1].rgbtBlue * (-1);
            GyB += copy[i + 1][j - 1].rgbtBlue;
            GyB += copy[i + 1][j].rgbtBlue * 2;
            GyB += copy[i + 1][j + 1].rgbtBlue;

            B = sqrt((GxB * GxB) + (GyB * GyB));

            SobelB = round(B);

            //check sobel is between 0 and 255
            SobelCheck(&SobelB);



            return SobelB;
            break;

    }

    printf("Invalid colour.\n");
    return 0;

}

// Function to limit a float's value to between 0 and 255 inclusive
void SobelCheck(int *Sobel)
{
    if (*Sobel < 0)
    {
        *Sobel = 0;
    }
    if (*Sobel > 255)
    {
        *Sobel = 255;
    }
}
