//
// Created by 谢威宇 on 2019-06-07.
//

#ifndef PORT_SCHEDULING_GENETIC_WORKER_H
#define PORT_SCHEDULING_GENETIC_WORKER_H


#include "worker_base.h"

class genetic_worker : public worker_base {
public:

    int population=1000,max_generation=100;
    double single_cross_possibility = 0.5;
    double double_cross_possibility = 0.4;
    double migrant_possibility = 0;
    double mutant_possibility = 0.05;


    std::vector<std::vector<cargo *>> cargo_rule_group;
    std::vector<std::vector<cargo *>> transport_rule_group;
    std::vector<double> fit_score;
    std::vector<double> fit_score_presum;




    genetic_worker(port *port1);

    void work();

    void finish_work() override;


private:

    int get_proportional_random_unit();
    void get_fit_score();
    void get_first_generation();
};


#endif //PORT_SCHEDULING_GENETIC_WORKER_H
