#pragma once
#include<glew.h>
#include<freeglut.h>
#include<fstream>
#include<iostream>
#include<string>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint loadBMP_custom(const char* imagepath);
GLuint loadDDS(const char* imagepath);