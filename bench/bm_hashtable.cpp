//
// Created by daniel on 16.03.18.
//

#include <benchmark/benchmark.h>
#include <chrono>
#include "../hashmap.h"
#include "../tests/utils.h"

class Hashtable_1000 : public benchmark::Fixture {
public:

    HashTable ht{20};

    void
    SetUp(const benchmark::State &state) override {
        for (Share s: random_shares(static_cast<size_t >(state.range(0)))) {
            ht.insert(std::move(s));
        }
    }
};

BENCHMARK_DEFINE_F(Hashtable_1000, RandomInsert)(benchmark::State &s) {
    int i = 0;
    for (auto _: s) {
        Share share = random_share();

        auto start = std::chrono::high_resolution_clock::now();
        auto ptr = ht.insert(std::move(share));
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds =
                std::chrono::duration_cast<std::chrono::duration<double>>(
                        end - start);
        s.SetIterationTime(elapsed_seconds.count());
        ++i;
        if(ptr)
            ht.remove_by_id(ptr->id);
    }
}

BENCHMARK_REGISTER_F(Hashtable_1000, RandomInsert)->UseManualTime()
         ->Arg(10)
//        ->Arg(1000)
//        ->Arg(2000)
//        ->Arg(5000)
//        ->Arg(7000)
//        ->Arg(8000)
//        ->Arg(9000)
//        ->Arg(9100)
//        ->Arg(9200)
//        ->Arg(9300)
//        ->Arg(9400)
//        ->Arg(9500)
//        ->Arg(9600)
//        ->Arg(9700)
//        ->Arg(9800)
//        ->Arg(9900)
;

BENCHMARK_MAIN();
