//
// Created by 谢威宇 on 2019-06-07.
//

#ifndef PORT_SCHEDULING_RANDOM_WORKER_H
#define PORT_SCHEDULING_RANDOM_WORKER_H


#include "brute_force_worker.h"

class random_worker : public brute_force_worker{
public:
    random_worker(port *port1);

    void work(long long int limit=1e4) override;

};


#endif //PORT_SCHEDULING_RANDOM_WORKER_H
