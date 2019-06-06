//
//  port.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include "port.h"


void port::load_ship(ship *tship) {
    ships.push_back(tship);
}

void port::info() {
    std::cout << "Port information:\n";
    for (int i = 0; i < ships.size(); i++) {
        ships[i]->info();
    }
}

port::~port() {
    for (int i = 0; i < ships.size(); i++) {
        delete ships[i];
    }
    delete this;
}

void port::simulate(std::vector<ship *> &ship_rule, std::vector<cargo *> &cargo_rule) {
    int ship_now = 0;
    int cargo_now = 0;
    int time_now = 0;

    //deck the first ship scheduled
    request_queue.push(new request(time_now, [=,&ship_now, &ship_rule, this]() {
        std::cout << "----time[0] " << ship_rule[ship_now]->get_name() << " decking" << std::endl;
        ship_now++;
        deck_num--;
        //set the left of ship in advance
        request_queue.push(new request(time_now + ship_rule[ship_now]->in_port_time, [=,&ship_now, &ship_rule, this]() {
            this->ships_in_deck.insert(ship_rule[ship_now]);
            std::cout << "----time[" << time_now + ship_rule[ship_now]->in_port_time << "] "
                      << ship_rule[ship_now]->get_name() << " decked" << std::endl;

        }));
    }));

    while (request_queue.size()) {
        request_queue.top()->work();
        time_now=request_queue.top()->time;
        request_queue.pop();
        //if there are ships remained and decks available, start decking a ship
        while (ship_now<ship_rule.size()&&deck_num){
            std::cout << "----time["<<time_now<<"] "
            << ship_rule[ship_now]->get_name() << " decking" << std::endl;
            ship_now++;
            deck_num--;
            request_queue.push(new request(time_now + ship_rule[ship_now]->in_port_time, [=,&ship_now, &ship_rule, this]() {
                this->ships_in_deck.insert(ship_rule[ship_now]);
                std::cout << "----time[" << time_now + ship_rule[ship_now]->in_port_time << "] "
                          << ship_rule[ship_now]->get_name() << " decked" << std::endl;

            }));
        }






    }
}

request::request(int time, const std::function<void()> &work) : time(time), work(work) {}
