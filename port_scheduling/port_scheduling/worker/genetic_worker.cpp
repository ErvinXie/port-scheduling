//
// Created by 谢威宇 on 2019-06-07.
//

#include "genetic_worker.h"
#include <random>

genetic_worker::genetic_worker(port *port1) : worker_base(port1) {}

void genetic_worker::work(int population,int max_generation) {
    //initialize the group
    get_first_generation(population);
    










    finish_work();
}

void genetic_worker::finish_work() {
    worker_base::finish_work();
}

void genetic_worker::get_first_generation(int population) {
    group.clear();
    unsigned seed = time(nullptr);
    for(int i=0;i<population;i++){
        std::shuffle(cargo_rule.begin(),cargo_rule.end(),std::default_random_engine (seed));
        group.push_back(cargo_rule);
    }
}
