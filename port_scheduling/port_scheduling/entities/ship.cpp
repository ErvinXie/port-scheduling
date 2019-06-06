//
//  ship.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include "ship.hpp"

std::string ship::get_name(){
    return "Ship["+std::to_string(id)+"]";
}
void ship::load_cargo(cargo* tcargo){
    cargos.push_back(tcargo);
    tcargo->parent_ship = this;
}

