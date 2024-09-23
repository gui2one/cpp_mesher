#ifndef FORMATTERS_H
#define FORMATTERS_H
#pragma once

#include <fmt/format.h>
#include <Mesh.h>
template <> class fmt::formatter<msh::Mesh> {
public:
  constexpr auto parse (format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format (msh::Mesh const& mesh, Context& ctx) const {
      return fmt::v10::format_to(ctx.out(), "Mesh [{} Points, {} Faces]", mesh.GetPoints().size(), mesh.GetFaces().size());  // --== KEY LINE ==--
  }
};


#endif // FORMATTERS_H