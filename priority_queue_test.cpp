//
// Created by lhy31 on 2023/8/28.
//#include "priority_queue.h"
#include <iostream>
#include"priority_queue.h"
#include "vector.h"
using namespace lhy;
int main() {
    lhy::priority_queue<int> pq;

    pq.push(3);
    pq.push(1);
    pq.push(4);

    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }

    std::cout << "\n";

    vector<int> arr = {5, 2, 7, 9, 6};
    lhy::priority_queue<int> pq2((int*)arr.begin(), (int*)arr.end());

    while (!pq2.empty()) {
        std::cout << pq2.top() << " ";
        pq2.pop();
    }

    return 0;
}