//
// Created by diqiu on 2016/5/18.
//

#ifndef MERCUR_MAXHEAP_H
#define MERCUR_MAXHEAP_H

#include <stdint.h>
#include <iostream>

template <class DataT>
class MaxHeap {
public:
    MaxHeap(int32_t nMax = 100)
    {
        _maxSize = nMax;
        _actualSize = 0;
        _heap = new DataT[_maxSize];
    }

    MaxHeap(const MaxHeap<DataT> &maxHeap)
    {
        _maxSize = maxHeap._maxSize;
        _actualSize = maxHeap._actualSize;
        _heap = new DataT[_maxSize];
        for ( int32_t i = 0; i < _actualSize; i++ ) {
            _heap[i] = maxHeap._heap[i];
        }
    }

    ~MaxHeap()
    {
        delete []_heap;
    }

    bool first(DataT &data)
    {
        if ( _actualSize > 0 ) {
            data = _heap[0];
            return  true;
        }
        return false;
    }

    bool insert(DataT i)
    {
        if ( isFull() ) 
        {
            return false;
        }

        if ( _actualSize == 0 ) 
        {
            _heap[_actualSize++] = i;
            return true;
        }

        int32_t sz = _actualSize++;
        while ( sz > 0 ) 
        {
            if ( i > _heap[(sz - 1) / 2]) 
            {
                _heap[sz] = _heap[(sz - 1) / 2];
                _heap[(sz - 1) / 2] = i;
                sz = (sz - 1) / 2;
            }
            else 
            {
                _heap[sz] = i;
                break;
            }
        }
        return true;
    }

    bool insertForce(DataT i)
    {
        if ( isFull() ) {
            DataT o;
            out(o);
        }
        return insert(i);
    }

    bool out(DataT &o)
    {
        if ( _actualSize > 0 )
        {
            int32_t sz = --_actualSize;
            o = _heap[0];
            _heap[0] = _heap[sz];
            int32_t start = 0;
            DataT tmp, max;
            while ( start < (sz / 2 ) ) 
            {
                if ( _heap[2 * start + 1] < _heap[2 * start + 2] ) 
                {
                    max = _heap[2 * start + 2];
                    start = 2 * start + 2;
                }
                else 
                {
                    max = _heap[2 * start + 1];
                    start = 2 * start + 1;
                }

                if ( _heap[(start - 1) / 2] < max ) 
                {
                    tmp = _heap[(start - 1) / 2];
                    _heap[(start - 1) / 2] = _heap[start];
                    _heap[start] = tmp;
                } 
                else 
                {
                    break;
                }
            }
            return true;
        }

        return false;
    }

    bool isFull() const
    {
        return _actualSize == _maxSize;
    }

    bool isEmpty() const
    {
        return _actualSize == 0;
    }

    void print() const
    {
        for ( int32_t i = 0; i < _actualSize; i++ ) {
        }
    }

private:
    int32_t _maxSize;
    int32_t _actualSize;
    DataT *_heap;
};

#endif //MERCUR_MAXHEAP_H
