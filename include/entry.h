//This is the header file for adding an entry into the journal.
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <string>
#include <ctime>
#ifndef ENTRY_H
#define ENTRY_H
inline std::int64_t timestamp();

void entry(std::string txt);


#endif
