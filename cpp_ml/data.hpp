// Guards to prevent multiple inclusion of the same header file
#ifndef __DATA_H
#define __DATA_H


#include <vector> // Dynamic arrays
#include "stdint.h"
#include "stdio.h"

class data{
    // Define as pointer to allow for dynamic allocation of memory
    std::vector<uint8_t> * feature_vector; //No class
    uint8_t label;
    int enum_label;

public:
    void set_feature_vector(std::vector<uint8_t> *);
    void append_to_feature_vector(uint8_t);
    void set_label(uint8_t);
    void set_enumerated_label(int);

    int get_feature_vector_size();
    uint8_t get_label();
    uint8_t get_enumerated_label();

    std::vector<uint8_t> * get_feature_vector();
};

#endif