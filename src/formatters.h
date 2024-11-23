#ifndef FORMATTERS_H
#define FORMATTERS_H
#pragma once


#include <spdlog/fmt/bundled/format.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"  // support for user defined types
#include "Mesh.h"
template <>
struct fmt::formatter<msh::Mesh> : fmt::formatter<std::string>{
    auto format(const msh::Mesh& mesh, format_context &ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "[Mesh {} points, {} vertices, {} faces]",mesh.GetPoints().size(), mesh.GetVertices().size(), mesh.GetFaces().size());
    }
};

template <>
struct fmt::formatter<msh::Face> : fmt::formatter<std::string>{
    auto format(const msh::Face& face, format_context &ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "[Face : vertices {} ]", face.GetVerticesIndex());
    }
};


#endif // FORMATTERS_H