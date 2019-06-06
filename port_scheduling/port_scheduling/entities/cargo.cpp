//
//  cargo.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include <iostream>
#include "cargo.h"
std::string cargo::get_name(){
    return "Cargo["+std::to_string(id)+"]";
}

cargo::cargo(int id, int targetStoreId, int liftTime) : id(id), target_store_id(targetStoreId), lift_time(liftTime) {}

void cargo::info() {
    std::cout<<"--"<<get_name()<<std::endl;
    std::cout<<"---target store:"<<target_store_id<<std::endl;
    std::cout<<"---lift time:"<<lift_time<<std::endl;
}

cargo::~cargo() {
    delete this;
}
