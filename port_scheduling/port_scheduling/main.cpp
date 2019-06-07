//
//  main.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include <iostream>
#include <worker/greedy_worker.h>
#include <worker/brute_force_worker.h>
#include <worker/random_worker.h>
#include "entities/ship.h"
#include "worker/worker_base.h"
#include "port.h"
int main(int argc, const char * argv[]) {
    
    std::cout<<
    "enter the port information:\n"
    "the number of dock(s):\n"
    "the number of lift(s):\n"
    "the number of vehicle(s):\n"
    "the number of store(s):\n"
    "vehicle transport time:\n"
    <<std::endl;
    int dock_num,lift_num,vehicle_num,store_num,transport_time;
    std::cin>>dock_num>>lift_num>>vehicle_num>>store_num>>transport_time;
    port* port1;
    port1 = new port(dock_num,lift_num,vehicle_num,store_num,transport_time);


    std::cout<<
    "enter the ship information:\n"
    "the number of ship(s):\n"
    <<std::endl;
    int n,m;
    std::cin>>n;
    for(int i=1;i<=n;i++){
        std::cout<<"enter the information of ship "+std::to_string(i)+":\n";
        std::cout<<"enter the in port time and out port time of the ship:"<<std::endl;
        std::cin>>lift_num>>vehicle_num;
        ship *tship = new ship(i,lift_num,vehicle_num);
        port1->load_ship(tship);
        std::cout<<"enter the number of cargoes on the ship:"<<std::endl;
        std::cin>>m;
        for(int j=1;j<=m;j++){
            std::cout<<j<<":enter the target store and lift time the cargo:"<<std::endl;
            int lt,ts;
            std::cin>>ts>>lt;
            tship->load_cargo(new cargo(i*100+j,ts,lt));
        }
    }
    port1->info();

    worker_base* workerBase = new worker_base(port1);
    workerBase->work();

    greedy_worker* greedyWorker = new greedy_worker(port1);
    greedyWorker->work();

    brute_force_worker* bruteForceWorker = new brute_force_worker(port1);
    bruteForceWorker->work(1e3);

    random_worker* randomWorker = new random_worker(port1);
    randomWorker->work(1e5);


    return 0;
}
