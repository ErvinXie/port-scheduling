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
#include "cargo.hpp"

class ship{
public:
    int id;
    int port_in_time;
    int port_out_time;
    
    std::vector<cargo*> cargos;
    
    
    
//Computing Methods
    ship(){}
    ship(int id,int port_in_time,int port_out_time):id(id),port_in_time(port_in_time),port_out_time(port_out_time){}
    
    
    void load_cargo(cargo* tcargo);
    
    
    
//Debug Methods
    
    std::string get_name();
    
    

};

#endif /* ship_hpp */
