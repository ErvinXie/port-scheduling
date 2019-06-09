//
// Created by 谢威宇 on 2019-06-07.
//

#ifndef PORT_SCHEDULING_BRUTE_FORCE_WORKER_H
#define PORT_SCHEDULING_BRUTE_FORCE_WORKER_H


#include "worker_base.h"

class brute_force_worker : public worker_base {
public:

    std::map<int, long long> time_distribute;


    brute_force_worker(port *port1);

    virtual void work(long long limit=1e4);


    void finish_work() override;
};


#endif //PORT_SCHEDULING_BRUTE_FORCE_WORKER_H
