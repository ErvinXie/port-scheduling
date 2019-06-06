//
//  cargo.hpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#ifndef cargo_hpp
#define cargo_hpp

#include <stdio.h>
#include <string>
class ship;

class cargo{
public:
    int id;
    int target_store_id;
    int lift_time;
    ship* parent_ship;
    
    std::string get_name();
};
#endif /* cargo_hpp */
