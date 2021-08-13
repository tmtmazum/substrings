#pragma once

#include "substrings-common.hpp"

#include <string>
#include <vector>

namespace substrings
{

//! A list of matches against a single word from all words list
struct matchlist
{
    //! super-word from all_words list
    std::string super;
    //! all substring matches from common words found in 'super'
    std::vector<std::string> matches;
};

//! finds all substring matches
//! \param all_words        list of words to search from
//! \param common_words     list of substring words to search using
//! \param num_threads      number of threads allowed in computation
std::vector<matchlist> find_matches(
    std::vector<std::string> const& all_words,
    std::vector<std::string> const& common_words,
    unsigned long                   num_threads
);

} // namespace substrings
