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


    // Methods
    cargo(int id, int targetStoreId, int liftTime);

    virtual ~cargo();

    // Debug Methods
    std::string get_name();
    void info();
};
#endif /* cargo_hpp */
