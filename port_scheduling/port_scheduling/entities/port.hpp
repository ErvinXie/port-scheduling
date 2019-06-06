//
//  port.hpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#ifndef port_hpp
#define port_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "ship.hpp"
class port{
public:
    int berth_num;
    int lift_num;
    int vichle_num;
    int store_num;
    
    std::vector<ship> ships;
    
//Methods
    port(){}
    port(int berth_num,int lift_num,int vichle_num,int store_num):berth_num(berth_num),lift_num(lift_num),vichle_num(vichle_num),store_num(store_num){}
    
    
    
    void load_ship(ship& tship);
    
    
    
    
    void simulate(std::vector<cargo*>&);
    
    
};

#endif /* port_hpp */
