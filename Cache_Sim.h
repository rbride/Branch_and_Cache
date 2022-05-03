#ifndef CACHE_SIM_H
#define CACHE_SIM_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

void cache_sim();
void get_line();
using Cache_Line = struct Cache_Line_t {
        int index;
        int lru_counter;
        int data; 
        bool dirty; 
        bool valid;
};
#endif