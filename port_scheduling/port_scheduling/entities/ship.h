//
//  ship.hpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#ifndef ship_hpp
#define ship_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "cargo.h"
class port;

class ship {
public:
    int id;
    int in_port_time;
    int out_port_time;

    std::vector<cargo *> cargoes;

    int on_deck_cargoes_num=0;

    port* parent_port;

//Computing Methods
    ship() {}

    ship(int id, int portInTime, int portOutTime);

    virtual ~ship();

    void load_cargo(cargo *tcargo);



//Debug Methods

    std::string get_name();

    void info();


};

#endif /* ship_hpp */
