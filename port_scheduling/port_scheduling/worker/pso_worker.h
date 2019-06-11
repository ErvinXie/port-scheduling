//
// Created by 谢威宇 on 2019-06-09.
//

#ifndef PORT_SCHEDULING_PSO_WORKER_H
#define PORT_SCHEDULING_PSO_WORKER_H

#include "worker_base.h"

class pso_worker : public worker_base {
public:
    //setting
    double velocity_max = 4.0;
    double velocity_min = -4.0;


    //parameters of pso
    double inertial_weight = 1;
    double alpha = 1;

    double c1 = 2;//personal effects
    double c2 = 2;//cargo_rule_group effects

    int population = 100;
    int max_generation = 100;

    int greedy=0;

    std::vector<std::vector<double>> position;
    std::vector<int> personal_best_time;
    std::vector<double> global_best_position;
    std::vector<std::vector<double>> personal_best_position;


    std::vector<std::vector<double>> velocity;

    void work() override;

    pso_worker(port *port1);

private:
    void get_first_generation();

    void get_fit_score();

};


#endif //PORT_SCHEDULING_PSO_WORKER_H
