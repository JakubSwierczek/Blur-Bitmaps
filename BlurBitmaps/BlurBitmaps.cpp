// BlurBitmaps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyDll.h"
#include <thread>

#include <vector>
#include <chrono>

enum pointers {red, green, blue};
double creating_threads(unsigned char* input, unsigned char* output, const int width, const int height, short threads, FUNCTION my_function)
{	
	auto shifts = new int[threads];
	auto fragment_height = new int[threads];
	auto array_thread = new std::thread[threads];
	auto temp_height = height;

	shifts[0] = 0;
	for (short avaible_threads = threads, i = 0; avaible_threads > 1; avaible_threads--, i++)
	{
		fragment_height[i] = temp_height / avaible_threads;
		temp_height = temp_height - fragment_height[i];
		shifts[i + 1] = shifts[i] + fragment_height[i] * width;
	}

	fragment_height[threads - 1] = temp_height;
	auto start_time = std::chrono::steady_clock::now();
	for (auto i = 0; i < threads; i ++)
		array_thread[i] = std::thread(my_function, input + shifts[i], output + shifts[i], width, fragment_height[i]);

	//waiting for all threads are done
	for (auto i = 0; i < threads; i++)
		if (array_thread[i].joinable())
			array_thread[i].join();
	
	auto end_time = std::chrono::steady_clock::now();
	auto duration = end_time - start_time;

	return std::chrono::duration<double, std::milli>(duration).count();

}
int main(int argc, char *argv[])
{
	std::string input_file_name("");
	std::string output_file_name("");
	unsigned short threads = 0;
	auto c_dll = false;
	auto asm_dll = false;
	auto time_c = 0.0, time_asm = 0.0;
	for (auto i = 1; i < argc; i++)
	{
		std::string current_arg(argv[i]);
		auto separator_possition = current_arg.find("=");
		auto mode = current_arg.substr(0, separator_possition);
		auto action = current_arg.substr(separator_possition+1);
		if (mode == "-i")
			input_file_name = action;
		else if (mode == "-o")
			output_file_name = action;
		else if (mode == "-t")
			threads = action[0] - '0';
		else if (mode == "-dll")
			if (action == "C" || action == "c")
				c_dll = true;
			else if (action == "asm" || action == "ASM")
				asm_dll = true;
	}

	if (input_file_name == "" || output_file_name == "")
	{
		std::cout << "Lack of filenames";
		return -1;
	}

	if(threads > std::thread::hardware_concurrency() || threads == 0)
		threads = std::thread::hardware_concurrency();
	
	if(c_dll == false && asm_dll == false)
	{
		c_dll = true;
		asm_dll = true;
	}
	
	load_dll(c_dll, asm_dll);
	

	bitmap_image image(input_file_name);
	if(!image)
		return -1;

	auto height = image.height();
	auto width = image.width();

	
	auto input = new unsigned char[((3 * height) + 2)*width];
	auto output = new unsigned char[((3 * height) + 2)*width];

	auto red = input + width;
	auto green = red + height*width;
	auto blue = green + height*width;

	// load pixels to red, green, blue array
	for (auto i = 0; i < height; ++i)
	{
		for (auto j = 0; j < width; ++j)
		{
			image.get_pixel(j, i, *red, *green, *blue);
			red++;
			green++;
			blue++;
		}
	}
	auto temp_input = input + width;
	auto temp_output = output + width;
	FUNCTION my_function;
	if(c_dll)
	{
		my_function = low_pass_filter_c_dll;
		time_c = creating_threads(temp_input, temp_output, width, height * 3, threads, my_function);
	}
	if (asm_dll)
	{
		my_function = low_pass_filter_asm_dll;
		time_asm = creating_threads(temp_input, temp_output, width, height * 3, threads, my_function);
	}
	
	
	
	//seting pixels
	red = output + width;
	green = red + height*width;
	blue = green + height*width;

	for (auto i = 0; i < height; ++i)
		for (auto j = 0; j < width; ++j)
		{
			image.set_pixel(j, i, *red, *green, *blue);
			red++;
			green++;
			blue++;

		}
	
	//crop image  
	bitmap_image output_image;
	image.region(1, 1, width - 2, height - 2, output_image);
	
	//saving image
	output_image.save_image(output_file_name);
	
	delete input;
	delete output;



	///////////////////////////////////////////////////////////////////
	View view(height, width, threads, time_c, time_asm);
	view.print_results_on_the_console();
	view.print_result_to_the_file("log.txt");
	///////////////////////////////////////////////////////////////////////

	return 0;
}

