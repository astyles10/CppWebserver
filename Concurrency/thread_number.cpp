#include <numeric>
#include <thread>

template <typename Iterator, typename T>
struct accumulate_block {
  void operator()(Iterator first, Iterator last, T& result) {
    result = std::accumulate(first, last, result);
  }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T& init) {
  const unsigned long length = std::distance(first, last);

  if (!length) return init;
  const unsigned long min_per_thread = 25;
  const unsigned long max_threads =
      (length + min_per_thread - 1) / min_per_thread;
  const unsigned long hardware_threads = std::thread::hardware_concurrency();
  const unsigned long num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
      
}

int main() { return 0; }