#ifndef CACHE_OBJ_H
#define CACHE_OBJ_H
#include <vector>

const int Address_Width = 32; //#of Address Bits

using Cache_Line = struct Cache_Line_t {
    int tag;
    //int lru_counter; 
    bool dirty; 
    bool valid;
};

class Cache {
public:
    Cache(int size, int assoc, int block_size, bool on=true); 
    ~Cache() {};
    bool read(int address);
    bool write(int address);
    void LRU_Update();

private:
    std::vector<std::vector<Cache_Line>> cache;
    int num_sets;
    int num_offset_bits;
    int num_index_bits;
    bool on;
    int mask_index;
    int mask_offset;
    int tag_in;
    int index_in;
    int offset_in;
    int lru_counter_max_val;
    int groups_in_set;
    int kwisatz_haderach; 
};

#endif