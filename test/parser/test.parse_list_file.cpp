#include <cmast/parser/parse_list_file.hpp>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string_view>

#include <catch.hpp>

using namespace cmast;

TEST_CASE("parser/parse_list_file", "[parser][integration]")
{
    static constexpr auto CMakeListsText = std::string_view(
        R"cmake(find_package(foobar 1.0 REQUIRED)

if(foobar)
    set(TEST "${foobar}")
endif()

message(STATUS ${TEST})
message(STATUS
  [=[some text]=]
)
)cmake");

    namespace fs = std::filesystem;

    auto const tmpPath = fs::temp_directory_path() / "CMakeLists.txt";

    {
        auto out = std::ofstream(tmpPath);
        out << CMakeListsText;
    }

    auto const deleteAtEnd
        = std::unique_ptr<fs::path const, void (*)(fs::path const*)>(
            &tmpPath, [](fs::path const* path) {
                fs::remove(*path); //
            });

    auto const parsed = parse_list_file(tmpPath);

    auto const strPath = std::string(tmpPath);
    auto const sinfo = [&](std::size_t n) { return SourceInfo{strPath, n}; };

    auto const expected = ListFile{
        strPath,
        {
            FunctionCall{
                sinfo(1),
                "find_package",
                {
                    FunctionArgument{
                        sinfo(1),
                        "foobar",
                        FunctionArgument::Unquoted,
                    },
                    FunctionArgument{
                        sinfo(1),
                        "1.0",
                        FunctionArgument::Unquoted,
                    },
                    FunctionArgument{
                        sinfo(1),
                        "REQUIRED",
                        FunctionArgument::Unquoted,
                    },
                },
            },
            FunctionCall{
                sinfo(3),
                "if",
                {
                    FunctionArgument{
                        sinfo(3),
                        "foobar",
                        FunctionArgument::Unquoted,
                    },
                },
            },
            FunctionCall{
                sinfo(4),
                "set",
                {
                    FunctionArgument{
                        sinfo(4),
                        "TEST",
                        FunctionArgument::Unquoted,
                    },
                    FunctionArgument{
                        sinfo(4),
                        "${foobar}",
                        FunctionArgument::Quoted,
                    },
                },
            },
            FunctionCall{
                sinfo(5),
                "endif",
                {},
            },
            FunctionCall{
                sinfo(7),
                "message",
                {
                    FunctionArgument{
                        sinfo(7),
                        "STATUS",
                        FunctionArgument::Unquoted,
                    },
                    FunctionArgument{
                        sinfo(7),
                        "${TEST}",
                        FunctionArgument::Unquoted,
                    },
                },
            },
            FunctionCall{
                sinfo(8),
                "message",
                {
                    FunctionArgument{
                        sinfo(8),
                        "STATUS",
                        FunctionArgument::Unquoted,
                    },
                    FunctionArgument{
                        sinfo(9),
                        "some text",
                        FunctionArgument::Bracketed,
                    },
                },
            },
        },
    };

    CHECK(parsed == expected);
}
