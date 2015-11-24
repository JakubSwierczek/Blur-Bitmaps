#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
class View
{
private:
	std::ostringstream everythink_to_print;
	std::ostringstream only_result_to_print;
	bool file_exists(const std::string& file_name) const;
	
public:
	View(const int height, const int width, const short threads, const double time_c, const double time_asm);
	~View();
	void print_results_on_the_console() const;
	void print_result_to_the_file(const std::string& file_name) const;
	
};

