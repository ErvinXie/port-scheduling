//
// Created by 谢威宇 on 2019-06-07.
//

#ifndef PORT_SCHEDULING_GENETIC_WORKER_H
#define PORT_SCHEDULING_GENETIC_WORKER_H


#include "worker_base.h"

class genetic_worker : public worker_base{
public:

     std::vector<std::vector<cargo*>> group;


    genetic_worker(port *port1);

    void work(int population,int max_generation=100);

    void finish_work() override;


private:

    void get_first_generation(int population);
};


#endif //PORT_SCHEDULING_GENETIC_WORKER_H
