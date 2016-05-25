// PinyinTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "include/DLV.h"
#include "include/BKTree.h"
#include "include/SpaceTree.h"
#include "include/HyperSpace.h"
#include "include/MaxHeap.h"
#include <stdint.h>

using namespace std;

void test_maxheap()
{
    MaxHeap<int32_t> maxHeap(100);
    maxHeap.insert(5);
    maxHeap.insert(8);
    maxHeap.insert(9);
    maxHeap.insert(11);
    maxHeap.insert(1);
    maxHeap.insert(4);
    maxHeap.insert(3);
    maxHeap.insert(6);
    maxHeap.insert(3);
    maxHeap.insert(-1);
    maxHeap.insert(9);
    maxHeap.print();
    MaxHeap<int32_t> c = maxHeap;
    c.print();
    int32_t tmp;
    while (c.out(tmp)) {
        std::cout << tmp << std::endl;
    }
}

void test_bktree()
{
    BKTree<char> bkTree;
    bkTree.addNode("shuang", (int32_t)6);
    bkTree.addNode("gong", (int32_t)4);
    bkTree.addNode("liang", (int32_t)5);
    bkTree.addNode("meng", (int32_t)4);
    bkTree.addNode("men", (int32_t)3);
    bkTree.addNode("lang", (int32_t)4);
    bkTree.addNode("chuang", (int32_t)6);
    bkTree.addNode("chong", (int32_t)6);
    char buf[10] = { 0 };
    std::vector<BKNode<char>> out;
    while (1) {
        memset(buf, 0, 10);
        std::cout << "please input a string:";
        std::cin >> buf;
        std::cout << std::endl;
        std::cout << "the correct string is ";
        bkTree.search(buf, strlen(buf), 1, out);
        std::cout << std::endl;
    }
}

void test_levdistance()
{
    DLV<char> dlv;
    char strIn1[10];
    char strIn2[10];
    while (1) {
        memset(strIn1, 0, sizeof(strIn1));
        memset(strIn2, 0, sizeof(strIn2));
        std::cout << "Please input two string:" << std::endl;
        std::cin >> strIn1;
        std::cin >> strIn2;
        std::cout << "The distance is " << dlv.calculateDistance(strIn1, strlen(strIn1), strIn2, strlen(strIn2)) << std::endl;
    }
}

void test_point()
{
    int32_t dimension = 4;
    Point<float> point(dimension);
    for (int32_t i = 0; i < dimension; i++) {
        point.setCoordinateByAxis(i, 1.0 + i);
    }
    point.print();
    Point<float> point2 = point;
    point2.print();
}

void test_spacetree()
{
    int32_t dimension = 2;
    SpaceTree<float> spaceTree(dimension);
    Point<float> point(dimension);
    for (int32_t i = 0; i < 10; i++) {
        for (int32_t j = 0; j < dimension; j++) {
            point.setCoordinateByAxis(j, i + 1.0);
        }
        spaceTree.addPoint(point);
    }
    spaceTree.build();
    Point<float> myPoint(dimension);
    for (int32_t i = 0; i < dimension; i++) {
        myPoint.setCoordinateByAxis(i, i + 5.5);
    }
    NeighborPoint<float> neighborPoint(myPoint);
    MaxHeap<NeighborPoint<float>> maxHeap(3);
    spaceTree.searchPoint(myPoint, maxHeap, &spaceTree);
    maxHeap.print();
}

void test_neighborpoint()
{
    int32_t dismention = 4;
    Point<float> point(dismention);
    for (int32_t i = 0; i < dismention; i++) {
        point.setCoordinateByAxis(i, i + 1.0);
    }
    NeighborPoint<float> neighborPoint(point, 3.55);
    point.setCoordinateByAxis(0, 5);
    NeighborPoint<float> neighborPoint2(point, 5);
    if (neighborPoint > neighborPoint2) {
        neighborPoint.print();
    }
    else {
        neighborPoint2.print();
    }
}

void test_hyperspace()
{
    std::string filename = "./pinyin.txt";
    //    std::string filename = "~/Documents/OneDrive/myCode/Mercur/pinyin.txt";
    HyperSpace *p = new HyperSpace(filename);
    char buf[1024] = { 0 };
    while (1)  {
        std::cout << "Please input a string not longer than six:";
        std::cin >> buf;
        buf[6] = 0;
        if (strcmp(buf, "exit") == 0) {
            break;
        }
        p->search(buf);
    }
    delete p;
}

int _tmain(int argc, _TCHAR* argv[])
{

    cout << "Hello, World!" << endl;
    //test_bktree();
    //    test_maxheap();
    //    test_point();
    //    test_spacetree();
    //    test_neighborpoint();

    test_hyperspace();
    system("pause");
	return 0;
}

