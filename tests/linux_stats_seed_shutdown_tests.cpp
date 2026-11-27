#include "stats_store.h"

#include <atomic>
#include <cstdio>
#include <future>
#include <thread>

int main() {
    std::promise<void> accountRequested;
    std::atomic<bool> firstRequest{true};
    std::atomic<bool> stopping{false};
    StatsStore::SetAccountIdProvider([&] {
        if (firstRequest.exchange(false))
            accountRequested.set_value();
        return 0u;
    });

    auto requested = accountRequested.get_future();
    std::thread seed([&] { StatsStore::SeedApps({}, &stopping); });
    requested.wait();
    stopping.store(true);
    seed.join();

    if (!StatsStore::WaitForSeed(0)) {
        std::fprintf(stderr, "Stopping the seed did not release waiters\n");
        return 1;
    }
    return 0;
}
