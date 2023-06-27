#ifndef TESTS_LEIF_UTILS_COMMON_H
#define TESTS_LEIF_UTILS_COMMON_H

#include <algorithm>

#include <translatedstring.h>

namespace Common
{
bool compareTranslatedStringLists(const QList<Utils::TranslatedString> &lhs,
                                  const QList<Utils::TranslatedString> &rhs)
{
    constexpr auto predicate = [](const Utils::TranslatedString &lhs, const Utils::TranslatedString &rhs) {
        if(lhs.id() != rhs.id())
            return false;

        if(lhs.translatedId() != rhs.translatedId())
            return false;

        return true;
    };

    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), predicate);
}
}


#endif // !TESTS_LEIF_UTILS_COMMON_H
