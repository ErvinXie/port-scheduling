//
// Created by 谢威宇 on 2019-06-07.
//

#include "greedy_worker.h"

greedy_worker::greedy_worker(port *port1) : worker_base(port1) {}

void greedy_worker::generate_method() {
    worker_base::generate_method();
    //sort by the total time of a ship
    std::sort(ship_rule.begin(),ship_rule.end(),[](ship*a,ship*b)->bool{
        return (a->out_port_time+a->out_port_time)<(b->in_port_time+b->out_port_time);
    });
    cargo_rule.clear();
    std::for_each(ship_rule.begin(),ship_rule.end(),[this](ship*this_ship){
        //sort by the lift time of a cargo
        std::sort(this_ship->cargoes.begin(),this_ship->cargoes.end(),[](cargo*a,cargo*b)->bool{
            return a->lift_time<b->lift_time;
        });

       std::for_each(this_ship->cargoes.begin(),this_ship->cargoes.end(),[this](cargo* this_cargo){
          cargo_rule.push_back(this_cargo);
       });
    });

}

void greedy_worker::work() {
    std::cout<<"-----------greedy algorithm---------"<<std::endl;
    generate_method();
    best_ship_rule = ship_rule;
    best_cargo_rule = cargo_rule;
    finish_work();
}
