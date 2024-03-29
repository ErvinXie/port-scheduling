//
// Created by 谢威宇 on 2019-06-07.
//

#include "brute_force_worker.h"
#include <algorithm>
#include <iostream>

void brute_force_worker::work(long long limit) {
    long long total_permutation = 1, now = 0;
    for (long long i = 1; i <= cargo_rule.size(); i++) {
        total_permutation *= i;
    }
    std::cout<<"-----------permutation algorithm---------"<<std::endl;
    std::cout<<"Total:"<<limit<<" ways will be tried"<<std::endl;
    //try every possible way
    while (now<limit&&std::next_permutation(cargo_rule.begin(), cargo_rule.end())) {
        while (std::next_permutation(ship_rule.begin(), ship_rule.end())) {
            int time = port1->simulate_greedy(ship_rule, cargo_rule,transport_rule);
            //statistics about the time distribution
            time_distribute[time]++;
            if(time<best_time){
                best_time=time;
                best_cargo_rule=cargo_rule;
                best_ship_rule=ship_rule;
            }
        }
        now++;
        if (now % 100LL == 0) {
            std::cout << "--" << now * 1.0 / limit * 100 << "%--finished"
                      <<" present best:"<<best_time
                      << std::endl;

        }

    }
    finish_work();
}


brute_force_worker::brute_force_worker(port *port1) : worker_base(port1) {}

void brute_force_worker::finish_work() {
    std::cout<<"Time distribution:";
    std::for_each(time_distribute.begin(),time_distribute.end(),[](std::pair<int,long long> p){
        std::cout<<"Time["<<p.first<<"]\t"<<p.second<<std::endl;
    });

    worker_base::finish_work();
}
