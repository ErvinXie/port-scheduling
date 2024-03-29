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
    tship->parent_port = this;
}

//Print the information of all things recursively.
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

//Different log level signifies different logs.
//when log level = 0, there is completely no logs.
int port::simulate_greedy(std::vector<ship *> &ship_rule,
                          std::vector<cargo *> &cargo_rule,
                          std::vector<cargo *> &transport_rule,
                          int log_level) {

    int ship_now = 0;
    bool *cargo_lifted = new bool[cargo_rule.size()];
    for (int i = 0; i < cargo_rule.size(); i++) {
        cargo_lifted[i] = 0;
    }
    bool *cargo_transported = new bool[transport_rule.size()];
    for (int i = 0; i < transport_rule.size(); i++) {
        cargo_transported[i] = 0;
    }


    lift_order.clear();
    transport_order.clear();


    int time_now = 0;

    //push an empty event
    request_queue.push(new request(time_now, []() {}));
    while (request_queue.size()) {
        //do the work scheduled
        request_queue.top()->work();
        //update the time;
        time_now = request_queue.top()->time;
        request_queue.pop();

        //ship decking
        //if there are ships remained and decks available, start decking a ship
        while (ship_now < ship_rule.size() && deck_num) {
            //temporary pointer for this ship
            ship *this_ship = ship_rule[ship_now];
            //the next ship id ready to deck
            ship_now++;
            deck_num--;

            if (log_level >= 2) {
                std::cout << "----time:\t" << time_now << "\t"
                          << this_ship->get_name()
                          << "\tdecking"
                          << std::endl;
            }

            //set the left of ship in advance
            const int finish_time = time_now + this_ship->in_port_time;
            request_queue.push(new request(finish_time, [=, &ship_rule, this]() {
                //insert the ship to the deck
                this->ships_in_deck.insert(this_ship);
                //load the cargoes on the ship
                std::for_each(this_ship->cargoes.begin(), this_ship->cargoes.end(),
                              [this](cargo *this_cargo) {
                                  cargoes_on_deck.insert(this_cargo);
                              });

                if (log_level >= 2) {
                    //log the information
                    std::cout << "----time:\t" << finish_time << "\t"
                              << this_ship->get_name()
                              << "\tdecked"
                              << std::endl;
                }

            }));
        }

        //cargo lifting
        //if there are idling lift and the next demanded cargo already on the deck
        while (lift_num) {
            //find an available cargo
            cargo *this_cargo = nullptr;
            for (int i = 0; i < cargo_rule.size(); i++) {
                if (cargo_lifted[i] == 0 && cargoes_on_deck.count(cargo_rule[i])) {
                    this_cargo = cargo_rule[i];
                    cargo_lifted[i] = 1;
                    break;
                }
            }
            //if no available cargo was find
            if (this_cargo == nullptr) {
                break;
            }

            lift_num--;
            cargoes_on_deck.erase(this_cargo);
            lift_order.push_back(this_cargo);
            if (log_level >= 2) {

                std::cout << "----time:\t" << time_now << "\t"
                          << this_cargo->get_name()
                          << "\tlifting "
                          << std::endl;
            }

            //set the event when the lifting is finished
            const int finish_time = time_now + this_cargo->lift_time;
            request_queue.push(new request(finish_time, [=, this]() {
                //release the lift
                lift_num++;

                //move the cargo from deck to buffer

                this_cargo->parent_ship->on_deck_cargoes_num--;
                cargoes_in_buffer.insert(this_cargo);
                if (log_level >= 3) {
                    std::cout << "----time:\t" << finish_time << "\t"
                              << this_cargo->get_name()
                              << "\tlifted"
                              << std::endl;
                }
                //if the ship is empty, the ship will leave
                ship *this_ship = this_cargo->parent_ship;
                if (this_ship->on_deck_cargoes_num == 0) {

                    if (log_level >= 2) {
                        std::cout << "----time:\t" << finish_time << "\t"
                                  << this_ship->get_name()
                                  << "\tstart to leave"
                                  << std::endl;
                    }

                    const int leave_finish_time = finish_time + this_ship->out_port_time;
                    request_queue.push(new request(leave_finish_time, [=, this]() {
                        //release the deck
                        deck_num++;
                        ships_in_deck.erase(this_ship);
                        if (log_level >= 2) {
                            std::cout << "----time:\t" << leave_finish_time << "\t"
                                      << this_ship->get_name()
                                      << "\thas left the deck"
                                      << std::endl;
                        }
                    }));
                }
            }));
        }

        //cargo transport
        //if there are idling vehicle and available cargo, move it to the store
        while (vehicle_num && cargoes_in_buffer.size()) {
            //find an available cargo
            cargo *this_cargo = nullptr;
            for (int i = 0; i < transport_rule.size(); i++) {
                if (cargo_transported[i] == 0 && cargoes_in_buffer.count(transport_rule[i])) {
                    this_cargo = transport_rule[i];
                    cargo_transported[i] = 1;
                    break;
                }
            }
            //if no available cargo was find
            if (this_cargo == nullptr) {
                break;
            }
            //remove the cargo from the buffer
            cargoes_in_buffer.erase(this_cargo);
            vehicle_num--;
            transport_order.push_back(this_cargo);
            if (log_level >= 2) {
                std::cout << "----time:\t" << time_now << "\t"
                          << this_cargo->get_name()
                          << "\tstart transport "
                          << "to store "
                          << this_cargo->target_store_id
                          << std::endl;
            }

            //set the event when the transport is finished
            const int arrival_time = time_now + transport_time[this_cargo->target_store_id];
            request_queue.push(new request(arrival_time, [=, this]() {
                if (log_level >= 4) {
                    std::cout << "----time:\t" << arrival_time << "\t"
                              << this_cargo->get_name()
                              << "\ttransported "
                              << " to store "
                              << this_cargo->target_store_id
                              << std::endl;
                }
                cargoes_in_store.insert(this_cargo);


                const int back_time = arrival_time + transport_time[this_cargo->target_store_id];
                request_queue.push(new request(back_time, [=, this]() {
                    vehicle_num++;

                    if (log_level >= 5) {
                        std::cout << "----time:\t" << back_time << "\t"
                                  << this_cargo->get_name()
                                  << "\ttransported "
                                  << " to store "
                                  << this_cargo->target_store_id
                                  << " and came back"
                                  << std::endl;
                    }
                }));
            }));

        }
    }

    if (log_level >= 1) {
        std::cout << "Time used: " << time_now << std::endl;
    }

    //because the cargo rule is a priority rule,
    //the lift order is much more valuable,
    //so we need this
    if (log_level >= 2) {
        std::cout << "Lift Order:" << std::endl;
        for (int i = 0; i < lift_order.size(); i++) {
            std::cout << lift_order[i]->get_name() << std::endl;
        }
        std::cout << "Transport Order:" << std::endl;
        for (int i = 0; i < transport_order.size(); i++) {
            std::cout << transport_order[i]->get_name() << std::endl;
        }
    }

    delete cargo_lifted;

    //after simulation finished
    std::for_each(ship_rule.begin(), ship_rule.end(), [](ship *this_ship) {
        this_ship->on_deck_cargoes_num = this_ship->cargoes.size();
    });
    ships_in_deck.clear();
    cargoes_in_buffer.clear();
    cargoes_on_deck.clear();
    cargoes_in_store.clear();

    return time_now;
}


port::port(int deckNum, int liftNum, int vehicleNum, int storeNum, int transportTime) : deck_num(deckNum),
                                                                                        lift_num(liftNum),
                                                                                        vehicle_num(vehicleNum),
                                                                                        store_num(storeNum) {}

int
port::simulate(std::vector<ship *> &ship_rule, std::vector<cargo *> &cargo_rule, std::vector<cargo *> &transport_rule,
               int log_level) {
    int ship_now = 0;
    int cargo_now = 0;
    int transport_now = 0;
    int time_now = 0;


    //push an empty event
    request_queue.push(new request(time_now, []() {}));



    while (request_queue.size()) {
        //do the work scheduled
        request_queue.top()->work();
        //update the time;
        time_now = request_queue.top()->time;
        request_queue.pop();

        //ship decking
        //if there are ships remained and decks available, start decking a ship
        std::cout<<ship_now<<std::endl;
        while (ship_now < ship_rule.size() && deck_num) {
            //temporary pointer for this ship
            ship *this_ship = ship_rule[ship_now];
            //the next ship id ready to deck
            ship_now++;
            deck_num--;

            if (log_level >= 2) {
                std::cout << "----time:\t" << time_now << "\t"
                          << this_ship->get_name()
                          << "\tdecking"
                          << std::endl;
            }

            //set the left of ship in advance
            const int finish_time = time_now + this_ship->in_port_time;
            request_queue.push(new request(finish_time, [=, &ship_rule, this]() {
                //insert the ship to the deck
                this->ships_in_deck.insert(this_ship);
                //load the cargoes on the ship
                std::for_each(this_ship->cargoes.begin(), this_ship->cargoes.end(),
                              [this](cargo *this_cargo) {
                                  cargoes_on_deck.insert(this_cargo);
                              });
                if (log_level >= 2) {
                    //log the information
                    std::cout << "----time:\t" << finish_time << "\t"
                              << this_ship->get_name()
                              << "\tdecked"
                              << std::endl;
                }

            }));
        }

        //cargo lifting
        //if there are idling lift and the next demanded cargo already on the deck
        while (cargo_now < cargo_rule.size() && lift_num) {
            //get the demanded cargo
            cargo *this_cargo = cargo_rule[cargo_now];
            if (cargoes_on_deck.count(this_cargo) == 0) {
                //if this cargo is not ready
                break;
            }

            cargo_now++;
            lift_num--;
            cargoes_on_deck.erase(this_cargo);
            if (log_level >= 2) {
                lift_order.push_back(this_cargo);
                std::cout << "----time:\t" << time_now << "\t"
                          << this_cargo->get_name()
                          << "\tlifting "
                          << std::endl;
            }

            //set the event when the lifting is finished
            const int finish_time = time_now + this_cargo->lift_time;
            request_queue.push(new request(finish_time, [=, this]() {
                //release the lift
                lift_num++;

                //move the cargo from deck to buffer

                this_cargo->parent_ship->on_deck_cargoes_num--;
                cargoes_in_buffer.insert(this_cargo);
                if (log_level >= 3) {
                    std::cout << "----time:\t" << finish_time << "\t"
                              << this_cargo->get_name()
                              << "\tlifted"
                              << std::endl;
                }
                //if the ship is empty, the ship will leave
                ship *this_ship = this_cargo->parent_ship;
                if (this_ship->on_deck_cargoes_num == 0) {

                    if (log_level >= 2) {
                        std::cout << "----time:\t" << finish_time << "\t"
                                  << this_ship->get_name()
                                  << "\tstart to leave"
                                  << std::endl;
                    }

                    const int leave_finish_time = finish_time + this_ship->out_port_time;
                    request_queue.push(new request(leave_finish_time, [=, this]() {
                        //release the deck
                        deck_num++;
                        ships_in_deck.erase(this_ship);
                        if (log_level >= 2) {
                            std::cout << "----time:\t" << leave_finish_time << "\t"
                                      << this_ship->get_name()
                                      << "\thas left the deck"
                                      << std::endl;
                        }
                    }));
                }
            }));
        }


        //cargo transport
        //if there are idling vehicle and available cargo, move it to the store
        while (transport_now < transport_rule.size() && vehicle_num) {
            //get the demanded cargo
            cargo *this_cargo = transport_rule[transport_now];
            if (cargoes_in_buffer.count(this_cargo) == 0) {
                //if this cargo is not ready
                break;
            }
            transport_now++;

            //remove the cargo from the buffer
            cargoes_in_buffer.erase(this_cargo);
            vehicle_num--;

            if (log_level >= 2) {
                transport_order.push_back(this_cargo);
                std::cout << "----time:\t" << time_now << "\t"
                          << this_cargo->get_name()
                          << "\tstart transport "
                          << "to store "
                          << this_cargo->target_store_id
                          << std::endl;
            }

            //set the event when the transport is finished
            const int arrival_time = time_now + transport_time[this_cargo->target_store_id];
            request_queue.push(new request(arrival_time, [=, this]() {
                if (log_level >= 4) {
                    std::cout << "----time:\t" << arrival_time << "\t"
                              << this_cargo->get_name()
                              << "\ttransported "
                              << " to store "
                              << this_cargo->target_store_id
                              << std::endl;
                }
                cargoes_in_store.insert(this_cargo);


                const int back_time = arrival_time + transport_time[this_cargo->target_store_id];
                request_queue.push(new request(back_time, [=, this]() {
                    vehicle_num++;
                    if (log_level >= 5) {
                        std::cout << "----time:\t" << back_time << "\t"
                                  << this_cargo->get_name()
                                  << "\ttransported "
                                  << " to store "
                                  << this_cargo->target_store_id
                                  << " and came back"
                                  << std::endl;
                    }
                }));
            }));

        }
    }

    if (log_level >= 1) {
        std::cout << "Time used: " << time_now << std::endl;
    }

    //because the cargo rule is a priority rule,
    //the lift order is much more valuable,
    //so we need this
    if (log_level >= 2) {
        std::cout << "Lift Order:" << std::endl;
        for (int i = 0; i < lift_order.size(); i++) {
            std::cout << lift_order[i]->get_name() << std::endl;
        }
        std::cout << "Transport Order:" << std::endl;
        for (int i = 0; i < transport_order.size(); i++) {
            std::cout << transport_order[i]->get_name() << std::endl;
        }
    }

    if (cargoes_in_store.size() != cargo_rule.size()) {
        time_now = 0x3f3f3f3f - 1;
    }
    //after simulation finished
    std::for_each(ship_rule.begin(), ship_rule.end(), [](ship *this_ship) {
        this_ship->on_deck_cargoes_num = this_ship->cargoes.size();
    });
    ships_in_deck.clear();
    cargoes_in_buffer.clear();
    cargoes_on_deck.clear();
    cargoes_in_store.clear();



    return time_now;
}

request::request(int time, const std::function<void()> &work) : time(time), work(work) {}
