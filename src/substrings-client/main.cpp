#include <substrings.hpp>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>

namespace {

unsigned int parse_uint(std::string_view arg, unsigned int fallback_value) noexcept
try
{
    return std::stoul(std::string{arg});
}
catch (std::invalid_argument const& inv)
{
    LOG_ERROR("failed to parse uint from argument '%hs' - invalid argument", std::string{arg}.c_str());
    return fallback_value;
}
catch (std::out_of_range const& out)
{
    LOG_ERROR("failed to parse uint from argument '%hs' - out of range", std::string{arg}.c_str());
    return fallback_value;
}

std::vector<std::string> read_words(std::filesystem::path const& file)
{
    std::vector<std::string> out;

    auto ifs = std::ifstream {file.c_str()};
    while (ifs.good())
    {
        std::string word;
        ifs >> word;
        if (!word.empty())
        {
            out.emplace_back(std::move(word));
        }
    }

    return out;
}

template <typename MatchList>
void print_matches(MatchList const& all_matches)
{
    for (auto const& matchlist : all_matches)
    {
        LOG_ALWAYS("%hs: ", matchlist.super.c_str());
        for (int i = 0; i < matchlist.matches.size(); ++i)
        {
            LOG_ALWAYS("%hs", matchlist.matches[i].c_str());
            if (i != matchlist.matches.size()-1)
            {
                LOG_ALWAYS(", ");
            }
        }
        LOG_ALWAYS("\n");
    }
}

} // namespace {}

int main(int argc, char** argv)
try
{
    auto const start_time = std::chrono::steady_clock::now();

    if (argc < 3)
    {
        LOG_ERROR("expected at least 3 arguments\n");
        LOG_ALWAYS("usage: ./substrings-client.exe <all-words-file> <common-words-file> [num_threads]\n");
        return 1;
    }

    auto const all_words_file = std::filesystem::path {argv[1]};
    auto const common_words_file = std::filesystem::path {argv[2]};

    std::error_code ec;
    if (!std::filesystem::exists(all_words_file, ec))
    {
        LOG_ERROR("couldn't find file '%ls'", all_words_file.c_str());
        return 1;
    }

    if (!std::filesystem::exists(common_words_file, ec))
    {
        LOG_ERROR("couldn't find file '%ls'", common_words_file.c_str());
        return 1;
    }

    auto const num_cores = std::max(1u, std::thread::hardware_concurrency() - 1u);
    auto const num_threads = (argc >= 4 ? parse_uint(argv[3], num_cores) : num_cores);

    auto const all_words = read_words(all_words_file);
    auto const common_words = read_words(common_words_file);

    auto const result = substrings::find_matches(all_words, common_words, num_threads);
    print_matches(result);

    auto const end_time = std::chrono::steady_clock::now();
    auto const ms = static_cast<size_t>(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count());
    LOG_DEBUG("completed in %zu ms\n", ms); 

    return 0;
}
catch (std::exception const& e)
{
    LOG_ERROR("unexpected error occurred : %hs", e.what());
    return 1;
}
