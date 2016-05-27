//
// Created by diqiu on 2016/5/16.
//

#ifndef MERCUR_DLV_H
#define MERCUR_DLV_H

#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <assert.h>
#include <iostream>

#define MAX_STR_LEN    6
template <class DataT>
class DLV {
public:
    DLV()
    {
    }

    ~DLV()
    {
    }

    int32_t minOfThree(int32_t a, int32_t b, int32_t c)
    {
        int32_t min = 0;
        a > b ? min = b : min = a;
        if ( c < min ) {
            min = c;
        }
        return min;
    }

    int32_t calculateDistance(DataT *str1, int32_t len1, DataT *str2, int32_t len2)
    {
        assert(len1 <= MAX_STR_LEN);
        assert(len2 <= MAX_STR_LEN);
        memset((void *)_matrix, 0, sizeof(_matrix));
        for (int32_t i = 0; i <= len1; i++) {
            _matrix[i][0] = i;
        }
        for (int32_t i = 0; i <= len2; i++) {
            _matrix[0][i] = i;
        }

        int32_t flag = 0;
        for ( int32_t i = 1; i <= len1; i++ ) 
        {
            for ( int32_t j = 1; j <= len2; j++ ) 
            {
                str1[i-1] == str2[j-1]? flag = 0: flag = 1;
                if ( (j >=2 && i >= 2) && str1[i-2] == str2[j-1] && str1[i-1] == str2[j-2] )
                { 
                    _matrix[i][j] = minOfThree(_matrix[i][j-1] + 1, _matrix[i-1][j] + 1, _matrix[i-2][j-2] + flag);
                }
                else
                {
                    _matrix[i][j] = minOfThree(_matrix[i][j-1] + 1, _matrix[i-1][j] + 1, _matrix[i-1][j-1] + flag);
                }
            }
        }
        return _matrix[len1][len2];
    }

    void dump()
    {
        for ( int32_t i = 0; i < MAX_STR_LEN; i++ ) {
            for ( int32_t j = 0; j < MAX_STR_LEN; j++ ) {
                std::cout << _matrix[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

private:
    int32_t _matrix[MAX_STR_LEN + 1][MAX_STR_LEN + 1];
};


#endif //MERCUR_DLV_H
