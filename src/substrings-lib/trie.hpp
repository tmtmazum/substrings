#pragma once
#include <memory>
#include <vector>
#include <string>

namespace substrings
{

//! Trie data structure for quick matching of substrings
class trie
{
public:
    //! finds substring matches in the trie for a single word
    virtual std::vector<std::string> find_matches(std::string const& word) const = 0;

    //! virtual destructor
    virtual ~trie() = default;
};


//! Build trie data structure from a set of common words
std::shared_ptr<trie> build_trie(std::vector<std::string> const& words);

} // namespace substrings
