//
//  port.hpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#ifndef port_hpp
#define port_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "ship.h"
#include <set>
#include <queue>
#include <functional>

struct request {
    int time;
    std::function<void()> work;

    request(int time, const std::function<void()> &work);

};

struct request_compare {
    bool operator()(request *a, request *b) {
        return a->time > b->time;
    }
};

class port {
public:
    //initial attributes
    int deck_num;
    int lift_num;
    int vehicle_num;
    int store_num;

    int transport_time[10] = {};

    std::vector<ship *> ships;

    //working attributes
    std::set<ship *> ships_in_deck;

    std::set<cargo *> cargoes_on_deck;
    std::set<cargo *> cargoes_in_buffer;
    std::set<cargo *> cargoes_in_store;
    std::priority_queue<request *, std::vector<request *>, request_compare> request_queue;


    //Methods
    port() {}

    port(int deckNum, int liftNum, int vehicleNum, int storeNum, int transportTime);

    virtual ~port();


    void load_ship(ship *tship);

    int simulate(std::vector<cargo *> &, std::vector<cargo *> &, int log_level = 0);

    int simulate_greedy(std::vector<ship *> &, std::vector<cargo *> &, std::vector<cargo *> &, int log_level = 0);

    //Debug Methods
    void info();

    std::vector<cargo *> lift_order;
    std::vector<cargo *> transport_order;

};

#endif /* port_hpp */
