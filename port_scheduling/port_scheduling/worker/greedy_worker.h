//
// Created by 谢威宇 on 2019-06-07.
//

#ifndef PORT_SCHEDULING_GREEDY_WORKER_H
#define PORT_SCHEDULING_GREEDY_WORKER_H


#include "worker_base.h"

class greedy_worker : public  worker_base {
public:
    greedy_worker(port *port1);

    void generate_method();
};


#endif //PORT_SCHEDULING_GREEDY_WORKER_H
