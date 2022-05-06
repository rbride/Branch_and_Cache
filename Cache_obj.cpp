#include "Cache_obj.h"
#include <cmath>
#include <iostream>
#include <fstream>
using std::vector;

Cache::Cache(int size, int assoc, int block_size, bool on) {
    if (on) {
        num_sets = size / (block_size * assoc);
        num_offset_bits = static_cast<int>(log(block_size) / log(2));
        num_index_bits = static_cast<int>(log(num_sets) / log(2));
        //Resize Cache to Proper Set Size
        cache.resize(num_sets);
        /*
       NOTE!!!! this was ++i    Idk did I or chris make and error
       */
        for (size_t i = 0; i < cache.size(); i++) {
            cache[i].resize(assoc, { 0, 0, 0, false, false });    //LRU counter Removed Data added   
        }
        //Generate Values for masks 
        for (int i = 0; i < num_index_bits; i++) {
            mask_index = (mask_index << 1) | 1;
        }
        for (int i = 0; i < num_offset_bits; i++) {
            mask_offset = (mask_offset << 1) | 1;
        }
        lru_counter_max_val = assoc - 1;
        tags_in_Set = assoc;
        kwisatz_haderach = 0;
    }
}

bool Cache::read(int address, bool w_flag) {
    //Awaken The Sleeper (Reset The Kwitsatz Haderach)
    kwisatz_haderach = 0;
    tag_in = address >> (num_index_bits + num_offset_bits);
    index_in = (address >> num_offset_bits) & mask_index;
    offset_in = address & mask_offset;

    //Find if its there act accordingly
    for (; kwisatz_haderach <= lru_counter_max_val; kwisatz_haderach++) {
        if (cache[index_in][kwisatz_haderach].tag == tag_in) {
            if (w_flag){
                cache[index_in][kwisatz_haderach].dirty = true; 
            }
            return false;
        }
    }

    //Data is not in This Cache, Read is a miss.
    return true;
}


int Cache::write(int address, bool w_flag) {
    //Awaken The Sleeper (Reset The Kwitsatz Haderach)
    int wtag = address >> (num_index_bits + num_offset_bits);
    int windex = (address >> num_offset_bits) & mask_index;
    int woffset = address & mask_offset;
    kwisatz_haderach = 0;
    //Check for invalid bits if so dump it there index in order
    for (; kwisatz_haderach <= lru_counter_max_val; kwisatz_haderach++) {
        if (!cache[windex][kwisatz_haderach].valid) {
            cache[windex][kwisatz_haderach].tag = wtag;
            cache[windex][kwisatz_haderach].valid = true;
            cache[windex][kwisatz_haderach].data = woffset;
            cache[windex][kwisatz_haderach].index = kwisatz_haderach;
            //If the Cache is operating a Write Command Label the new bit Dirty
            if(w_flag){
                cache[windex][kwisatz_haderach].dirty = true;
            }
            //No eviction so return address 0;
            return 0;
        }
    }
    //No Invalid Bits, Evict the last one First undo the math to seperate the bits
    int evicted_address = (
        (cache[windex][lru_counter_max_val].tag << (num_index_bits + num_offset_bits))
        | (cache[windex][lru_counter_max_val].index << num_offset_bits)
        | cache[windex][lru_counter_max_val].data
        );
    //Replace data at evicted spot
    cache[windex][lru_counter_max_val].tag = wtag;
    cache[windex][lru_counter_max_val].valid = true;
    if(w_flag){
        cache[windex][lru_counter_max_val].dirty = true;   ///////ffffffff
    }
    else{
        cache[windex][lru_counter_max_val].dirty = false;   ///////ffffffff

    }
    cache[windex][lru_counter_max_val].data = woffset;
    cache[windex][lru_counter_max_val].index = lru_counter_max_val;
    return(evicted_address);
}

void Cache::LRU_Update() {
    if (tags_in_Set == 1) {
        return;
    }
    // If there is two sets instead of 1 and we are on 2 we just iter swap
    // vec elem 1 wit vect elem 0 two simulate a LRU counter ie second becomes first   
    if ( tags_in_Set == 2 && kwisatz_haderach) {
        std::iter_swap(cache[index_in].begin(), cache[index_in].end());
        return;
    }
    // incase #sets > 2 first one is because kwitz_had plus 1 will fail if its the final one
    if (kwisatz_haderach == lru_counter_max_val) {
        std::rotate(cache[index_in].rbegin(),
            cache[index_in].rbegin() + 1,
            cache[index_in].rend());
    }
    //Niether of the above to cases rotate accordingly
    else {
        std::rotate(cache[index_in].begin(),
            cache[index_in].begin() + kwisatz_haderach,
            cache[index_in].begin() + kwisatz_haderach + 1);
    }
    return;
}

void Cache::spit_out_data(std::ofstream &outputfile) {
    for (int i = 0; i < num_sets; i++) {
        outputfile << "set\t" << std::dec << i << ':' << '\t';
        for (int q = 0; q <= lru_counter_max_val; q++) {
            outputfile << std::hex << cache[i][q].tag << ' ';
            if (cache[i][q].dirty) {
                outputfile << ' ' << 'D' << ' ';
            }
            else {
                outputfile << "   ";
            }
        }

        outputfile << '\n';
    }
    return;
}