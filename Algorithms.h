//
// Created by Semyon Tikhonenko on 8/7/16.
//

#ifndef PITCHDETECTION_AZAZAI_ALGO_H
#define PITCHDETECTION_AZAZAI_ALGO_H

namespace CppUtils {
    template<typename Iterator, typename Predicate>
    bool Contains(Iterator begin, Iterator end, const Predicate& predicate) {
        for (auto iter = begin; iter != end; ++iter) {
            if (predicate(*iter)) {
                return true;
            }
        }

        return false;
    }

    template<typename Collection>
    bool ContainsValue(const Collection& collection, const typename Collection::value_type& value) {
        return Contains(collection.begin(), collection.end(), [&] (const auto& v) {
            return value == v;
        });
    }
}

#endif //PITCHDETECTION_AZAZAI_ALGO_H
