#include <iostream>
#include <random>
#include <vector>
#include <utility>

typedef std::pair<float, float> pair_code;
typedef std::vector<pair_code *> vec_code;

pair_code *find_nearest_codevector (pair_code *train, pair_code *codevectors[10][10], int *near_y, int *near_x);


int main (void)
{
    // vec_code *codevectors = new vec_code();

    pair_code *codevectors[10][10];

    std::random_device rnd;

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            float rand_y = static_cast<float>(rnd()) / rnd.max();
            float rand_x = static_cast<float>(rnd()) / rnd.max();

            pair_code *new_pair = new pair_code (rand_y, rand_x);
            // codevectors->push_back (new_pair);
            codevectors[y][x] = new_pair;
        }
    }

    // vec_code::iterator it = codevectors->begin();
    // while (it != codevectors->end()) {
    //     std::cout << "Pair <" << (*it)->first << ", " << (*it)->second << ">\n";
    //     it++;
    // }

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            std::cout << "Pair <" << codevectors[y][x]->first << ", " << codevectors[y][x]->second << ">\n";
        }
    }


    vec_code *vec_train = new vec_code ();
    for (int y = -4; y < 5; y++) {
        for (int x = -4; x < 5; x++) {
            pair_code *train_item = new pair_code (static_cast<float>(y), static_cast<float>(x));
            vec_train->push_back (train_item);
        }
    }


    vec_code *code_training[10][10];
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            code_training[y][x] = new vec_code ();
        }
    }

    const int iterate_limit = 1;

    for (int t = 0; t < iterate_limit; t++) {

        float dist_limit;
        if (t <= iterate_limit/8) {
            dist_limit = 10.0 * 10.0;
        } else if (t < iterate_limit / 2) {
            dist_limit = 5.0 * 5.0;
        } else {
            dist_limit = 0.0;
        }

        vec_code::iterator it = vec_train->begin();
        while (it != vec_train->end()) {
            int near_y, near_x;
            pair_code *nearest_c = find_nearest_codevector ((*it), codevectors, &near_y, &near_x);
            std::cout << "Nearest is <" << nearest_c->first << ", " << nearest_c->second << "(" << near_y << ", " << near_x << ")>\n";
            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    if (y != near_y && x != near_x) {
                        // float dist = powf (codevectors[y][x]->first - codevectors[near_y][near_x]->first, 2) +
                        //     powf (codevectors[y][x]->second - codevectors[near_y][near_x]->second, 2);
                        // if (dist < dist_limit) {
                        //     code_training[y][x]->push_back ((*it));
                        //     std::cout << "  Push into <" << y << ", " << x << ">\n";
                        // }
                        if (near_y - 1 >= 0 && near_x -1 >= 0) {
                            code_training[y][x]->push_back ((*it));
                        }
                    }
                }
            }

            it++;
        }


        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                vec_code::iterator it = code_training[y][x]->begin ();
                float total_x = 0.0f, total_y = 0.0f;
                int count = 0;
                while (it != code_training[y][x]->end ()) {
                    total_y += (*it)->first;
                    total_x += (*it)->second;
                    std::cout << "<" << (*it)->first << ", " << (*it)->second << ">, ";
                    count++;
                    it++;
                }
                codevectors[y][x]->first  = total_y / count;
                codevectors[y][x]->second = total_x / count;

                std::cout << "CodeVectors[" << y << "][" << x << "] = (" <<
                    codevectors[y][x]->first  << ", " <<
                    codevectors[y][x]->second << ")\n";
            }
        }

    }

    return 0;
}


pair_code *find_nearest_codevector (pair_code *train, pair_code *codevectors[10][10], int *near_y, int *near_x)
{
    float min_length = 1000.0f;
    pair_code *min_pair = NULL;
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            float length = powf(train->first - (codevectors[y][x])->first, 2) +
                powf(train->second - (codevectors[y][x])->second, 2);
            if (length < min_length) {
                min_length = length;
                min_pair = codevectors[y][x];
                *near_y = y;
                *near_x = x;
            }
        }
    }

    return min_pair;
}
