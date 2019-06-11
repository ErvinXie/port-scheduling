//
// Created by 谢威宇 on 2019-06-06.
//

#ifndef PORT_SCHEDULING_WORKER_BASE_H
#define PORT_SCHEDULING_WORKER_BASE_H

#include <vector>
#include "port.h"
#include "time.h"

class worker_base {
public:
    std::vector<ship *> ship_rule;
    std::vector<cargo *> cargo_rule;
    std::vector<cargo *> transport_rule;

    port *port1;
    int best_time = 0x3f3f3f3f;

    int debug = 0;

    std::vector<ship *> best_ship_rule;
    std::vector<cargo *> best_cargo_rule;
    std::vector<cargo *> best_transport_rule;

    clock_t startTime,endTime;


    worker_base(port *port1);

    virtual void work();

    virtual void generate_method();

    virtual void finish_work();


};


#endif //PORT_SCHEDULING_WORKER_BASE_H
