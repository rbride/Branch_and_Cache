//**************************************************************************//
// Project  ECE 483 Cache Simulator                                         //
// Author:  Ryan Bride                                                      //
// File:    Cache_Sim.cpp                                                   //
// Note:    Num Address bits = 32                                           //
//         - Ryan Bride                                                     //
//**************************************************************************//
#include "Cache_Sim.h"
using namespace std;

int main() {
    while(true) {
        int mode = 0;
        cout << "Enter 1 to begin Cache Simulator or 2 to Exit \n>: ";
        cin >> mode; cout << '\n';   
        switch (mode) {
            case 1:
                cache_sim();
                break;
            case 2:
                return 0;
            default:
                cout << "\nInvalid Entry";
                break;      
            }
        }
    return 0;
}

void cache_sim() {
    //Define L1 & L2 caches
    vector<vector<Cache_Line>> L1_Cache;
    vector<vector<Cache_Line>> L2_Cache;
    string InFileName, OutFileName, index;
    char w_r = '\0';

    //Parse File Input
    cout << "File Name: ";  cin >> InFileName;
    cout << "\nOutput File Name: "; cin >> OutFileName;
    ofstream outfile(OutFileName);
    ifstream infile(InFileName);
    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Input file and/or output file could not be opened." << endl;
        exit(1);
    }

    int blocksize = 0; int L1_size = 0; int L1_assoc = 0; int L2_size = 0; int L2_assoc = 0;    
    cout << "Enter Block Size:\n";              cin >> blocksize;
    cout << "\nEnter L1 Cache Size:\n";         cin >> L1_size;
    cout << "\nEnter L1 Associativity:\n";      cin >> L1_assoc;
    cout << "\nEnter L2 Cache Size:\n";         cin >> L2_size;
    cout << "\nEnter L2 Associativity:\n";      cin >> L2_assoc;

    //initialize and properly size L1 Cache
    int num_sets_L1 = L1_size / (blocksize * L1_assoc);
    int num_block_offset_bits_L1 = log(blocksize)/log(2);
    int num_index_bits_L1 = log(num_sets_L1)/log(2);
    L1_Cache.resize(num_sets_L1);
    for (size_t i = 0; i < L1_Cache.size(); ++i) {
        L1_Cache[i].resize(L1_assoc, {0, 0, 0, false, false});           
    }    
    
    //initialize and properly size L1 Cache
    int num_sets_L2 = L2_size / (blocksize * L2_assoc);
    int num_block_offset_bits_L2 = log(blocksize)/log(2);
    int num_index_bits_L2 = log(num_sets_L2)/log(2);
    L2_Cache.resize(num_sets_L2);
    for (size_t i = 0; i < L2_Cache.size(); ++i) {
        L2_Cache[i].resize(L2_assoc, {0, 0, 0, false, false});           
    }
    
    int idx = 0;
    while(infile >> w_r >> index) {
        idx = stoi(index, nullptr, 16);
        
    }
    return;
}

