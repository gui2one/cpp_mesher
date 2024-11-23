#ifndef FORMATTERS_H
#define FORMATTERS_H
#pragma once

#include <fmt/format.h>
#include "spdlog/fmt/ostr.h"  // support for user defined types
#include "./Mesh.h"
template <>
struct fmt::formatter<msh::Mesh> : fmt::formatter<std::string>{
    auto format(const msh::Mesh& mesh, format_context &ctx) const -> decltype(ctx.out()) {
        return fmt::v10::format_to(ctx.out(), "[Mesh {} points, {} vertices, {} faces]", mesh.GetPoints().size(), mesh.GetVertices().size(), mesh.GetFaces().size());
    }
};


#endif // FORMATTERS_H