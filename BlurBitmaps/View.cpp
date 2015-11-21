#include "stdafx.h"


View::View(const int height, const int width, const short threads, const double time_c, const double time_asm)
{
	everythink_to_print << "Image: " << height << "x" << width << std::endl;
	everythink_to_print << "Threads: " << threads << std::endl;
	everythink_to_print << "Dll Type: " << std::endl;
	if (time_c > 0)
		everythink_to_print << "C:" << std::endl << "time:  " << time_c << " ms." << std::endl;
	if (time_asm > 0)
		everythink_to_print << "ASM:" << std::endl << "time: " << time_asm << " ms." << std::endl;
	
} 

View::~View()
{
}


void View::print_results_on_the_console() const
{
	std::cout << everythink_to_print.str();
}

void View::print_result_to_the_file(const std::string& file_name) const
{
	std::fstream file(file_name, std::ios::app);
	if (file.is_open())
	{
		file << everythink_to_print.str();
		file << "**********************************************" << std::endl;
	}
	file.close();
}