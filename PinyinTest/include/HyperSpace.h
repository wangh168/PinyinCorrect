//
// Created by diqiu on 2016/5/20.
//

#ifndef MERCUR_HYPERSPACE_H
#define MERCUR_HYPERSPACE_H

#include <iostream>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "SpaceTree.h"
#include "BKTree.h"

static int DecomposeContentIntoFields(char *buf, const char *fieldSeprator, std::vector<std::string> &vectorFields)
{
    char *pTemp = strtok(buf, fieldSeprator);
    while (pTemp) {
        vectorFields.push_back(std::string(pTemp));
        pTemp = strtok(NULL, fieldSeprator);
    }
    return 0;
}

class HyperSpace {
public:
    HyperSpace(std::string filename)
    {
        _xAxis['q' - 'a'] = 0.7;
        _xAxis['w' - 'a'] = 2.4;
        _xAxis['e' - 'a'] = 4.1;
        _xAxis['r' - 'a'] = 5.8;
        _xAxis['t' - 'a'] = 7.5;
        _xAxis['y' - 'a'] = 9.2;
        _xAxis['u' - 'a'] = 10.9;
        _xAxis['i' - 'a'] = 12.6;
        _xAxis['o' - 'a'] = 14.3;
        _xAxis['p' - 'a'] = 16.0;

        _xAxis['a' - 'a'] = 1.2;
        _xAxis['s' - 'a'] = 2.9;
        _xAxis['d' - 'a'] = 4.6;
        _xAxis['f' - 'a'] = 6.3;
        _xAxis['g' - 'a'] = 8.0;
        _xAxis['h' - 'a'] = 9.7;
        _xAxis['j' - 'a'] = 11.4;
        _xAxis['k' - 'a'] = 13.1;
        _xAxis['l' - 'a'] = 14.8;

        _xAxis['z' - 'a'] = 2.1;
        _xAxis['x' - 'a'] = 3.8;
        _xAxis['c' - 'a'] = 5.5;
        _xAxis['v' - 'a'] = 7.2;
        _xAxis['b' - 'a'] = 8.9;
        _xAxis['n' - 'a'] = 10.6;
        _xAxis['m' - 'a'] = 12.3;

        _yAxis['q' - 'a'] = 3;
        _yAxis['w' - 'a'] = 3;
        _yAxis['e' - 'a'] = 3;
        _yAxis['r' - 'a'] = 3;
        _yAxis['t' - 'a'] = 3;
        _yAxis['y' - 'a'] = 3;
        _yAxis['u' - 'a'] = 3;
        _yAxis['i' - 'a'] = 3;
        _yAxis['o' - 'a'] = 3;
        _yAxis['p' - 'a'] = 3;

        _yAxis['a' - 'a'] = 2;
        _yAxis['s' - 'a'] = 2;
        _yAxis['d' - 'a'] = 2;
        _yAxis['f' - 'a'] = 2;
        _yAxis['g' - 'a'] = 2;
        _yAxis['h' - 'a'] = 2;
        _yAxis['j' - 'a'] = 2;
        _yAxis['k' - 'a'] = 2;
        _yAxis['l' - 'a'] = 2;

        _yAxis['z' - 'a'] = 1;
        _yAxis['x' - 'a'] = 1;
        _yAxis['c' - 'a'] = 1;
        _yAxis['v' - 'a'] = 1;
        _yAxis['b' - 'a'] = 1;
        _yAxis['n' - 'a'] = 1;
        _yAxis['m' - 'a'] = 1;

        _bkTree = new BKTree<char>();
        for ( int32_t i = 0; i < 6; i++ ) 
        {
            _spaceTree[i] = new SpaceTree<float>((i+1) * 2);
        }
//        std::string filename = "E:/OneDrive/myCode/Mercur/pinyin.txt";
        FILE *f = fopen(filename.c_str(), "r");
        if ( f )
        {
            std::vector<std::string> fields;
            char buf[1024] = {0};
            while ( fgets(buf, 1024, f) )
            {
                fields.clear();
                DecomposeContentIntoFields(buf, "\n\t\r ", fields);
                assert(fields.size()==2);
                int32_t  size = fields[0].length();
                const char *str = fields[0].c_str();
                Point<float> point(size*2);
                for ( int32_t i = 0; i < size * 2; i++ ) 
                {
                    point.setCoordinateByAxis(i * 2, _xAxis[str[i] - 'a']);
                    point.setCoordinateByAxis(i * 2 + 1, _yAxis[str[i] - 'a']);
                }
                point.setData(str);
                _spaceTree[size-1]->addPoint(point);
                _bkTree->addNode((char *)str, size);
            }

            for ( int32_t i = 0; i < 6; i++ )
            {
                _spaceTree[i]->build();
            }

            fclose(f);
        }
    }

    void search(std::string str)
    {
        int32_t size = str.size();
        Point<float> point(size * 2);
        for ( int32_t i = 0; i < size; i++ ) 
        {
            point.setCoordinateByAxis(i * 2, _xAxis[str[i] - 'a']);
            point.setCoordinateByAxis(i * 2 + 1, _yAxis[str[i] - 'a']);
        }
        NeighborPoint<float> neighborPoint(point);
        MaxHeap<NeighborPoint<float>> maxHeap(3);
        _spaceTree[size - 1]->searchPoint(point, maxHeap, _spaceTree[size - 1]);
        NeighborPoint<float> first;
        while ( maxHeap.first(first) )
        {
            maxHeap.out(first);
            first.print();
        }

        std::vector<BKNode<char>> vec;
        _bkTree->search((char *)str.c_str(), str.length(),  1, vec);
        std::vector<std::string> store;
        for ( std::vector<BKNode<char>>::iterator it = vec.begin(); it != vec.end(); it++ ) {
            std::string str;
            for ( int32_t i = 0; i < it->getLen(); i++ ) {
                str.push_back(it->getData()[i]);
            }
            store.push_back(str);
            std::cout << str.c_str() << std::endl;
        }
    }

    ~HyperSpace()
    {
        for ( int32_t i = 0; i < 6; i++ ) {
            delete _spaceTree[i];
        }
    }

private:
    float _xAxis[26];
    float _yAxis[26];
    SpaceTree<float> *_spaceTree[6];
    BKTree<char> *_bkTree;
};

#endif //MERCUR_HYPERSPACE_H
