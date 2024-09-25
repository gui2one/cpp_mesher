#ifndef CPP_MESHER_H
#define CPP_MESHER_H
#pragma once

#include <filesystem>
#include <memory>


#include "Log.h"

#define PI 3.141592653589793238462643383279502884197

namespace msh{

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr Ref<T> MakeRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

namespace fs = std::filesystem;

#endif