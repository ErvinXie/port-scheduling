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
#include <worker/greedy_random_worker.h>
#include <worker/genetic_worker.h>
#include <worker/pso_worker.h>
#include "entities/ship.h"
#include "worker/worker_base.h"
#include "port.h"
void temp_tool(){
    std::string a,la;
    int b,lb;
    while (std::cin>>a>>b) {
        if(a==la){
            std::cout<<" \t"<<b-lb;
        }else{
            std::cout<<std::endl<<a<<" \t"<<b;
        }
        la=a;
        lb=b;
    }
}
int main(int argc, const char *argv[]) {
//    temporary function for data analysis
//    temp_tool();

    std::cout <<
              "enter the port information:\n"
              "the number of dock(s):\n"
              "the number of lift(s):\n"
              "the number of vehicle(s):\n"
              "the number of store(s):\n"
              "vehicle transport time:\n"
              << std::endl;
    int dock_num, lift_num, vehicle_num, store_num, transport_time;
    std::cin >> dock_num >> lift_num >> vehicle_num >> store_num >> transport_time;
    port *port1;
    port1 = new port(dock_num, lift_num, vehicle_num, store_num, transport_time);


    std::cout <<
              "enter the ship information:\n"
              "the number of ship(s):\n"
              << std::endl;
    int n, m;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        std::cout << "enter the information of ship " + std::to_string(i) + ":\n";
        std::cout << "enter the in port time and out port time of the ship:" << std::endl;
        std::cin >> lift_num >> vehicle_num;
        ship *tship = new ship(i, lift_num, vehicle_num);
        port1->load_ship(tship);
        std::cout << "enter the number of cargoes on the ship:" << std::endl;
        std::cin >> m;
        for (int j = 1; j <= m; j++) {
            std::cout << j << ":enter the target store and lift time the cargo:" << std::endl;
            int lt, ts;
            std::cin >> ts >> lt;
            tship->load_cargo(new cargo(i * 100 + j, ts, lt));
        }
    }

    //set the transport time
    port1->transport_time[1] = 11;
    port1->transport_time[2] = 8;
    port1->transport_time[3] = 12;
//
//    port1->info();
//

//    worker_base *workerBase = new worker_base(port1);
//    std::vector<ship*> new_ship_rule;
//    std::vector<cargo*> new_cargo_rule;
//    int shipp[]={2,3,4,1};
//    int cargoo[]={5,13,4,17,3,10,15,7,12, 2, 16, 6, 1, 19, 9, 14, 18, 8, 11};
//    for(int i=0;i<4;i++){
//        new_ship_rule.push_back(workerBase->ship_rule[shipp[i]-1]);
//    }
//    for(int i=0;i<19;i++){
//        new_cargo_rule.push_back(workerBase->cargo_rule[cargoo[i]-1]);
//    }
//    workerBase->ship_rule = new_ship_rule;
//    workerBase->cargo_rule = new_cargo_rule;
//    workerBase->transport_rule = new_cargo_rule;
//    workerBase->work();


//

    greedy_worker *greedyWorker = new greedy_worker(port1);
    greedyWorker->work();


//    brute_force_worker* bruteForceWorker = new brute_force_worker(port1);
//    bruteForceWorker->work(100);


    greedy_random_worker* randomWorker = new greedy_random_worker(port1);
    randomWorker->work(100);

    pso_worker *psoWorker = new pso_worker(port1);
    psoWorker->population = 1000;
    psoWorker->alpha = 1;
    psoWorker->debug = 0;
    psoWorker->max_generation = 10;
    psoWorker->greedy=1;
    psoWorker->work();
    delete psoWorker;

    genetic_worker* geneticWorker = new genetic_worker(port1);
    geneticWorker->max_generation = 1000;
    geneticWorker->population = 100;
    geneticWorker->debug=0;
    geneticWorker->work();
    delete geneticWorker;

    return 0;
}


