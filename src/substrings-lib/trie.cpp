#include "trie.hpp"

#include <memory>
#include <unordered_map>
#include <algorithm>
#include <cassert>

namespace substrings
{

//! custom hash function for a character
//! if the character falls in the range [a, z], then
//! the character index from 'a' is returned
struct char_hash
{
    std::hash<char> fallback_hash;

    size_t operator()(char const c) const noexcept
    {
        // use fallback hash if its outside the common ascii set
        if (c < ' ' || c > '~')
            return fallback_hash(c);
        return static_cast<size_t>(c) - ' ';
    }
};

//! A single node for a trie
//! Has links to children node, implemented as an unordered_map
class trie_node
{
public:
    //! inserts a node into the trie at this position if no sub-node exists with that character
    //! otherwise returns the existing sub-node at that character
    trie_node* insert_or_get(char c)
    {
        return m_sub.emplace(c, std::make_unique<trie_node>()).first->second.get();
    }

    //! marks this node as the end of a word
    void mark_end(std::string const& w) noexcept
    {
        m_is_end = true;
        m_word = w;
    }

    //! returns true if this is the end of a valid substring
    bool is_end() const noexcept { return m_is_end; }

    //! returns the word associated with the end of this substring
    //! \pre    is_end() must be true
    std::string const& get_word() const noexcept {
        assert(is_end());
        assert(m_word.size());
        return m_word;
    }

    //! returns the sub-node in the trie matching 'c' if one exists
    //! returns nullptr otherwise
    trie_node const* try_match(char c) const
    {
        auto const it = m_sub.find(c);
        return it == m_sub.end() ? nullptr : it->second.get();
    }

private:
    using sub_map = std::unordered_map<char, std::unique_ptr<trie_node>, char_hash>;

    sub_map     m_sub;
    std::string m_word;
    bool        m_is_end = false;
};

//! Implementation of trie data structure
class trie_impl final : public trie
{
public:
    trie_impl(std::vector<std::string> const& words)
    {
        for (auto const& word : words)
        {
            trie_node* n = &m_master_node;
            for (auto c : word)
            {
                n = n->insert_or_get(c);
            }
            n->mark_end(word);
        }
    }

    //! finds substring matches in the trie for a single word
    std::vector<std::string> find_matches(std::string const& word) const override
    {
        std::vector<std::string> matches;

        using node_list = std::list<trie_node const*>;

        node_list partial_matches {&m_master_node};

        for (int i = 0; i < word.size(); ++i)
        {
            node_list to_add;
            std::vector<node_list::iterator> to_remove;

            for (auto it = partial_matches.begin(); it != partial_matches.end(); it++)
            {
                auto const node_ptr = (*it);

                auto const next = node_ptr->try_match(word.at(i));
                if (next)
                {
                    if (next->is_end())
                    {
                        matches.emplace_back(next->get_word());
                    }
                    else
                    {
                        to_add.emplace_back(next);
                    }
                }

                if (node_ptr != &m_master_node)
                {
                    to_remove.emplace_back(it);
                }
            } // O(k) where k:max number of simultaneous running matches

            for (auto const& rm : to_remove)
            {
                partial_matches.erase(rm);
            }
            partial_matches.splice(partial_matches.end(), std::move(to_add));
        } // O(k*m) where m:word.size() and k:max number of simultaneous matches

        std::sort(matches.begin(), matches.end()); // O(n log n) where n: number of matches

        // remove duplicates
        auto const last = std::unique(matches.begin(), matches.end());
        matches.erase(last, matches.end());

        return matches;
    } // O(max(m, n log n)) where m:size of word, n:number of matches within word

private:
    trie_node m_master_node;
};


std::shared_ptr<trie> build_trie(std::vector<std::string> const& words)
{
    return std::make_shared<trie_impl>(words);
}

} // namespace substrings
