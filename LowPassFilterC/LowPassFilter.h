#pragma once
#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
void DLLIMPORT low_pass_filter(const unsigned char* pixels_to_change, const unsigned char* output_pixels, const int width, const int fragment_height);