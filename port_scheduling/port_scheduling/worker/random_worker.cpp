//
// Created by 谢威宇 on 2019-06-07.
//

#include "random_worker.h"
#include <random>
#include <ctime>
random_worker::random_worker(port *port1) : brute_force_worker(port1) {}

void random_worker::work(long long int limit) {
    //try every possible way
    std::cout<<"-----------random algorithm---------"<<std::endl;
    std::cout << "Total:" << limit << " ways will be tried" << std::endl;
    int now = 0;
    unsigned seed = time(nullptr);
    while (now < limit ) {
        std::shuffle(cargo_rule.begin(), cargo_rule.end(),std::default_random_engine (seed));
        while (std::next_permutation(ship_rule.begin(), ship_rule.end())) {
            int time = port1->simulate(ship_rule, cargo_rule);
            //statistics about the time distribution
            time_distribute[time]++;
            if (time < best_time) {
                best_time = time;
                best_cargo_rule = cargo_rule;
                best_ship_rule = ship_rule;
            }
        }
        now++;
        if (now % 100LL == 0) {
            std::cout << "--" << now * 1.0 / limit * 100 << "%--finished"
                      << " present best:" << best_time
                      << std::endl;

        }

    }

    finish_work();
}
