#include <iostream>

#define SLASHES std::cout << "---------------------------------------------\n";
#define FSLASHES output   << "---------------------------------------------\n";
#define FSHORTSLASH output << "-----------------\n";

#include "config.h"
#include <assert.h>
#include <vector>
#include <string>
#include <ctype.h>

#include "error_classes.h"
#include "file_class.cpp"
#include "TOKEN_INFO.cpp"
#include "lexical_analizer.cpp"
#include "Btree.cpp"
#include "syntax_analizer.cpp"
