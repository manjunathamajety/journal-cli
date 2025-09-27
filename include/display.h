//header file for display journal entries in human readable format.
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>
#ifndef DISPLAY_H
#define DISPLAY_H

std::string timeconvert(std::string timestamp);

void displayn(int n);

void display(std::string num);

    
#endif
