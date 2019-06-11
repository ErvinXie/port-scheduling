//
// Created by 谢威宇 on 2019-06-09.
//

#include "pso_worker.h"

#define RAND(a, b) (rand()*((b)-(a))/double(RAND_MAX)+(a))


pso_worker::pso_worker(port *port1) : worker_base(port1) {}


void pso_worker::work() {
    std::cout << "-----------pso algorithm---------" << std::endl
              << "population:" << population << " max generation:" << max_generation << std::endl;

    get_first_generation();
    int generation = 1;
    while (generation <= max_generation) {
        std::cout << "generation (" << generation << "/" << max_generation << ")";
        get_fit_score();
        if (generation == max_generation)
            break;


        //reproduce the next generation

        //update inertial weight
        inertial_weight *= alpha;

        //update velocity
        for (int i = 0; i < population; i++) {
            for (int j = 0; j < cargo_rule.size() + transport_rule.size(); j++) {
                velocity[i][j] = velocity[i][j] * inertial_weight
                                 + c1 * RAND(0, 2) * (personal_best_position[i][j] - position[i][j])
                                 + c2 * RAND(0, 2) * (global_best_position[j] - position[i][j]);
            }
        }
        //update position
        for (int i = 0; i < population; i++) {
            for (int j = 0; j < cargo_rule.size() + transport_rule.size(); j++) {
                position[i][j] = position[i][j] + velocity[i][j];
            }
        }
        generation++;
    }
    finish_work();
}

void pso_worker::get_first_generation() {
    position.clear();
    velocity.clear();
    personal_best_position.clear();
    personal_best_time.clear();
    for (int i = 0; i < population; i++) {
        std::vector<double> p;
        std::vector<double> v;
        for (int j = 0; j < cargo_rule.size() + transport_rule.size(); j++) {
            //make the initial value restricted
            p.push_back(RAND(0, velocity_max));
            v.push_back(RAND(velocity_min, velocity_max));
        }


        //if not greedy first, we have to make the first generation legal
        if (greedy == 0) {
            std::vector<std::pair<double, int>> p_to_id;
            for (int j = 0; j < cargo_rule.size(); j++) {
                p_to_id.push_back(std::make_pair(p[j], j));
            }

            std::sort(p_to_id.begin(), p_to_id.end());

            std::vector<cargo *> this_cargo_rule;

            for (int k = 0; k < cargo_rule.size(); k++) {
                this_cargo_rule.push_back(cargo_rule[p_to_id[k].second]);
            }

            p_to_id.clear();
            for (int j = 0; j < transport_rule.size(); j++) {
                p_to_id.push_back(std::make_pair(p[cargo_rule.size() + j], j));
            }
            std::sort(p_to_id.begin(), p_to_id.end());
            std::vector<cargo *> this_transport_rule;
            for (int k = 0; k < cargo_rule.size(); k++) {
                this_transport_rule.push_back(transport_rule[p_to_id[k].second]);
            }


            port1->simulate_greedy(ship_rule, this_cargo_rule, this_transport_rule, 0);
            this_cargo_rule = port1->lift_order;
            this_transport_rule = port1->transport_order;


            p.clear();
            std::map<cargo *, double> p_value;
            for (int k = 0; k < this_cargo_rule.size(); k++) {
                p_value[this_cargo_rule[k]] = velocity_max / this_cargo_rule.size() * k;
            }
            for (int k = 0; k < cargo_rule.size(); k++) {
                p.push_back(p_value[cargo_rule[k]]);
            }
            p_value.clear();
            for (int k = 0; k < this_transport_rule.size(); k++) {
                p_value[this_transport_rule[k]] = velocity_max / this_transport_rule.size() * k;
            }
            for (int k = 0; k < transport_rule.size(); k++) {
                p.push_back(p_value[cargo_rule.size() + transport_rule[k]]);
            }

        }


        position.push_back(p);
        personal_best_time.push_back(0X3f3f3f3f);
        personal_best_position.push_back(p);
        velocity.push_back(v);
    }

}

void pso_worker::get_fit_score() {

    int min_time = 0x3f3f3f3f;
    int debug_out = 0;
    for (int i = 0; i < population; i++) {
        //get the rule
        std::vector<std::pair<double, int>> p_to_id;
        for (int j = 0; j < cargo_rule.size(); j++) {
            p_to_id.push_back(std::make_pair(position[i][j], j));
        }

        std::sort(p_to_id.begin(), p_to_id.end());

        std::vector<cargo *> this_cargo_rule;

        for (int i = 0; i < cargo_rule.size(); i++) {
            this_cargo_rule.push_back(cargo_rule[p_to_id[i].second]);
        }


        p_to_id.clear();
        for (int j = 0; j < transport_rule.size(); j++) {
            p_to_id.push_back(std::make_pair(position[i][cargo_rule.size() + j], j));
        }
        std::sort(p_to_id.begin(), p_to_id.end());
        std::vector<cargo *> this_transport_rule;
        for (int i = 0; i < cargo_rule.size(); i++) {
            this_transport_rule.push_back(transport_rule[p_to_id[i].second]);
        }


        //simulate for the time
        std::sort(ship_rule.begin(), ship_rule.end());
        int this_best_time = 0x3f3f3f3f;
        while (std::next_permutation(ship_rule.begin(), ship_rule.end())) {
            int time;
            if (greedy == 1)
                time = port1->simulate_greedy(ship_rule, this_cargo_rule, this_transport_rule);
            else {
                time = port1->simulate(ship_rule, this_cargo_rule, this_transport_rule);
            }

            if (time < this_best_time) {
                this_best_time = time;
            }
            //update the global information
            if (time < best_time) {
                debug_out = 1;
                best_time = time;
                best_cargo_rule = this_cargo_rule;
                best_transport_rule = this_transport_rule;
                best_ship_rule = ship_rule;
                global_best_position = position[i];
            }
            //update the personal information
            if (time < personal_best_time[i]) {
                personal_best_time[i] = time;
                personal_best_position[i] = position[i];
            }
            //update this generation information
            if (time < min_time) {
                min_time = time;
            }
        }
    }

    std::cout << " this generation best time:"
              << min_time
              << " global best:"
              << best_time
              << std::endl;


    if (debug && debug_out) {
        std::cout << "The best schedule is:";
        port1->simulate_greedy(best_ship_rule, best_cargo_rule, best_transport_rule, 10);
        std::cout << "\n";
        std::cout << "The best ship rule is:\n";
        for (int i = 0; i < best_ship_rule.size(); i++) {
            std::cout << best_ship_rule[i]->get_name() << "\n";
        }
    }
}


