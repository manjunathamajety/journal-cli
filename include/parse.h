#include <iostream>
#include <jrnlmanager.h>
#include <journal.h>
#include <config.h>
#include <unistd.h>
#include <string>
#include <help.h>

#ifndef PARSE_H
#define PARSE_H

void init_handle(int argc, char** argv);

void add_handle(int argc,char** argv);

void display_handle(int argc,char** argv);

void backup_handle(int argc, char** argv);

#endif
