//
//  main.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include <iostream>
#include "entities/ship.h"
#include "worker/worker_base.h"
#include "port.h"
int main(int argc, const char * argv[]) {
    
    std::cout<<
    "enter the port information:\n"
    "the number of berth(es):\n"
    "the number of lift(s):\n"
    "the number of vehicle(s):\n"
    "thi number of store(s):\n"
    <<std::endl;
    int a,b,c,d;
    std::cin>>a>>b>>c>>d;
    port* port1;
    port1 = new port(a,b,c,d);


    std::cout<<
    "enter the ship information:\n"
    "the number of ship(s):\n"
    <<std::endl;
    int n,m;
    std::cin>>n;
    for(int i=1;i<=n;i++){
        std::cout<<"enter the information of ship "+std::to_string(i)+":\n";
        std::cout<<"enter the in port time and out port time of the ship:"<<std::endl;
        std::cin>>b>>c;
        ship *tship = new ship(i,b,c);
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

    worker_base* workerBase;
    workerBase = new worker_base(port1);

    port1->simulate(workerBase->ship_rule,workerBase->cargo_rule);


    return 0;
}
