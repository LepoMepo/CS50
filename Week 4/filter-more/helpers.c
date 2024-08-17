#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int new_colour = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float)3);
            image[i][j].rgbtBlue = new_colour;
            image[i][j].rgbtGreen = new_colour;
            image[i][j].rgbtRed = new_colour;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int temp_Blue;
            int temp_Green;
            int temp_Red;

            temp_Blue = image[i][j].rgbtBlue;
            temp_Green = image[i][j].rgbtGreen;
            temp_Red = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - (j+1)].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - (j+1)].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - (j+1)].rgbtRed;

            image[i][width - (j+1)].rgbtBlue = temp_Blue;
            image[i][width - (j+1)].rgbtGreen = temp_Green;
            image[i][width - (j+1)].rgbtRed = temp_Red;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int sum_blue, sum_green, sum_red;
    int counter;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum_blue = sum_green = sum_red = 0;
            counter = 0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (k + i < 0 || k + i > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        continue;
                    }
                    sum_blue += image[i+k][j+l].rgbtBlue;
                    sum_green += image[i+k][j+l].rgbtGreen;
                    sum_red += image[i+k][j+l].rgbtRed;

                    counter++;
                }
            }
            temp[i][j].rgbtBlue = round(sum_blue / (float)counter);
            temp[i][j].rgbtGreen = round(sum_green / (float)counter);
            temp[i][j].rgbtRed = round(sum_red / (float)counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int Gx[3][3] = {{-1,0,1}, {-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1}, {0,0,0},{1,2,1}};
    int Gx_Blue, Gy_Blue, Gx_Green, Gy_Green, Gx_Red, Gy_Red;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Gx_Blue = Gy_Blue = Gx_Green = Gy_Green = Gx_Red = Gy_Red = 0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (k + i < 0 || k + i > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        continue;
                    }
                    Gx_Blue += image[i+k][l+j].rgbtBlue * Gx[k + 1][l + 1];
                    Gx_Green += image[i+k][l+j].rgbtGreen * Gx[k + 1][l + 1];
                    Gx_Red += image[i+k][l+j].rgbtRed * Gx[k + 1][l + 1];

                    Gy_Blue += image[i+k][l+j].rgbtBlue * Gy[k + 1][l + 1];
                    Gy_Green += image[i+k][l+j].rgbtGreen * Gy[k + 1][l + 1];
                    Gy_Red += image[i+k][l+j].rgbtRed * Gy[k + 1][l + 1];
                }
            }
            temp[i][j].rgbtBlue = fmin(round(sqrt(Gx_Blue * Gx_Blue + Gy_Blue * Gy_Blue)), 255);
            temp[i][j].rgbtGreen = fmin(round(sqrt(Gx_Green * Gx_Green + Gy_Green * Gy_Green)), 255);
            temp[i][j].rgbtRed = fmin(round(sqrt(Gx_Red * Gx_Red + Gy_Red * Gy_Red)), 255);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }

    return;
}
