#include <substrings.hpp>
#include <gtest/gtest.h>

#include <thread>

namespace {

using namespace substrings;

TEST(substring_test, basic_singlethreaded)
{
    auto const a = find_matches({"that", "pat"}, {"at", "that", "hat"}, 1);

    EXPECT_EQ(a[0].super, "that");
    EXPECT_EQ(a[0].matches[0], "at");
    EXPECT_EQ(a[0].matches[1], "hat");
    EXPECT_EQ(a[0].matches[2], "that");
    EXPECT_EQ(a[0].matches.size(), 3);

    EXPECT_EQ(a[1].super, "pat");
    EXPECT_EQ(a[1].matches[0], "at");
    EXPECT_EQ(a[1].matches.size(), 1);
}

TEST(substring_test, basic_multithreaded)
{
    auto const a = find_matches({"that", "pat"}, {"at", "that", "hat"}, std::thread::hardware_concurrency());

    EXPECT_EQ(a[0].super, "that");
    EXPECT_EQ(a[0].matches[0], "at");
    EXPECT_EQ(a[0].matches[1], "hat");
    EXPECT_EQ(a[0].matches[2], "that");
    EXPECT_EQ(a[0].matches.size(), 3);

    EXPECT_EQ(a[1].super, "pat");
    EXPECT_EQ(a[1].matches[0], "at");
    EXPECT_EQ(a[1].matches.size(), 1);
}

TEST(substring_test, basic_caps)
{
    auto const a = find_matches({"THAT", "PAT"}, {"AT", "THAT", "HAT"}, 1);

    EXPECT_EQ(a[0].super, "THAT");
    EXPECT_EQ(a[0].matches[0], "AT");
    EXPECT_EQ(a[0].matches[1], "HAT");
    EXPECT_EQ(a[0].matches[2], "THAT");
    EXPECT_EQ(a[0].matches.size(), 3);

    EXPECT_EQ(a[1].super, "PAT");
    EXPECT_EQ(a[1].matches[0], "AT");
    EXPECT_EQ(a[1].matches.size(), 1);
}


TEST(substring_test, empty)
{
    auto const a = find_matches({}, {"at", "that", "hat"}, std::thread::hardware_concurrency());
    EXPECT_TRUE(a.empty());

    auto const b = find_matches({"that", "pat"}, {}, std::thread::hardware_concurrency());

    EXPECT_EQ(b[0].super, "that");
    EXPECT_EQ(b[0].matches.size(), 0);
    EXPECT_EQ(b[1].super, "pat");
    EXPECT_EQ(b[1].matches.size(), 0);
}

TEST(substring_test, multimatch)
{
    auto const a = find_matches({"ratata"}, {"at"}, 1);

    EXPECT_EQ(a[0].super, "ratata");
    EXPECT_EQ(a[0].matches[0], "at");
    EXPECT_EQ(a[0].matches.size(), 1);
}

TEST(substring_test, multimatch_dups)
{
    auto const a = find_matches({"ratata"}, {"ata"}, 1);

    EXPECT_EQ(a[0].super, "ratata");
    EXPECT_EQ(a[0].matches[0], "ata");
    EXPECT_EQ(a[0].matches.size(), 1);
}

TEST(substring_test, exact_match)
{
    auto const a = find_matches({"ratata", "boo"}, {"ratata"}, 1);

    EXPECT_EQ(a[0].super, "ratata");
    EXPECT_EQ(a[0].matches[0], "ratata");
    EXPECT_EQ(a[0].matches.size(), 1);
    EXPECT_EQ(a[1].super, "boo");
    EXPECT_EQ(a[1].matches.size(), 0);
}

TEST(substring_test, alphabetic)
{
    auto const a = find_matches({"accd", "efgh"}, {"h","g","f","e","c"}, 1);

    EXPECT_EQ(a[0].super, "accd");
    EXPECT_EQ(a[0].matches[0], "c");
    EXPECT_EQ(a[0].matches.size(), 1);
    EXPECT_EQ(a[1].super, "efgh");
    EXPECT_EQ(a[1].matches[0], "e");
    EXPECT_EQ(a[1].matches[1], "f");
    EXPECT_EQ(a[1].matches[2], "g");
    EXPECT_EQ(a[1].matches[3], "h");
    EXPECT_EQ(a[1].matches.size(), 4);
}


TEST(substring_test, symbols)
{
    auto const a = find_matches({"+_)_!%&$$&!#@$", "(&@^*!#)&$@^"}, {"$$", "!"}, 1);

    EXPECT_EQ(a[0].super, "+_)_!%&$$&!#@$");
    EXPECT_EQ(a[0].matches[0], "!");
    EXPECT_EQ(a[0].matches[1], "$$");
    EXPECT_EQ(a[0].matches.size(), 2);
    EXPECT_EQ(a[1].super, "(&@^*!#)&$@^");
    EXPECT_EQ(a[1].matches[0], "!");
    EXPECT_EQ(a[1].matches.size(), 1);
}

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
