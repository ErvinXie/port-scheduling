//
// Created by 谢威宇 on 2019-06-06.
//

#include "worker_base.h"

const static std::string TAG = "work_base::";

worker_base::worker_base(port *port1) : port1(port1) {

    for (int i = 0; i < port1->ships.size(); i++) {
        ship_rule.push_back(port1->ships[i]);
        for (int j = 0; j < port1->ships[i]->cargoes.size(); j++) {
            cargo_rule.push_back(port1->ships[i]->cargoes[j]);
            transport_rule.push_back(port1->ships[i]->cargoes[j]);
        }
    }
    std::cout << TAG << "ships and cargoes loaded!" << std::endl;

    startTime = clock();
}

void worker_base::work() {

    int time = port1->simulate_greedy(ship_rule, cargo_rule,transport_rule ,10);
    best_time = std::min(best_time, time);
    best_ship_rule = ship_rule;
    best_cargo_rule = cargo_rule;
    best_transport_rule = transport_rule;

    //finish
    finish_work();
}


void worker_base::generate_method() {

}

//display the best solution found
void worker_base::finish_work() {
    std::cout << "The Best TIME is " << best_time << std::endl;

    std::cout << "The best schedule is:";
    port1->simulate_greedy(best_ship_rule, best_cargo_rule,best_transport_rule, 10);
    std::cout << "\n";

    std::cout << "The best ship rule is:\n";
    for (int i = 0; i < best_ship_rule.size(); i++) {
        std::cout << best_ship_rule[i]->get_name() << "\n";
    }

    endTime = clock();
    std::cout << "Totle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << "Working Finished" << std::endl<<"--------------------------";

}
