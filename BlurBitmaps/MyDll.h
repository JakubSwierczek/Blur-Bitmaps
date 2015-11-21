#pragma once
#include <windows.h>
using FUNCTION = void* (*)(const unsigned char*, const unsigned char*, const int, const int);
FUNCTION low_pass_filter_c_dll;
FUNCTION low_pass_filter_asm_dll;
inline void load_c_dll_function()
{
	//using WinAPI to load DLL
	HINSTANCE dll_c_hinstance = LoadLibrary(L"LowPassFilterC.dll");
	if (!dll_c_hinstance)
	{
		exit(-2);
	}
	
	low_pass_filter_c_dll = reinterpret_cast<FUNCTION>(GetProcAddress(static_cast<HMODULE>(dll_c_hinstance), "low_pass_filter"));
	
	if (!low_pass_filter_c_dll)
	{
		exit(-2);
	}
	
}

inline void load_asm_dll_function()
{
	HINSTANCE dll_asm_hinstance = LoadLibrary(L"LowPassFilterAsm.dll");


	if (!dll_asm_hinstance)
	{
		exit(-2);
	}
	low_pass_filter_asm_dll = reinterpret_cast<FUNCTION>(GetProcAddress(static_cast<HMODULE>(dll_asm_hinstance), "LowPassFilterProc"));
	if (!low_pass_filter_asm_dll)
	{
		exit(-2);
	}
}
inline void load_dll(const bool c_dll, const bool asm_dll)
{
	if (c_dll)
		load_c_dll_function();
	if (asm_dll)
		load_asm_dll_function();
}