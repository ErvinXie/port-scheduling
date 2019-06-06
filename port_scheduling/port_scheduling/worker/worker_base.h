//
// Created by 谢威宇 on 2019-06-06.
//

#ifndef PORT_SCHEDULING_WORKER_BASE_H
#define PORT_SCHEDULING_WORKER_BASE_H

#include <vector>
#include "port.h"

class worker_base {
public:
    std::vector<ship*> ship_rule;
    std::vector<cargo*> cargo_rule;

    port* port1;

    worker_base(port *port1);

};


#endif //PORT_SCHEDULING_WORKER_BASE_H
