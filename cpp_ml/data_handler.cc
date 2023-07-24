#include "data_handler.hpp"

data_handler::data_handler() {

    data_array = new std::vector<data *>;
    test_data = new std::vector<data *>;
    training_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}

data_handler::~data_handler();

