//
// Created by diqiu on 2016/5/18.
//

#ifndef MERCUR_SPACETREE_H_H
#define MERCUR_SPACETREE_H_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "MaxHeap.h"
#include <string.h>

#define  MIN_LEAF_SIZE  4

template <class DataT>
class Point {
public:
    Point()
    {
        _dimension = 0;
        _coordinate = NULL;
    }

    Point(int32_t dimension, DataT *coordinate = NULL)
    {
        assert(dimension != 0);
        _dimension = dimension;
        _coordinate = (DataT *)malloc(sizeof(DataT) * dimension);
        if ( coordinate != NULL ) {
            for ( int32_t i = 0; i < dimension; i++ ) {
                _coordinate[i]  = coordinate[i];
            }
        }
    }

    Point<DataT> operator=(const Point<DataT> &point)
    {
        if ( &point == this ) {
            return *this;
        }
        if ( _coordinate ) {
            free(_coordinate);
            _coordinate = NULL;
        }
        _dimension = point._dimension;
        _coordinate = (DataT *)malloc(sizeof(DataT) * point._dimension);
        for ( int32_t i = 0; i < point._dimension; i++ ) {
            _coordinate[i] = point._coordinate[i];
        }
        strncpy(_buf, point._buf, 10);
        return *this;
    }

    Point(const Point<DataT> &point)
    {
        _dimension = point._dimension;
        _coordinate = (DataT *)malloc(sizeof(DataT) * point._dimension);
        for ( int32_t i = 0; i < point._dimension; i++ ) {
            _coordinate[i] = point._coordinate[i];
        }
        strncpy(_buf, point._buf, 10);
    }

    DataT getCoordinateByAxis(int32_t i)
    {
        return _coordinate[i];
    }

    int32_t getDimension()
    {
        return _dimension;
    }

    bool setCoordinateByAxis(int32_t axis, DataT val)
    {
        assert(_coordinate != NULL);
        if ( axis + 1 > _dimension ) {
            return false;
        }
        _coordinate[axis]  = val;
        return true;
    }

    ~Point()
    {
        if ( _coordinate ) {
            free(_coordinate);
            _coordinate = NULL;
        }
    }

    void print()
    {
        /*
        for ( int32_t i = 0; i < _dimension; i++ )  {
            std::cout << _coordinate[i] << "\t";
        }
        std::cout << std::endl;
         */
        std::cout << _buf << std::endl;
    }

    void setData(const char *p)
    {
        memset(_buf, 0, 10);
        strncpy(_buf, p, 10);
    }

private:
    DataT *_coordinate;
    int32_t _dimension;
    char _buf[10];
};

template <class DataT>
class NeighborPoint {
public:
    NeighborPoint(Point<DataT> point, DataT distance = 0)
    {
        _distance = distance;
        _point = Point<DataT>(point);
    }

    NeighborPoint()
    {
        _distance = 0;
    }

    NeighborPoint(const NeighborPoint<DataT> &neighborPoint)
    {
        _distance = neighborPoint._distance;
        _point = neighborPoint._point;
    }

    ~NeighborPoint()
    {
    }

    Point<DataT> &getPoint()
    {
        return _point;
    }

    NeighborPoint<DataT> operator=(const NeighborPoint<DataT> &neighborPoint)
    {
        if ( &neighborPoint == this ) {
            return *this;
        }
        _distance = neighborPoint._distance;
        _point = neighborPoint._point;
        return *this;
    }

    bool operator< (const NeighborPoint<DataT> &neighborPoint)
    {
        if ( _distance < neighborPoint._distance ) {
            return true;
        }
        return false;
    }

    bool operator> (const NeighborPoint<DataT> &neighborPoint)
    {
        if ( _distance > neighborPoint._distance ) {
            return true;
        }
        return false;
    }

    void print()
    {
        std::cout << "distance is " << _distance << "\t";
        _point.print();
    }

    std::ostream &operator<<(NeighborPoint<DataT> &neighborPoint)
    {
        neighborPoint.print();
    }

private:
    Point<DataT> _point;
    DataT   _distance;
};

template <class DataT>
class SpaceTree {
public:
    SpaceTree(int32_t dimension)
    {
        _dimension = dimension;
        _radius = 0;
        _leftChild = NULL;
        _rightChild = NULL;
        _pivot = Point<DataT>(dimension);
    }

    ~SpaceTree()
    {
        delete _leftChild;
        delete _rightChild;
        _points.clear();
    }

    void addPoint(Point<DataT> &point)
    {
        _points.push_back(point);
    }

    bool getCentroid(Point<DataT> &centroidPoint)
    {
        if (centroidPoint.getDimension() != _dimension) {
            assert(false);
            return false;
        }
        int32_t count = _points.size();
        for (int32_t i = 0; i < _dimension; i++) {
            DataT sum = 0;
            for (int32_t j = 0; j < count; j++) {
                sum += _points[j].getCoordinateByAxis(i);
            }
            centroidPoint.setCoordinateByAxis(i, sum / count);
        }
    }

    float calcEuclideanDistance(Point<DataT> &a, Point<DataT> &b)
    {
        assert(a.getDimension() == b.getDimension());
        int32_t dimension = a.getDimension();
        float sum = 0;
        for (int32_t i = 0; i < dimension; i++) {
            DataT temp = (a.getCoordinateByAxis(i) - b.getCoordinateByAxis(i));
            sum += (float) temp * temp;
        }
        return sqrtf(sum);
    }

    Point<DataT> farthestFromThisPoint(Point<DataT> &point)
    {
        assert(_points.size() > 0 && point.getDimension() == _dimension);
        float maxDistance = 0;
        int32_t found = -1;
        for ( int32_t i = 0; i < _points.size(); i++ ) {
            float distance = calcEuclideanDistance(_points[i], point);
            if ( maxDistance < distance ) {
                maxDistance = distance;
                found = i;
            }
        }
        return _points[found];
    }

    bool isLeaf() const
    {
        if ( _leftChild == NULL && _rightChild == NULL ) {
            return true;
        }
        return false;
    }

    // please make sure you have inserted all the points by calling function named addPoint
    void build()
    {
        getCentroid(_pivot);
        float maxDistance = 0.0;
        for ( int32_t i = 0; i < _points.size(); i++ ) {
            float distance = calcEuclideanDistance(_points[i], _pivot);
            if ( maxDistance < distance ) {
                maxDistance = distance;
            }
        }
        _radius = maxDistance;
        if ( _points.size() <= MIN_LEAF_SIZE ) {
            return;
        }

        Point<DataT> leftChild = farthestFromThisPoint(_pivot);
        Point<DataT> rightChild = farthestFromThisPoint(leftChild);
        if ( _leftChild == NULL ) {
            _leftChild = new SpaceTree<DataT>(_dimension);
        }
        if ( _rightChild == NULL ) {
            _rightChild = new SpaceTree<DataT>(_dimension);
        }
        for ( int32_t i = 0; i < _points.size(); i++ ) {
            if ( calcEuclideanDistance(_points[i], leftChild) <= calcEuclideanDistance(_points[i], rightChild) ) {
                _leftChild->addPoint(_points[i]);
            } else {
                _rightChild->addPoint(_points[i]);
            }
        }
        _leftChild->build();
        _rightChild->build();
    }

    void searchPoint(Point<DataT> &point, MaxHeap<NeighborPoint<DataT>> &store, SpaceTree<DataT> *node)
    {
        if ( node == NULL ) {
            return;
        }
        NeighborPoint<DataT> first;
        if ( store.isFull() ) {
            store.first(first);
            if ( calcEuclideanDistance(point, node->_pivot) - node->_radius >= calcEuclideanDistance(point, first.getPoint()) ) {
                return;
            }
        }
        if ( node->isLeaf() ) {
            for ( int32_t i = 0; i < node->_points.size(); i++ ) {
                DataT distance1 = calcEuclideanDistance(point, node->_points[i]);
                NeighborPoint<DataT> neighborPoint(node->_points[i], distance1);
                if ( !store.isFull() || ( store.first(first) && distance1 <= calcEuclideanDistance(point, first.getPoint()) ) ) {
                    store.insertForce(neighborPoint);
                }
            }
        } else {
            float disLeft = 0, disRight = 0;
            if ( node->_leftChild != NULL ) {
                disLeft = calcEuclideanDistance(node->_pivot, node->_leftChild->_pivot);
            }
            if ( node->_rightChild != NULL ) {
                disRight = calcEuclideanDistance(node->_pivot, node->_rightChild->_pivot);
            }
            if ( disLeft < disRight ) {
                searchPoint(point, store, node->_leftChild);
                searchPoint(point, store, node->_rightChild);
            } else {
                searchPoint(point, store, node->_rightChild);
                searchPoint(point, store, node->_leftChild);
            }
        }
    }

    void print(SpaceTree<DataT> *T)
    {
        for ( int32_t i = 0; i < _points.size(); i++ ) {
            _points[i].print();
        }
        if ( T->_leftChild ) {
            print(T->_leftChild);
        }
        if ( T->_rightChild ) {
            print(T->_rightChild);
        }
    }

private:
    // the dimension of space
    int32_t _dimension;
    std::vector<Point<DataT>> _points;
    SpaceTree<DataT> *_leftChild;
    SpaceTree<DataT> *_rightChild;
    Point<DataT> _pivot;
    float _radius;
};

#endif //MERCUR_SPACETREE_H_H
