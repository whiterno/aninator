#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/akinator.h"
#include "../include/big_array.h"

int bigArrayCtor(BinDatabase** database_tree){
    assert(database_tree);

    (*database_tree)->big_array_capacity = BIG_ARR_BEG_CAPACITY;
    (*database_tree)->big_array = (BigArrElem*)calloc(BIG_ARR_BEG_CAPACITY, sizeof(BigArrElem));
    for (int i = 0; i < BIG_ARR_BEG_CAPACITY; i++){
        (*database_tree)->big_array[i].beg = (char*)calloc(BIG_ARR_ELEM_CAPACITY, sizeof(char));
        (*database_tree)->big_array[i].end = (*database_tree)->big_array[i].beg;
    }

    return NO_ERROR;
}

int addToBigArray(BinDatabase** database_tree, char* database_txt, char** feature, int feature_len){
    assert(database_tree);
    assert(database_txt);
    assert(feature);

    for (int i = 0; i < (*database_tree)->big_array_capacity; i++){
        int left_space = BIG_ARR_ELEM_CAPACITY - ((*database_tree)->big_array[i].end - (*database_tree)->big_array[i].beg + 1);

        if (feature_len > left_space){
            continue;
        }

        strncpy((*database_tree)->big_array[i].end + 1, database_txt + 1, feature_len - 1);
        (*feature) = (*database_tree)->big_array[i].end + 1;

        (*database_tree)->big_array[i].end = (*database_tree)->big_array[i].end + feature_len;
        *(*database_tree)->big_array[i].end = '\0';

        return NO_ERROR;
    }

    //! TODO: сделать расширяемый bigArray

    return NO_ERROR;
}
