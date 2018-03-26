//
// Created by daniel on 25.03.18.
//

#include "../hashmap.h"
#include "../tests/utils.h"
#include <chrono>
#include <memory>
#include <list>

//==== insert =====
void ht_one_rand_insert(int iterations, float load_factor) {

    std::vector<long> times{};
    times.reserve(iterations);


    for (int i = 0; i < iterations; ++i) {

        HashTable ht(2048);


        auto rnd_shares = random_shares(static_cast<size_t>(2048 * load_factor));

        for (auto &&sh: rnd_shares)
            ht.insert(std::move(sh));

        auto to_insert = random_shares(1);


        auto start = std::chrono::high_resolution_clock::now();
        Share *s = nullptr;
        for (auto &&is : to_insert)
            s = ht.insert(std::move(is));
        auto end = std::chrono::high_resolution_clock::now();

        if (s == nullptr) exit(-1);

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0) / times.size();
    std::cout << "hashtable: average: " << average << "ns" << " from " << iterations << " iterations"
              << "to insert 1 random share with load factor:" << load_factor << std::endl;
}

void stdmap_one_rand_insert(int iterations, float load_factor) {
    std::vector<long> times{};
    times.reserve(iterations);

    float lf = 0.0;
    for (int i = 0; i < iterations; ++i) {

        std::unordered_map<std::string, Share> idmap{};
        idmap.reserve(2048);
        idmap.max_load_factor(0.99);

        std::unordered_map<std::string, Share> namemap{};
        namemap.reserve(2048);
        namemap.max_load_factor(0.99);

        auto rnd_shares1 = random_shares(static_cast<size_t>(2048 * load_factor));
        auto rnd_shares2 = rnd_shares1;

        for (auto &&sh: rnd_shares1)
            idmap[sh.id] = sh;

        for (auto &&sh: rnd_shares2)
            namemap[sh.id] = sh;

        lf = idmap.load_factor();

        auto to_insert = random_share();
        Share to_insert2 = to_insert;
        std::string id = to_insert.id;
        std::string name = to_insert.name;

        auto start = std::chrono::high_resolution_clock::now();
        idmap.insert(std::move(std::make_pair(id, std::move(to_insert))));
        namemap.insert(std::move(std::make_pair(name, std::move(to_insert2))));

        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0) / times.size();
    std::cout << "std::unordered_map: average: " << average << "ns" << " from " << iterations << " iterations"
              << "to insert 1 random share with load factor:" << lf << std::endl;
}

void vector_one_rand_insert_rand_pos(int iterations) {

    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        std::vector<std::unique_ptr<Share>> shs(2048);

        for (auto &&s: random_shares(1000))
            shs.push_back(std::make_unique<Share>(s));

        auto to_insert = std::make_unique<Share>(random_share());

        auto ri = (int) ((((double) std::rand()) / INT32_MAX) * 1000);

        auto start = std::chrono::high_resolution_clock::now();
        shs.emplace(shs.begin() + ri, std::move(to_insert));
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0) / times.size();

    std::cout << "vector: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to insert 1 random share at random position" << std::endl;
}

void vector_one_rand_insert_back(int iterations) {

    std::vector<long> times{};
    times.reserve(iterations);


    for (int i = 0; i < iterations; ++i) {
        std::vector<std::unique_ptr<Share>> shs(2048);

        for (auto &&s: random_shares(1000))
            shs.push_back(std::make_unique<Share>(s));

        auto to_insert = std::make_unique<Share>(random_share());

        auto ri = (int) ((((double) std::rand()) / INT32_MAX) * 1000);

        auto start = std::chrono::high_resolution_clock::now();
        shs.emplace_back(std::move(to_insert));
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0) / times.size();

    std::cout << "vector: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to insert 1 random share at back" << std::endl;
}

void list_one_rand_share_insert_back(int iterations) {
    std::vector<long> times{};
    times.reserve(iterations);


    for (int i = 0; i < iterations; ++i) {
        std::list<Share> l{};
        for (auto &&s: random_shares(1000))
            l.emplace_back(std::move(s));

        auto to_insert = random_share();

        auto start = std::chrono::high_resolution_clock::now();
        l.emplace_back(std::move(to_insert));
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0) / times.size();

    std::cout << "list: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to insert 1 random share at rand pos" << std::endl;
}

void list_one_rand_share_insert_rand(int iterations) {
    std::vector<long> times{};
    times.reserve(iterations);


    for (int i = 0; i < iterations; ++i) {
        std::list<Share> l{};
        for (auto &&s: random_shares(1000))
            l.emplace_back(std::move(s));

        auto to_insert = random_share();
        auto ri = (int) ((((double) std::rand()) / INT32_MAX) * 1000);
        auto it = l.begin();

        auto start = std::chrono::high_resolution_clock::now();
        advance(it, ri);
        l.emplace(it, std::move(to_insert));
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0) / times.size();

    std::cout << "list: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to insert 1 random share at back" << std::endl;
}

//=== look up ====

void ht_get_random_share_by_key(int iterations, float load_factor) {
    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {

        HashTable ht(2048);


        auto rnd_shares = random_shares(static_cast<size_t>(2048 * load_factor));
        auto rand_index = (int) ((((double) std::rand()) / INT32_MAX) * rnd_shares.size());

        auto search_key = rnd_shares[rand_index].id;
        for (auto &&sh: rnd_shares)
            ht.insert(std::move(sh));

        auto start = std::chrono::high_resolution_clock::now();
        auto volatile sh = ht.get_by_id(search_key);
        auto end = std::chrono::high_resolution_clock::now();
        sh->id;

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0l) / times.size();

    std::cout << "hashtable: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to look up 1 random share with load factor: " << load_factor << std::endl;

}

void vec_get_random_share_by_key(int iterations) {
    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        auto rnd_shares = random_shares(static_cast<size_t>(1000));
        auto rand_index = (int) ((((double) std::rand()) / INT32_MAX) * rnd_shares.size());
        auto search_key = rnd_shares[rand_index].id;

        auto start = std::chrono::high_resolution_clock::now();
        auto volatile find = std::find_if(rnd_shares.begin(), rnd_shares.end(),
                                          [&](Share &s) { return s.id == search_key; });
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0l) / times.size();

    std::cout << "vector: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to look up 1 random share by key" << std::endl;
}

void list_get_random_share_by_key(int iterations) {
    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        auto rnd_shares = random_shares(static_cast<size_t>(1000));
        auto rand_index = (int) ((((double) std::rand()) / INT32_MAX) * rnd_shares.size());
        auto search_key = rnd_shares[rand_index].id;
        std::list<Share> list{};
        for (auto &&s: rnd_shares) {
            list.emplace_back(std::move(s));
        }

        auto start = std::chrono::high_resolution_clock::now();
        auto volatile find = std::find_if(list.begin(), list.end(),
                                          [&](Share &s) { return s.id == search_key; });
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0l) / times.size();

    std::cout << "list: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to look up 1 random share by key" << std::endl;
}

void vec_get_random_share_by_pos(int iterations) {
    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        auto rnd_shares = random_shares(static_cast<size_t>(1000));
        auto rand_index = (int) ((((double) std::rand()) / INT32_MAX) * rnd_shares.size());


        auto start = std::chrono::high_resolution_clock::now();
        volatile Share &res = rnd_shares[rand_index];
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0l) / times.size();

    std::cout << "vector: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to look up 1 random share by pos" << std::endl;
}

void list_get_random_share_by_pos(int iterations) {
    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        auto rnd_shares = random_shares(static_cast<size_t>(1000));
        auto rand_index = (int) ((((double) std::rand()) / INT32_MAX) * rnd_shares.size());
        std::list<Share> list{};
        for (auto &&s: rnd_shares) {
            list.emplace_back(std::move(s));
        }

        auto it = list.begin();

        auto start = std::chrono::high_resolution_clock::now();
        std::advance(it, rand_index);
        auto volatile res = *it;
        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    long average = std::accumulate(times.begin(), times.end(), 0l) / times.size();

    std::cout << "list: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to look up 1 random share by pos" << std::endl;
}

// === remove ====

void ht_remove_and_get(int iterations, float load_factor_before, float load_factor_after) {

    std::vector<long> times{};
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        HashTable ht{ 2048 };

        auto rnd_shares = random_shares(static_cast<size_t>(2048 * load_factor_before));
        std::vector<Share> rnd_shares2 = rnd_shares;
        auto rng = std::default_random_engine{};
        std::shuffle(rnd_shares2.begin(), rnd_shares2.end(), rng);

        for (auto &&sh: rnd_shares)
            ht.insert(std::move(sh));

        int split_i = 0;
        int split = (int)((load_factor_before - load_factor_after) * 2048);
        for(; split_i < split; ++split_i ){
            ht.remove_by_id(rnd_shares2[split_i].id);
        }
        split_i++;
        auto start = std::chrono::high_resolution_clock::now();

        for(; split_i < rnd_shares2.size(); ++split_i){
            auto volatile a = ht.get_by_id(rnd_shares2[split_i].id);
        }

        auto end = std::chrono::high_resolution_clock::now();

        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / (split_i - split));
    }

    long average = std::accumulate(times.begin(), times.end(), 0l) / times.size();

    std::cout << "hashtable: average: " << average << "ns" << " from " << iterations << " iterations "
              << "to look up 1 element after filling up to "<< load_factor_before << "and then deleting to" << load_factor_after << std::endl;
}

int main() {
    std::cout << "=== inserting ===" << std::endl;

    stdmap_one_rand_insert(1000, .00);
    stdmap_one_rand_insert(1000, .25);
    stdmap_one_rand_insert(1000, .50);
    stdmap_one_rand_insert(1000, .75);
    stdmap_one_rand_insert(1000, .90);

    ht_one_rand_insert(10000, 0   );
    ht_one_rand_insert(10000, 0.25);
    ht_one_rand_insert(10000, 0.5 );
    ht_one_rand_insert(10000, 0.75);
    ht_one_rand_insert(10000, 0.9 );

    vector_one_rand_insert_rand_pos(10000);
    vector_one_rand_insert_back(10000);

    list_one_rand_share_insert_rand(10000);
    list_one_rand_share_insert_back(10000);

    std::cout << "=== look up ===" << std::endl;

    ht_get_random_share_by_key(10000, .1 );
    ht_get_random_share_by_key(10000, .25);
    ht_get_random_share_by_key(10000, .5 );
    ht_get_random_share_by_key(10000, .75);
    ht_get_random_share_by_key(10000, .9 );

    vec_get_random_share_by_key(10000);
    list_get_random_share_by_key(10000);

    vec_get_random_share_by_pos(10000);
    list_get_random_share_by_pos(10000);

    std::cout << "deleting" << std::endl;

    ht_remove_and_get(10000, .95, .10);

}
