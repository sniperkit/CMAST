#include <catch.hpp>

#include <cmListFileCache.h>
#include <cmMessenger.h>
#include <cmState.h>

TEST_CASE("experiment")
{
    auto listFile = cmListFile();
    auto state = cmState();
    auto messenger = cmMessenger(&state);
}
