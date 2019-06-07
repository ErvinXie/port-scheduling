//
// Created by 谢威宇 on 2019-06-06.
//

#include "worker_base.h"
const static std::string TAG = "work_base::";
worker_base::worker_base(port *port1) : port1(port1) {
    for(int i=0;i<port1->ships.size();i++){
        ship_rule.push_back(port1->ships[i]);
        for(int j=0;j<port1->ships[i]->cargoes.size();j++){
            cargo_rule.push_back(port1->ships[i]->cargoes[j]);
        }
    }
    std::cout<<TAG<<"ships and cargoes loaded!"<<std::endl;
}

void worker_base::work() {
    //get the method
    generate_method();


    int time = port1->simulate(ship_rule,cargo_rule,2);
    best_time = std::min(best_time,time);


    //finish
    finish_work();
}


void worker_base::generate_method() {

}

void worker_base::finish_work() {
    std::cout<<"Working Finished\n"
    <<"the best time is "
    <<best_time
    <<std::endl;
}
