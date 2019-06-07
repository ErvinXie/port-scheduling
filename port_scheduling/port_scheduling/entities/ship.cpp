//
//  ship.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include "ship.h"

ship::ship(int id, int portInTime, int portOutTime) : id(id), in_port_time(portInTime), out_port_time(portOutTime) {}

ship::~ship() {
    for(int i=0;i<cargoes.size();i++){
        delete cargoes[i];
    }
    delete this;
}



void ship::load_cargo(cargo *tcargo) {
    cargoes.push_back(tcargo);
    on_deck_cargoes_num++;
    tcargo->parent_ship = this;
}


void ship::info() {
    std::cout << "-" << get_name() << ": " << std::endl;
    std::cout<<"--In Port Time: "<<in_port_time<<std::endl;
    std::cout<<"--Out Port Time: "<<out_port_time<<std::endl;
    for (int i = 0; i < cargoes.size(); i++) {
        cargoes[i]->info();
    }
}



std::string ship::get_name() {
    return "Ship[" + std::to_string(id) + "]";
}