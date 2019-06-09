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
    //initialize the group
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
        std::vector<std::vector<cargo *> > new_group;
        int i = 0;
        double possibility_sum = 0;
        //single cross
        possibility_sum += single_cross_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            //select parents
            int
                    father_id = RAND(0, population),
                    mother_id = RAND(0, population);

//            std::cout<<father_id<<" "<<mother_id<<std::endl;

            std::vector<cargo *>
                    boy = group[father_id],
                    girl = group[mother_id];

            //select a point in [1,n-1];
            int point = RAND(1, boy.size());

            //cross
            for (int i = point; i < boy.size(); i++) {
                std::swap(boy[i], girl[i]);
            }

            //repair
            std::set<cargo *> inset;
            for (int i = 0; i < point; i++) {
                inset.insert(boy[i]);
            }
            for (int i = point; i < boy.size(); i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(boy[i])) {
                    while (j < boy.size() && inset.count(group[mother_id][j]))
                        j++;
                    boy[i] = group[mother_id][j];
                }
                inset.insert(boy[i]);
            }
            inset.clear();
            for (int i = 0; i < point; i++) {
                inset.insert(girl[i]);
            }
            for (int i = point; i < girl.size(); i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(girl[i])) {
                    while (j < girl.size() && inset.count(group[father_id][j]))
                        j++;
                    girl[i] = group[father_id][j];
                }
                inset.insert(girl[i]);
            }

            //add them to the new group
            new_group.push_back(boy);
            i++;
            if (i < possibility_sum * population) {
                new_group.push_back(girl);
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
                    boy = group[father_id],
                    girl = group[mother_id];

            //select a point in [1,n-1];
            int
                    point1 = RAND(1, boy.size()),
                    point2 = RAND(1, boy.size());
            if (point1 > point2)
                std::swap(point1, point2);

            //cross
            for (int i = point1; i <= point2; i++) {
                std::swap(boy[i], girl[i]);
            }

            //repair
            std::set<cargo *> inset;
            for (int i = 0; i < boy.size(); i++) {
                if (i < point1 || i > point2)
                    inset.insert(boy[i]);
            }
            for (int i = point1; i <= point2; i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(boy[i])) {
                    while (j < boy.size() && inset.count(group[mother_id][j]))
                        j++;
                    boy[i] = group[mother_id][j];
                }
                inset.insert(boy[i]);
            }

            inset.clear();
            for (int i = 0; i < girl.size(); i++) {
                if (i < point1 || i > point2)
                    inset.insert(girl[i]);
            }
            for (int i = point1; i <= point2; i++) {
                int j = 0;
                //if this cargo already showed up
                if (inset.count(girl[i])) {
                    while (j < girl.size() && inset.count(group[father_id][j]))
                        j++;
                    girl[i] = group[father_id][j];
                }
                inset.insert(girl[i]);
            }

            //add them to the new group
            new_group.push_back(girl);
            i++;
            if (i <(int)(possibility_sum * population)) {
                new_group.push_back(boy);
            }

        }
        //migrant
        possibility_sum += migrant_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            std::shuffle(cargo_rule.begin(), cargo_rule.end(), std::default_random_engine(seed));
            new_group.push_back(cargo_rule);
        }

        //mutant
        possibility_sum += mutant_possibility;
        for (; i < (int)(possibility_sum * population); i++) {
            std::vector<cargo *> mutant_unit = group[get_proportional_random_unit()];
            std::swap(mutant_unit[RAND(0, mutant_unit.size())], mutant_unit[RAND(0, mutant_unit.size())]);
            new_group.push_back(mutant_unit);
        }

        //stable
        for (; i < population; i++) {
            new_group.push_back(group[get_proportional_random_unit()]);
        }
        //make sure the best one stay
//        new_group.push_back(best_cargo_rule);

        group = new_group;
        generation++;
    }
    finish_work();
}

void genetic_worker::finish_work() {
    worker_base::finish_work();
}

void genetic_worker::get_first_generation() {
    group.clear();

    for (int i = 0; i < population; i++) {
        std::shuffle(cargo_rule.begin(), cargo_rule.end(), std::default_random_engine(seed));
        group.push_back(cargo_rule);
    }
}

void genetic_worker::get_fit_score() {
    //clear existing score
    fit_score.clear();
    for (int i = 0; i < group.size(); i++) {
        std::sort(ship_rule.begin(), ship_rule.end());
        int this_best_time = 0x3f3f3f3f;
        while (std::next_permutation(ship_rule.begin(), ship_rule.end())) {
            int time = port1->simulate(ship_rule, group[i]);
            if (time < this_best_time) {
                this_best_time = time;
            }
            if (time < best_time) {
                best_time = time;
                best_cargo_rule = group[i];
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
        port1->simulate(best_ship_rule, best_cargo_rule, 10);
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
