//
// Created by 谢威宇 on 2019-06-07.
//

#include "greedy_worker.h"

greedy_worker::greedy_worker(port *port1) : worker_base(port1) {}

void greedy_worker::generate_method() {
    worker_base::generate_method();
    //sort by the total time of a ship

    std::sort(ship_rule.begin(), ship_rule.end(), [](ship *a, ship *b) {
        std::sort(a->cargoes.begin(), a->cargoes.end(), [](cargo *a, cargo *b) {
            return a->lift_time < b->lift_time;
        });

        int ta = a->in_port_time + (*a->cargoes.begin())->lift_time, //+ a->out_port_time,
                tb = b->in_port_time + (*b->cargoes.begin())->lift_time; //+ b->out_port_time;
//        for (int i = 0; i < a->cargoes.size(); i++) {
//            ta += a->cargoes[i]->lift_time;
//        }
//        for (int i = 0; i < b->cargoes.size(); i++) {
//            tb += b->cargoes[i]->lift_time;
//        }
        return ta < tb;
    });

//    std::for_each(ship_rule.begin(),ship_rule.end(),[=](ship*a){
//        std::sort(a->cargoes.begin(),a->cargoes.end(),[](cargo *a, cargo *b) {
//            return a->lift_time < b->lift_time;
//        });
//        if(a->in_port_time+*a->cargoes.begin()<(*ship_rule.begin())->in_port_time+*(*ship_rule.begin())->cargoes.begin()){
//            std::swap(a,*ship_rule.begin());
//        }
//    });

    std::sort(cargo_rule.begin(), cargo_rule.end(), [](cargo *a, cargo *b) {
        return a->lift_time < b->lift_time;
    });
    std::sort(transport_rule.begin(), transport_rule.end(), [](cargo *a, cargo *b) {
        return a->parent_ship->parent_port->transport_time[a->target_store_id] <
               b->parent_ship->parent_port->transport_time[b->target_store_id];
    });
}


void greedy_worker::work() {
    std::cout << "-----------greedy algorithm---------" << std::endl;
    std::sort(cargo_rule.begin(), cargo_rule.end(), [](cargo *a, cargo *b) {
        return a->lift_time < b->lift_time;
    });
    std::sort(transport_rule.begin(), transport_rule.end(), [](cargo *a, cargo *b) {
        return a->parent_ship->parent_port->transport_time[a->target_store_id] <
               b->parent_ship->parent_port->transport_time[b->target_store_id];
    });

    int time = port1->simulate_greedy(ship_rule, cargo_rule, transport_rule);
    if (time < best_time) {
        best_time = time;
        best_ship_rule = ship_rule;
        best_cargo_rule = cargo_rule;
        best_transport_rule = transport_rule;
    }

    //method ----1
    std::sort(ship_rule.begin(), ship_rule.end(), [](ship *a, ship *b) {
        std::sort(a->cargoes.begin(), a->cargoes.end(), [](cargo *a, cargo *b) {
            return a->lift_time < b->lift_time;
        });

        int ta = a->in_port_time + (*a->cargoes.begin())->lift_time,
                tb = b->in_port_time + (*b->cargoes.begin())->lift_time;
        return ta < tb;
    });
    time = port1->simulate_greedy(ship_rule, cargo_rule, transport_rule);
    if (time < best_time) {
        best_time = time;
        best_ship_rule = ship_rule;
        best_cargo_rule = cargo_rule;
        best_transport_rule = transport_rule;
    }

    //method ----2
    std::sort(ship_rule.begin(), ship_rule.end(), [](ship *a, ship *b) {
        int ta = a->in_port_time ,
                tb = b->in_port_time ;
        for (int i = 0; i < a->cargoes.size(); i++) {
            ta += a->cargoes[i]->lift_time;
        }
        for (int i = 0; i < b->cargoes.size(); i++) {
            tb += b->cargoes[i]->lift_time;
        }
        return ta < tb;
    });
    time = port1->simulate_greedy(ship_rule, cargo_rule, transport_rule);
    if (time < best_time) {
        best_time = time;
        best_ship_rule = ship_rule;
        best_cargo_rule = cargo_rule;
        best_transport_rule = transport_rule;
    }

    //method ----3

    std::sort(ship_rule.begin(), ship_rule.end(), [](ship *a, ship *b) {
        int ta = a->in_port_time + a->out_port_time,
                tb = b->in_port_time + b->out_port_time;
        return ta < tb;
    });
    time = port1->simulate_greedy(ship_rule, cargo_rule, transport_rule);
    if (time < best_time) {
        best_time = time;
        best_ship_rule = ship_rule;
        best_cargo_rule = cargo_rule;
        best_transport_rule = transport_rule;
    }

    std::sort(ship_rule.begin(),ship_rule.end());
    while (std::next_permutation(ship_rule.begin(),ship_rule.end())){
        time = port1->simulate_greedy(ship_rule, cargo_rule, transport_rule);
        if (time < best_time) {
            best_time = time;
            best_ship_rule = ship_rule;
            best_cargo_rule = cargo_rule;
            best_transport_rule = transport_rule;
        }
    }

    finish_work();
}
