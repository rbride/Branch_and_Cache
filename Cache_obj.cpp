#include "Cache_obj.h"
#include <cmath>
#include <iostream>

using std::vector;

Cache::Cache(int size, int assoc, int block_size, bool on) {
    if(on){
        num_sets = size / (block_size * assoc);
        num_offset_bits = static_cast<int>(log(block_size)/log(2));
        num_index_bits = static_cast<int>(log(num_sets)/log(2));
        //Resize Cache to Proper Set Size
        cache.resize(num_sets);
         /*
        NOTE!!!! 
        this was ++i    Idk did I or chris make and error
        */
        for (size_t i = 0; i < cache.size(); i++) {
            cache[i].resize(assoc, {0, false, false});    //LRU counter Removed      
        }
        //Generate Values for masks 
        for (int i = 0; i < num_index_bits; i++){
            mask_index = (mask_index << 1) | 1;
        }
        for(int i = 0; i < num_offset_bits; i++ ){
            mask_offset = (mask_offset << 1) | 1;
        }
        lru_counter_max_val = assoc - 1;
        groups_in_set = assoc;
        kwisatz_haderach = 0;
    }
}


bool Cache::read(int address) {
    //Awaken The Sleeper (Reset The Kwitsatz Haderach)
    kwisatz_haderach = 0;
    tag_in = address >> (num_index_bits + num_offset_bits);
    index_in = (address >> num_offset_bits) & mask_index;
    offset_in = address & mask_offset;
    //Find if its there act accordingly
    for(kwisatz_haderach; kwisatz_haderach < groups_in_set; kwisatz_haderach++){
        if( static_cast<bool>(cache[index_in][kwisatz_haderach].tag == tag_in)){
            cache[index_in][kwisatz_haderach].tag = tag_in; 
            return false; 
        }
    }
    //Data is not in This Cache, Read is a miss.
    return true;
}

bool Cache::write(int address) {
    /* Question 
    I I Write to a block, and its there, I am pretty sure I am suppose to no longer
    label it as dirty, we can test this by doing so and comparing my results to the output
    */
    tag_in = address >> (num_index_bits + num_offset_bits);
    index_in = (address >> num_offset_bits) & mask_index;
    offset_in = address & mask_offset;

}

void Cache::LRU_Update() {
    //He who Controls the Spice Controls the Universe
    //LRU Management could be really fucking wrong but yeet
    // If there is two sets instead of 1 and we are on 2 we just iter swap
    // vec elem 1 wit vect elem 0 two simulate a LRU counter ie second becomes first   
    if( (groups_in_set == 1 ) && kwisatz_haderach ){  
        std::iter_swap(cache[index_in].begin(), cache[index_in].end());
        }
    // incase #sets > 2 Yeet Maxed Can't stop the Yeet
    //First one is because kwitz_had plus 1 will fail if its the final one
    //Ripped straight off Stack overflow and the CPP Ref for Rotate
    if (kwisatz_haderach == lru_counter_max_val){
        std::rotate(cache[index_in].rbegin(), 
            cache[index_in].rbegin()+1, 
            cache[index_in].rend());
    }
    //Niether of the above to cases rotate accordingly
    else {
        std:rotate(cache[index_in].begin(), 
            cache[index_in].begin()+kwisatz_haderach, 
            cache[index_in].begin()+kwisatz_haderach+1)
    }

}
