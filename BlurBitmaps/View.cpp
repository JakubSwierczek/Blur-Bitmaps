#include "stdafx.h"


View::View(const int height, const int width, const short threads, const double time_c, const double time_asm)
{
	auto width_stream = 15;
	std::ostringstream values;
	values << width << "x" << height;
	std::string piksels_string = values.str();
	everythink_to_print.width(width_stream);
	everythink_to_print << "Image:";
	everythink_to_print.width(width_stream);
	everythink_to_print << "Threads:";
	everythink_to_print.width(width_stream);
	everythink_to_print << std::internal << "DLL C time:";
	everythink_to_print.width(width_stream);
	everythink_to_print << std::internal << "DLL ASM time:";
	everythink_to_print << std::endl;
	everythink_to_print.width(width_stream);
	everythink_to_print << std::internal << piksels_string;
	everythink_to_print.width(width_stream);
	everythink_to_print << std::internal << threads;
	if (time_c > 0)
	{
		everythink_to_print.width(width_stream);
		everythink_to_print << std::internal << time_c;
	}
	else
	{
		everythink_to_print.width(width_stream);
		everythink_to_print << std::internal << " ";
	}
		
	if (time_asm > 0)
	{
		everythink_to_print.width(width_stream);
		everythink_to_print << std::internal << time_asm;
	}
	else
	{
		everythink_to_print.width(width_stream);
		everythink_to_print << std::internal << " ";
	}
		
	everythink_to_print << std::endl;

	only_result_to_print.width(width_stream);
	only_result_to_print << std::internal << piksels_string;
	only_result_to_print.width(width_stream);
	only_result_to_print << std::internal << threads;
	if (time_c > 0)
	{
		only_result_to_print.width(width_stream);
		only_result_to_print << std::internal << time_c;
	}
	else
	{
		only_result_to_print.width(width_stream);
		only_result_to_print << std::internal << " ";
	}

	if (time_asm > 0)
	{
		only_result_to_print.width(width_stream);
		only_result_to_print << std::internal << time_asm;
	}
	else
	{
		only_result_to_print.width(width_stream);
		only_result_to_print << std::internal << " ";
	}

	only_result_to_print << std::endl;
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
	if(file.is_open())
	{
		if (file_exists(file_name))
		{
			file << only_result_to_print.str();
		}
		else
		{
			file << everythink_to_print.str();
		}
	}
	file.close();
}

bool View::file_exists(const std::string& file_name) const 
{
	bool result;
	std::fstream file;
	file.open(file_name, std::ios::in);
	std::string temp;
	file >> temp;
	if (temp == "")
		result = false;
	else
		result = true;

	file.close();
	return result;
}
