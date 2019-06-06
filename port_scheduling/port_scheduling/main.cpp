//
//  main.cpp
//  port_scheduling
//
//  Created by 谢威宇 on 2019/6/6.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//

#include <iostream>
#include "port.hpp"
int main(int argc, const char * argv[]) {
    
    std::cout<<
    "enter the port infomation:\n"
    "the number of berth(es):\n"
    "the number of lift(s):\n"
    "the number of vichle(s):\n"
    "thi number of store(s):\n"
    <<std::endl;
    int a,b,c,d;
    std::cin>>a>>b>>c>>d;
    port A(a,b,c,d);
    
    std::cout<<
    "enter the ship infomation:\n"
    "the number of ship(s):\n"
    <<std::endl;
    
    
    
    
    
    return 0;
}
