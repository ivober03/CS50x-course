#include "helpers.h"
#include <math.h>

// I checked a solution that i found on github for reference in some parts of the program.
// I understood the logic behind the program but in many cases my algorithms were inefficient and over complicated.

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int h = 0; h < height; h++)
    {
        for(int w = 0; w < width; w++)
        {
            int newValue = round((image[h][w].rgbtBlue + image[h][w].rgbtRed + image[h][w].rgbtGreen) / 3.0);
            image[h][w].rgbtBlue = image[h][w].rgbtGreen = image[h][w].rgbtRed = newValue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int h = 0; h < height; h++)
    {
        if(width % 2 == 0)
        {
            for(int w = 0; w < width / 2; w++)
            {
                RGBTRIPLE aux[height][width];
                aux[h][w] = image[h][w];
                image[h][w] = image[h][width - (w + 1)];
                image[h][width - (w + 1)] = aux[h][w];
            }
        }
        else
        {
            for(int w = 0; w < (width - 1) / 2; w++)
            {
                RGBTRIPLE aux[height][width];
                aux[h][w] = image[h][w];
                image[h][w] = image[h][width - (w + 1)];
                image[h][width - (w + 1)] = aux[h][w];
            }
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float newRed = 0.0;
            float newGreen = 0.0;
            float newBlue = 0.0;
            int cantPixels = 0;

            for (int rows = -1; rows < 2; rows++)
            {
                for (int cols = -1; cols < 2; cols++)
                {
                    if (h + rows < 0 || h + rows > height - 1)
                    {
                        continue;
                    }

                    if (w + cols < 0 || w + cols > width - 1)
                    {
                        continue;
                    }

                    newRed = newRed + image[h + rows][w + cols].rgbtRed;
                    newGreen = newGreen + image[h + rows][w + cols].rgbtGreen;
                    newBlue = newBlue + image[h + rows][w + cols].rgbtBlue;
                    cantPixels++;
                }
            }

            aux[h][w].rgbtRed = round(newRed / cantPixels);
            aux[h][w].rgbtGreen = round(newGreen / cantPixels);
            aux[h][w].rgbtBlue = round(newBlue / cantPixels);
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = aux[h][w].rgbtRed;
            image[h][w].rgbtGreen = aux[h][w].rgbtGreen;
            image[h][w].rgbtBlue = aux[h][w].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width];
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int GxRed = 0;
            int GxGreen = 0;
            int GxBlue = 0;
            int GyRed = 0;
            int GyGreen = 0;
            int GyBlue = 0;

            for (int rows = -1; rows < 2; rows++)
            {
                for (int cols = -1; cols < 2; cols++)
                {
                    if (h + rows < 0 || h + rows > height - 1)
                    {
                        continue;
                    }

                    if (w + cols < 0 || w + cols > width - 1)
                    {
                        continue;
                    }

                    GxRed = GxRed + image[h + rows][w + cols].rgbtRed * Gx[rows + 1][cols + 1];
                    GxGreen = GxGreen + image[h + rows][w + cols].rgbtGreen * Gx[rows + 1][cols + 1];
                    GxBlue = GxBlue + image[h + rows][w + cols].rgbtBlue * Gx[rows + 1][cols + 1];

                    GyRed = GyRed + image[h + rows][w + cols].rgbtRed * Gy[rows + 1][cols + 1];
                    GyGreen = GyGreen + image[h + rows][w + cols].rgbtGreen * Gy[rows + 1][cols + 1];
                    GyBlue = GyBlue + image[h + rows][w + cols].rgbtBlue * Gy[rows + 1][cols + 1];
                }
            }
                int newRed = round(sqrt((GxRed * GxRed) + (GyRed * GyRed)));
                int newGreen = round(sqrt((GxGreen * GxGreen) + (GyGreen * GyGreen)));
                int newBlue = round(sqrt((GxBlue * GxBlue) + (GyBlue * GyBlue)));

                if (newRed > 255)
                {
                    aux[h][w].rgbtRed = 255;
                } else aux[h][w].rgbtRed = newRed;

                if (newBlue > 255)
                {
                    aux[h][w].rgbtBlue = 255;
                } else aux[h][w].rgbtBlue = newBlue;

                if (newGreen > 255)
                {
                    aux[h][w].rgbtGreen = 255;
                } else aux[h][w].rgbtGreen = newGreen;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = aux[h][w].rgbtRed;
            image[h][w].rgbtGreen = aux[h][w].rgbtGreen;
            image[h][w].rgbtBlue = aux[h][w].rgbtBlue;
        }
    }
    return;
}
