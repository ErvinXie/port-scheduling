//
// Created by 谢威宇 on 2019-06-07.
//

#include "genetic_worker.h"
#include <random>
#include <algorithm>

#define RAND(a, b) ((rand() % ((b)-(a)))+ (a))

genetic_worker::genetic_worker(port *port1) : worker_base(port1) {}

unsigned seed = time(nullptr);

void genetic_worker::work() {
    //initialize the cargo_rule_group
    std::cout << "-----------genetic algorithm---------" << std::endl
              << "population:" << population << " max generation:" << max_generation << std::endl
              << "single cross:" << single_cross_possibility << " double cross:" << double_cross_possibility
              << " mutant:" << mutant_possibility << std::endl;

    get_first_generation();
    int generation = 1;
    while (generation <= max_generation) {
        std::cout << "generation (" << generation << "/" << max_generation << ")";
        get_fit_score();

        if (generation == max_generation)
            break;
        //reproduce the next generation
        std::vector<std::vector<cargo *> > new_cargo_group;
        std::vector<std::vector<cargo *> > new_transport_group;
        int i = 0;
        double possibility_sum = 0;
        //single cross
        possibility_sum += single_cross_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            //select parents
            int
                    father_id = RAND(0, population),
                    mother_id = RAND(0, population);

            std::vector<cargo *>
                    cargo_boy = cargo_rule_group[father_id],
                    cargo_girl = cargo_rule_group[mother_id],
                    transport_boy = transport_rule_group[father_id],
                    transport_girl = transport_rule_group[mother_id];

            //select a point in [1,n-1];
            int point = RAND(1, cargo_boy.size());
            //cross
            for (int i = point; i < cargo_boy.size(); i++) {
                std::swap(cargo_boy[i], cargo_girl[i]);
            }
            //repair
            std::set<cargo *> inset;
            for (int i = 0; i < point; i++) {
                inset.insert(cargo_boy[i]);
            }
            for (int i = point; i < cargo_boy.size(); i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(cargo_boy[i])) {
                    while (j < cargo_boy.size() && inset.count(cargo_rule_group[mother_id][j]))
                        j++;
                    cargo_boy[i] = cargo_rule_group[mother_id][j];
                }
                inset.insert(cargo_boy[i]);
            }
            inset.clear();
            for (int i = 0; i < point; i++) {
                inset.insert(cargo_girl[i]);
            }
            for (int i = point; i < cargo_girl.size(); i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(cargo_girl[i])) {
                    while (j < cargo_girl.size() && inset.count(cargo_rule_group[father_id][j]))
                        j++;
                    cargo_girl[i] = cargo_rule_group[father_id][j];
                }
                inset.insert(cargo_girl[i]);
            }

            //select a point in [1,n-1];
            point = RAND(1, cargo_boy.size());
            //cross
            for (int i = point; i < transport_boy.size(); i++) {
                std::swap(transport_boy[i], transport_girl[i]);
            }
            //repair
            inset.clear();
            for (int i = 0; i < point; i++) {
                inset.insert(transport_boy[i]);
            }
            for (int i = point; i < transport_boy.size(); i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(transport_boy[i])) {
                    while (j < transport_boy.size() && inset.count(transport_rule_group[mother_id][j]))
                        j++;
                    transport_boy[i] = transport_rule_group[mother_id][j];
                }
                inset.insert(transport_boy[i]);
            }
            inset.clear();
            for (int i = 0; i < point; i++) {
                inset.insert(transport_girl[i]);
            }
            for (int i = point; i < transport_girl.size(); i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(transport_girl[i])) {
                    while (j < transport_girl.size() && inset.count(transport_rule_group[father_id][j]))
                        j++;
                    transport_girl[i] = transport_rule_group[father_id][j];
                }
                inset.insert(transport_girl[i]);
            }

            //add them to the new rule_group
            new_cargo_group.push_back(cargo_boy);
            new_transport_group.push_back(transport_boy);
            i++;
            if (i < possibility_sum * population) {
                new_cargo_group.push_back(cargo_girl);
                new_transport_group.push_back(transport_girl);
            }

        }
        //double cross
        possibility_sum += double_cross_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            //select parents
            int
                    father_id = RAND(0, population),
                    mother_id = RAND(0, population);
            std::vector<cargo *>
                    cargo_boy = cargo_rule_group[father_id],
                    cargo_girl = cargo_rule_group[mother_id],
                    transport_boy = transport_rule_group[father_id],
                    transport_girl = transport_rule_group[mother_id];



            //select two points in [1,n-1];
            int
                    point1 = RAND(1, cargo_boy.size()),
                    point2 = RAND(1, cargo_boy.size());
            if (point1 > point2)
                std::swap(point1, point2);
            //cross
            for (int i = point1; i <= point2; i++) {
                std::swap(cargo_boy[i], cargo_girl[i]);
            }

            //repair
            std::set<cargo *> inset;
            for (int i = 0; i < cargo_boy.size(); i++) {
                if (i < point1 || i > point2)
                    inset.insert(cargo_boy[i]);
            }
            for (int i = point1; i <= point2; i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(cargo_boy[i])) {
                    while (j < cargo_boy.size() && inset.count(cargo_rule_group[mother_id][j]))
                        j++;
                    cargo_boy[i] = cargo_rule_group[mother_id][j];
                }
                inset.insert(cargo_boy[i]);
            }

            inset.clear();
            for (int i = 0; i < cargo_girl.size(); i++) {
                if (i < point1 || i > point2)
                    inset.insert(cargo_girl[i]);
            }
            for (int i = point1; i <= point2; i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(cargo_girl[i])) {
                    while (j < cargo_girl.size() && inset.count(cargo_rule_group[father_id][j]))
                        j++;
                    cargo_girl[i] = cargo_rule_group[father_id][j];
                }
                inset.insert(cargo_girl[i]);
            }


            //select two points in [1,n-1];

                    point1 = RAND(1, cargo_boy.size());
                    point2 = RAND(1, cargo_boy.size());
            if (point1 > point2)
                std::swap(point1, point2);
            //cross
            for (int i = point1; i <= point2; i++) {
                std::swap(transport_boy[i], transport_girl[i]);
            }



            //repair
            inset.clear();
            for (int i = 0; i < transport_boy.size(); i++) {
                if (i < point1 || i > point2)
                    inset.insert(transport_boy[i]);
            }
            for (int i = point1; i <= point2; i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(transport_boy[i])) {
                    while (j < transport_boy.size() && inset.count(transport_rule_group[mother_id][j]))
                        j++;
                    transport_boy[i] = transport_rule_group[mother_id][j];
                }
                inset.insert(transport_boy[i]);
            }

            inset.clear();
            for (int i = 0; i < transport_girl.size(); i++) {
                if (i < point1 || i > point2)
                    inset.insert(transport_girl[i]);
            }
            for (int i = point1; i <= point2; i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(transport_girl[i])) {
                    while (j < transport_girl.size() && inset.count(transport_rule_group[father_id][j]))
                        j++;
                    transport_girl[i] = transport_rule_group[father_id][j];
                }
                inset.insert(transport_girl[i]);
            }

            //add them to the new rule_group
            new_cargo_group.push_back(cargo_girl);
            new_transport_group.push_back(transport_girl);
            i++;
            if (i < possibility_sum * population) {
                new_cargo_group.push_back(cargo_boy);
                new_transport_group.push_back(transport_boy);
            }

        }
        //migrant
        possibility_sum += migrant_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            std::shuffle(cargo_rule.begin(), cargo_rule.end(), std::default_random_engine(seed));
            new_cargo_group.push_back(cargo_rule);

            std::shuffle(transport_rule.begin(), transport_rule.end(), std::default_random_engine(seed));
            new_transport_group.push_back(transport_rule);
        }

        //mutant
        possibility_sum += mutant_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            std::vector<cargo *> mutant_unit = cargo_rule_group[get_proportional_random_unit()];
            std::swap(mutant_unit[RAND(0, mutant_unit.size())], mutant_unit[RAND(0, mutant_unit.size())]);
            new_cargo_group.push_back(mutant_unit);

            mutant_unit = transport_rule_group[get_proportional_random_unit()];
            std::swap(mutant_unit[RAND(0, mutant_unit.size())], mutant_unit[RAND(0, mutant_unit.size())]);
            new_transport_group.push_back(mutant_unit);
        }

        //stable
        for (; new_cargo_group.size() < population; i++) {
            new_cargo_group.push_back(cargo_rule_group[get_proportional_random_unit()]);
            new_transport_group.push_back(transport_rule_group[get_proportional_random_unit()]);

        }
        //make sure the best one stay
//        new_cargo_group.push_back(best_cargo_rule);
//        new_transport_group.push_back(best_transport_rule);

        cargo_rule_group = new_cargo_group;
        transport_rule_group = new_transport_group;
        generation++;
    }
    finish_work();
}

void genetic_worker::finish_work() {
    worker_base::finish_work();
}

void genetic_worker::get_first_generation() {
    cargo_rule_group.clear();
    for (int i = 0; i < population; i++) {
        std::shuffle(cargo_rule.begin(), cargo_rule.end(), std::default_random_engine(seed));
        cargo_rule_group.push_back(cargo_rule);

        std::shuffle(transport_rule.begin(), transport_rule.end(), std::default_random_engine(seed));
        transport_rule_group.push_back(transport_rule);

    }
}

void genetic_worker::get_fit_score() {
    //clear existing score
    fit_score.clear();
    for (int i = 0; i < population; i++) {
        std::sort(ship_rule.begin(), ship_rule.end());
        int this_best_time = 0x3f3f3f3f;
        while (std::next_permutation(ship_rule.begin(), ship_rule.end())) {
            //to be changed
            int time = port1->simulate_greedy(ship_rule, cargo_rule_group[i],transport_rule_group[i]);
            if (time < this_best_time) {
                this_best_time = time;
            }
            if (time < best_time) {
                best_time = time;
                best_cargo_rule = cargo_rule_group[i];
                best_transport_rule = transport_rule_group[i];
                best_ship_rule = ship_rule;
            }
        }
        fit_score.push_back(this_best_time);
    }


    std::vector<std::pair<double, int>> score_to_id;
    double max_time = 0, min_time = 0x3f3f3f3f;
    for (int i = 0; i < fit_score.size(); i++) {
        score_to_id.push_back(std::make_pair(fit_score[i], i));
        max_time = std::max(max_time, fit_score[i]);
        min_time = std::min(min_time, fit_score[i]);
    }


    std::cout << " this generation best time:"
              << min_time
              <<" global best:"
              <<best_time
              << std::endl;
    if (debug&&min_time <= best_time) {
        std::cout << "The best schedule is:";
        port1->simulate_greedy(best_ship_rule, best_cargo_rule, best_transport_rule,10);
        std::cout << "\n";
        std::cout << "The best ship rule is:\n";
        for (int i = 0; i < best_ship_rule.size(); i++) {
            std::cout << best_ship_rule[i]->get_name() << "\n";
        }
    }


    sort(score_to_id.begin(), score_to_id.end());

    //compute a new score
    double total = 0;

    int selected_num = population * 0.05;
    double setted_score = score_to_id[population - 1].first;
    for (int i = 0; i < score_to_id.size(); i++) {
        if (i < selected_num) {
            fit_score[i] = (setted_score - fit_score[i]);
        } else {
            fit_score[i] = 0;
        }
        total += fit_score[i];
    }

    //normalize the new score
    for (int i = 0; i < fit_score.size(); i++) {
        fit_score[i] = fit_score[i] * population / total;
    }

    //calculate the pre sum
    fit_score_presum.clear();
    fit_score_presum.push_back(fit_score[0]);
    for (int i = 1; i < fit_score.size(); i++) {
        fit_score_presum.push_back(fit_score[i] + fit_score_presum.back());
    }
}

int genetic_worker::get_proportional_random_unit() {
    double r = rand() * population / double(RAND_MAX);
    return std::lower_bound(fit_score_presum.begin(), fit_score_presum.end(), r) - fit_score_presum.begin();
}
