//**************************************************************************//
// Project  ECE 483 Cache Simulator                                         //
// Author:  Ryan Bride                                                      //
// File:    Cache_Sim.cpp                                                   //
// Note:    Num Address bits = 32                                           //
//          address -> [tag][index][offset]                                 //
//          LRU is only 0 or 1 as assoc is 1 or 2 in test cases therefore   //
//          for ease of dev I do not dynamically control the LRU I just     //
//          Swap the first and second place if its the item is not there    //
//         - Ryan Bride                                                     //
//**************************************************************************//
#include "Cache_obj.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static bool L2_flag = false; 
void cache_sim();

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
    string InFileName, OutFileName, address_str;
    char w_r = '\0'; int blocksize = 0;  int L1_size = 0;
    int L1_assoc = 0; int L2_size = 0; int L2_assoc = 0;

    //Parse File Input
    cout << "File Name: ";  cin >> InFileName;
    cout << "\nOutput File Name: "; cin >> OutFileName;
    ofstream outfile(OutFileName);
    ifstream infile(InFileName);
    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Input file and/or output file could not be opened." << endl;
        exit(1);
    }

    cout << "Enter Block Size:\n";
    cin >> blocksize;
    cout << "\nEnter L1 Cache Size:\n";
    cin >> L1_size;
    cout << "\nEnter L1 Associativity:\n";
    cin >> L1_assoc;
    cout << "\nEnter L2 Cache Size:\n";
    cin >> L2_size;
    if (L2_size != 0){
            cout << "\nEnter L2 Associativity:\n";      
            cin >> L2_assoc;
            L2_flag = true;
    }

    //Initilize needed Cache and Read/Write Counters
    int L1_read_count = 0; int L1_read_miss_count = 0;
    int L1_write_count = 0; int L1_write_miss_count = 0;
    int L2_read_count = 0; int L2_read_miss_count = 0;
    int L2_write_count = 0; int L2_write_miss_count = 0;
    Cache L1_Cache(L1_size, L1_assoc, blocksize); 
    Cache L2_Cache(L2_size, L2_assoc, blocksize, L2_flag);
    
    while(infile >> w_r >> address_str) {
        /*
        NOTE!!!! 
        When doing a read, if the read misses, and I do a write,
        Do I count that Write as a Write and a Write Miss? Might have to
        Test by adding those in later, for now they have been remove           
        */
        bool missed_flag = false; 
        int address = stoi(address_str, nullptr, 16);  
        //compare linearly the tags at the set associated with the index
        if(w_r == 'r'){
            L1_read_count++;
            missed_flag = L1_Cache.read(address);
            // Read Miss at L1 Chace (L1 is the only Cache)
            if(missed_flag && !L2_flag){
                L1_read_miss_count++;
                L1_Cache.write(address);
                L1_Cache.LRU_Update();
                break;
            }
            // Read Miss at L1 Cache (L2 Cache Exist)
            else if(missed_flag && L2_flag){
                L1_read_miss_count++;
                L2_read_count++;
                L1_Cache.write(address);
                missed_flag = L2_Cache.read(address);
                //Read Miss at L2
                if(missed_flag){
                    L2_read_miss_count++;
                    L2_Cache.write(address);
                    L2_Cache.LRU_Update()
                }
                //Else its a Hit update LRU 
                else{
                    L2_Cache.LRU_Update();
                }
                //Either way Update L1 LRU counter
                L1_Cache.LRU_Update();
            }
            //Hit Just Update LRU
            else{
                L1_Cache.LRU_Update();
            }
        
        }

        else if(w_r == 'w'){

        }

        else {
            cerr << "Write better tests, *insert Politically correct insult*." << endl;
            exit(1);
        }

    }
    return;
}

/*
store
    check if tag match in L1
        if hit in L1
            - update LRU 
            - increment write 
        if miss in L1
            - increment write miss
            - check if there is an invalid block at that location
            - if invalid block found
                - store at that location 
                - update LRU
                - label dirty, and valid
                - write block or whatever

            - if all blocks valid
                - single out block for eviction Should be oldest block (highest LRU)
                - store block at location of evicted block
                    - label block dirty, and valid
                    - update LRU
                - send victim block to next level up and store
                
                read L2 cache 
                    if hit at L2
                        - update LRU
                        - increment Write
                    if miss:
                        - increment write miss
                        - check if there is invalid block
                        if invalid block found
                            - store at that location 
                            - update LRU
                            - label dirty, and valid
                            - write block or whatever
                        if no invalid block found
                            - single out block for eviction Should be oldest block (highest LRU)
                            - store block at location of evicted block
                                - label block dirty, and valid
                                - update LRU
                            - Yeet the victim block out of existence because no higher level 
                            
load
    check if tag match in L1
        if hit in L1
            - update LRU
            - increment read
            return 
        if miss in L1
            - increments read and misses
            - increment LRU
            - store() the value at the index.
            
            check if tag match at l2
                if hit in l2
                    update LRU 
                    - increment l2 reads
                if miss in l2
                    increment l2 read and misses
                    - update LRU
                    store() the value at index
            
*/