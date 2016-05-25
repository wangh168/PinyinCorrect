//
// Created by diqiu on 2016/5/17.
//

#ifndef MERCUR_TRIE_H
#define MERCUR_TRIE_H

// Created by diqiu on 2015/10/15.
//

#ifndef TITAN_TRIE_H
#define TITAN_TRIE_H

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

template <class KeyT, class DataT>
class DataBody {
public:
    DataBody<KeyT, DataT>()
    {

    }

    DataBody<KeyT, DataT> operator =(DataBody &t)
    {
        if ( &t == this ) {
            return *this;
        }
        _key = t._key;
        _value = t._value;
        return *this;
    }

    DataBody<KeyT, DataT>(DataBody<KeyT, DataT> & dataBody)
    {
        _key = dataBody._key;
        _value = dataBody._value;
    };

    KeyT _key;
    DataT _value;
};

//the array sorted by the keyt from min to max
template <class KeyT, class DataT>
class SortArray {
public:
    SortArray(int32_t size=0):_actualSize(0),_maxSize(size)
    {
        if (size) {
            _body = (DataBody<KeyT, DataT> *)malloc(sizeof(DataBody<KeyT, DataT>)*size);
            memset(_body, 0, sizeof(DataBody<KeyT, DataT>)*size);
            //_body = new DataBody<KeyT, DataT>[size];
        } else {
            _body = NULL;
        }
    }

    SortArray(SortArray<KeyT, DataT> &sortArray)
    {
        _actualSize = sortArray._actualSize;
        _maxSize = sortArray._maxSize;
        _body = (DataBody<KeyT, DataT> *)malloc(sizeof(DataBody<KeyT, DataT>)*_maxSize);
        memset(_body, 0, sizeof(DataBody<KeyT, DataT>)*_maxSize);
        //_body = new DataBody<KeyT, DataT>[_maxSize];
        for ( int32_t i = 0; i < _actualSize; i++ ) {
            _body[i]  = sortArray._body[i];
        }
    }

    ~SortArray()
    {
        if ( _body ) {
            free(_body);
            //delete[]_body;
        }
    }

    SortArray<KeyT, DataT> operator=(SortArray<KeyT, DataT> &sortArray)
    {
        if ( &sortArray == this ) {
            return *this;
        }
        if ( sortArray._maxSize == 0 ) {
            _actualSize = sortArray._actualSize;
            _maxSize = sortArray._maxSize;
            _body = sortArray._body;
            return *this;
        }
        _body = (DataBody<KeyT, DataT> *)malloc(sizeof(DataBody<KeyT, DataT>)*_maxSize);
        memset(_body, 0, sizeof(DataBody<KeyT, DataT>)*_maxSize);
        //_body = new DataBody<KeyT, DataT>[_maxSize];
        for ( int32_t i = 0; i < sortArray._actualSize; i++ ) {
            _body[i] = *sortArray[i];
        }
        _maxSize = sortArray._maxSize;
        _actualSize = sortArray._actualSize;
        return *this;
    }

    DataBody<KeyT, DataT> *operator[](int32_t pos)
    {
        return &_body[pos];
    }

    void release()
    {
        if ( _body ) {
            free(_body);
            //delete[]_body;
            _actualSize = 0;
            _maxSize = 0;
            _body = NULL;
        }
    }

    void alloc(int32_t size)
    {
        if ( _maxSize > size ) {
            return;
        }

        DataBody<KeyT, DataT> *p =(DataBody<KeyT, DataT> *) malloc(sizeof(DataBody<KeyT, DataT>) *size);
        memset(p, 0, sizeof(DataBody<KeyT, DataT>)*size);
        //DataBody<KeyT, DataT> *p = new DataBody<KeyT, DataT>[size];
        _maxSize = size;
        if (_body == NULL)
        {
            _body = p;
            return;
        }
        memcpy(p, _body, _actualSize*sizeof(DataBody<KeyT, DataT>));
        //delete []_body;
        free(_body);
        _body = p;
    }

    int32_t insert(DataBody<KeyT, DataT> &t)
    {
        int32_t index = 0;
        bool hasExists = find(t._key, index);
        if ( hasExists ) {
            std::cout << "the key has exists" << std::endl;
            return index;
        }
        if (_maxSize == _actualSize)  {
            alloc(_maxSize+1);
        }

        int32_t pos = 0;
        if ( _actualSize == 0 ) 
        {
            _body[pos] = t;
        }
        else if ( _body[index]._key > t._key ) 
        {
            memmove(&_body[index+1], &_body[index], sizeof(DataBody<KeyT, DataT>)*(_actualSize-index));
            _body[index] = t;
            pos = index;
        }
        else
        {
            memmove(&_body[index+2], &_body[index+1], sizeof(DataBody<KeyT, DataT>)*(_actualSize-index-1));
            _body[index+1] = t;
            pos = index+1;
        }
        _actualSize++;

        return pos;
    }

    bool find(KeyT &key, int32_t &index)
    {
        int32_t begin = 0;
        int32_t end = _actualSize-1;
        int32_t middle = 0;
        while ( begin <= end )  {
            middle =  begin + (end - begin)/2;
            if ( _body[middle]._key < key ) {
                begin = middle + 1;
            } else if (_body[middle]._key > key ) {
                end = middle - 1;
            } else {
                index = middle;
                return true;
            }
        }
        index = middle;
        return false;
    }

    bool find(KeyT &key)
    {
        int32_t index = 0;
        return find(key, index);
    }

    bool find(KeyT &key, DataT *&value)
    {
        int32_t index = 0;
        if (find(key, index)) {
            value =(DataT *) &(_body[index]._value);
            return true;
        }

        value = NULL;
        return false;
    }

    bool remove(KeyT &key)
    {
        DataT *val;
        int32_t index = 0;
        if ( find(key, index) ) {
            memmove(&_body[index], &_body[index+1], (_actualSize-index-1)*sizeof(DataBody<KeyT, DataT>));
            _actualSize--;
        }
        return true;
    }

    void dump()
    {
        for ( int32_t i = 0; i < _actualSize; i++ ) {
            std::cout << "key is " << _body[i]._key << ";";
//            std::cout << "data is " << _body[i]._value << ";";
            std::cout << std::endl;
        }
    }

    int64_t memory_size()
    {
        return _maxSize * sizeof(DataBody<KeyT, DataT>) + sizeof(*this);
    }

    int32_t size()
    {
        return _maxSize;
    }

    int32_t count()
    {
        return _actualSize;
    }

private:
    int32_t _maxSize;
    int32_t _actualSize;
    DataBody<KeyT, DataT> *_body;
};

template <class KeyT, class DataT> class SubTrieIteator;
template <class KeyT, class DataT>
class Trie {
    friend class  SubTrieIteator<KeyT, DataT>;
public:
    Trie()
    {
    }

    int32_t getSubTrieSize()
    {
        return _sub.size();
    }

    int32_t setSubTrieSize(int32_t &size)
    {
        _sub.alloc(size);
        return size;
    }

    bool insertSubTrie(DataBody<KeyT, DataT> &t)
    {
        DataBody<KeyT, Trie<KeyT, DataT> > data;
        data._key = t._key;
        data._value._data = t._value;
        return _sub.insert(data);
    }

    //@return: if not found, return NULL
    Trie<KeyT, DataT> *findSubTrie(KeyT &key)
    {
        Trie<KeyT, DataT> *trie;
        _sub.find(key, trie);
        return trie;
    }

    DataT &findDataT()
    {
        return _data;
    }

    void updateDataT(DataT &data)
    {
        _data = data;
    }

    bool removeTrie(KeyT &key)
    {
        Trie<KeyT, DataT> *trie;
        if ( _sub.find(key, trie) ) {
            _sub.remove(key);
        }
        return true;
    }

    bool removeSubTrie()
    {
        _sub.release();
        return true;
    }

    SortArray< KeyT, Trie<KeyT, DataT> > *Childrens()
    {
        return (SortArray< KeyT, Trie<KeyT, DataT> > *)&_sub;
    }

private:
    SortArray<KeyT, Trie<KeyT, DataT>> _sub;
    DataT _data;
};

template <class KeyT, class DataT>
class SubTrieIteator
{
public:
    SubTrieIteator(Trie<KeyT, DataT> *t, int32_t pos = 0):_root(t), _currentPos(pos)
    {
    }

    ~SubTrieIteator()
    {
    }
    /*
     * just iterator his children
     */
    Trie<KeyT, DataT> *getDataT()
    {
        int32_t  count = _root->getSubTrieSize();
        if ( count == 0 || (count-1) < _currentPos ) {
            return NULL;
        }
        return &_root->_sub[_currentPos]->_value;
    }

    KeyT *getKeyT()
    {
        assert(_currentPos >= 0);
        int32_t  count = _root->getSubTrieSize();
        if ( count == 0 || (count-1) < _currentPos ) {
            return NULL;
        }
        return &_root->_sub[_currentPos]->_key;
    }

    SubTrieIteator<KeyT, DataT> operator ++(int i)
    {
        _currentPos++;
        return *this;
    };

    void reset()
    {
        _currentPos = 0;
    }

private:
    Trie<KeyT, DataT> *_root;
    int32_t  _currentPos;
};

#endif //TITAN_TRIE_H

#endif //MERCUR_TRIE_H
