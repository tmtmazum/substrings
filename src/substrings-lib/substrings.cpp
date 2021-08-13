#include "substrings.hpp"
#include "trie.hpp"

#include <algorithm>
#include <iterator>
#include <thread>
#include <utility>

namespace substrings
{

//! find all substring matches
//! \param all_words        list of words to search from
//! \param common_words     list of substring words to search using
//! \param num_threads      number of threads allowed in computation
std::vector<matchlist> find_matches(
    std::vector<std::string> const& all_words,
    std::vector<std::string> const& common_words,
    unsigned long                   num_threads
)
{
    if (all_words.empty())
    {
        return {};
    }

    std::vector<matchlist> matches;

    matches.reserve(all_words.size());

    // Pre-make the entire vector by pushing all words with empty matches.
    // The threads will modify each entry in-place by index -- this allows
    // us to reuse the same container without needing to synchronize pushing to the matches vector.
    std::transform(begin(all_words), end(all_words), std::inserter(matches, matches.end()), [](auto const& word)
    {
        return matchlist{word, {}};
    });

    auto const n = all_words.size();
    auto const actual_threads = std::min(static_cast<size_t>(num_threads), n);
    auto const num_words_per_thread = (n / actual_threads) + !!(n % actual_threads);

    std::vector<std::thread> threads;
    threads.reserve(actual_threads);

    // Create a single trie object from common words and share it across all the threads
    // Because the trie is constant after creation it is safe to access concurrently.
    auto const t = build_trie(common_words);

    // divide and conquer -- each thread gets fixed number of words (except last in case of remainders)
    // e.g. for 7 total words and 2 threads, thread 0 gets words 0-3, thread 1 gets words 4-6
    for (auto i = size_t{0}; i < actual_threads; ++i)
    {
        threads.emplace_back(std::thread{[&, tid = i]()
        {
            for (auto j = size_t{0}; j < num_words_per_thread; ++j)
            {
                auto const index = (tid * num_words_per_thread) + j;
                if (index >= matches.size())
                    continue;

                auto& item = matches[index];

                // populate the matches for a single word
                item.matches = t->find_matches(item.super);
            }
        }});
    }

    for (auto& t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return matches;
}

}
