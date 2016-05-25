//
// Created by diqiu on 2016/5/16.
//

#ifndef MERCUR_BKTREE_H
#define MERCUR_BKTREE_H

#include "Trie.h"
#include "DLV.h"
#include <vector>

template <class DataT>
class BKNode{
public:
    BKNode()
    {
        _data = NULL;
        _len = 0;
    }

    BKNode(DataT *data, int32_t len)
    {
        _data = new DataT[len];
        _len = len;
        memcpy((char *)_data, (char *)data, sizeof(DataT) * len);
    }

    BKNode<DataT> operator=(BKNode<DataT> node)
    {
        if ( &node == this ) {
            return *this;
        }

        if ( _len != node._len || _data == NULL) {
            delete []_data;
            _data = new DataT[node._len];
        }

        _len = node._len;
        memcpy((char *)_data, (char *)node._data, sizeof(DataT) * node._len);
        return *this;
    }

    BKNode<DataT>(const BKNode<DataT> &bkNode)
    {
        _data = new DataT[bkNode._len];
        _len = bkNode._len;
        memcpy((char *)_data, (char *)bkNode._data, sizeof(DataT) * _len);
    }

    ~BKNode()
    {
        delete []_data;
        _data = NULL;
        _len = 0;
    }

    DataT *getData()
    {
        return _data;
    }

    int32_t getLen()
    {
        return _len;
    }
private:
    DataT *_data;
    int32_t _len;
};

template <class DataT>
class BKTree{
public:
    BKTree()
    {

    }

    BKTree(BKTree<DataT> &bkTree)
    {
        _sub = bkTree._sub;
        _node = bkTree._node;
    }

    BKTree<DataT> operator=(BKTree<DataT> &bkTree)
    {
        if ( &bkTree == this ) {
            return *this;
        }
        _sub = bkTree._sub;
        _node = bkTree._node;
        return *this;
    }

    ~BKTree()
    {
    }

    int32_t search(DataT *data, int32_t len, int32_t diffVal, std::vector<BKNode<DataT>> &out)
    {
        if ( _node.getLen() == 0 ) {
            return -1;
        }
        DLV<DataT> dlv;
        int32_t distance = dlv.calculateDistance(_node.getData(), _node.getLen(), data, len);
        if ( distance == diffVal ) {
            out.push_back(_node);
        }
        int32_t minDis = diffVal - std::abs(distance);
        int32_t maxDis = diffVal + std::abs(distance);
        for ( int32_t i = minDis; i <= maxDis; i++ ) {
            BKTree<DataT> *bkTree = NULL;
            if ( _sub.find(i, bkTree) ) {
                bkTree->search(data, len, diffVal, out);
            }
        }
        return 0;
    }

    int32_t addNode(DataT *data, int32_t len)
    {
        if ( data == NULL || len == 0 ) {
            assert(data != NULL && len != 0);
            return -1;
        }
        if ( _node.getLen() == 0 ) {
            _node = BKNode<DataT>(data, len);
            return 0;
        }
        DLV<DataT> dlv;
        int32_t distance = dlv.calculateDistance(_node.getData(), _node.getLen(), data, len);
        if ( distance <= 0 ) {
            return -1;
        }
        BKTree<DataT> *pSubTree = NULL;
        if ( !_sub.find(distance, pSubTree) ) {
            DataBody<int32_t , BKTree<DataT>> _body;
            _body._key = distance;
            _body._value._node = BKNode<DataT>(data, len);
            _sub.insert(_body);
        } else {
            return pSubTree->addNode(data, len);
        }
    }

private:
    SortArray<int32_t, BKTree<DataT>> _sub;
    BKNode<DataT> _node;
};

#endif //MERCUR_BKTREE_H
