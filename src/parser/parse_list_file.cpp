#include <cmast/parser/parse_list_file.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>

#include <cmListFileCache.h>
#include <cmMessenger.h>
#include <cmState.h>
#include <cmStateSnapshot.h>

#include "convert_function_call.hpp"

namespace cmast {
    auto parse_list_file(std::filesystem::path const& filepath) -> ListFile
    {
        auto result = ListFile();
        result.filepath = filepath;

        auto listFile = cmListFile();
        auto state = cmState();
        auto messenger = cmMessenger(&state);
        auto snapshot = state.CreateBaseSnapshot();
        auto bt = cmListFileBacktrace(snapshot);

        bool successful
            = listFile.ParseFile(filepath.c_str(), &messenger, bt);

        assert(successful);

        result.calls.reserve(listFile.Functions.size());
        std::transform(listFile.Functions.begin(), listFile.Functions.end(),
            std::back_inserter(result.calls), [&](auto const& fn) {
                return detail::convert_function_call(result.filepath, fn);
            });

        return result;
    }
}
