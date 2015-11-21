#include "LowPassFilter.h"
void low_pass_filter(const unsigned char* pixels_to_change, const unsigned char* output_pixels, const int width, const int fragment_height)
{
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned int value = 0;
	unsigned char* matrix[9];
	unsigned char* temp_pixel = pixels_to_change;
	unsigned char* output_temp_pixel = output_pixels;


	//my matrix
	matrix[0] = temp_pixel - width;
	matrix[1] = matrix[0]++;
	matrix[2] = matrix[1]++;
	matrix[3] = temp_pixel;
	matrix[4] = temp_pixel + 1;
	matrix[5] = matrix[4] + 1;
	matrix[6] = matrix[3] + width;
	matrix[7] = matrix[4] + width;
	matrix[8] = matrix[5] + width;

	output_temp_pixel++;
	for (i = 0; i < fragment_height; i++)
	{
		for (j = 0; j < width; j++)
		{
			value = 0;
			for (k = 0; k < 9; k++)
				value += *matrix[k];
			*output_temp_pixel = value/9;
			output_temp_pixel++;
			for (k = 0; k < 9; k++)
				matrix[k]++;
		}
	}
}


