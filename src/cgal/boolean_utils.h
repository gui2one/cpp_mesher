#ifndef CPP_MESHER_BOOLEAN_UTILS_H
#define CPP_MESHER_BOOLEAN_UTILS_H
#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
 
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
 
#include <iostream>
#include <string>
 
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef boost::graph_traits<Mesh>::vertex_descriptor vertex_descriptor;
typedef Mesh::Property_map<vertex_descriptor,EK::Point_3> Exact_point_map;
 
namespace PMP = CGAL::Polygon_mesh_processing;
namespace params = CGAL::parameters;
 
struct Exact_vertex_point_map
{
  // typedef for the property map
  typedef boost::property_traits<Exact_point_map>::value_type value_type;
  typedef boost::property_traits<Exact_point_map>::reference reference;
  typedef boost::property_traits<Exact_point_map>::key_type key_type;
  typedef boost::read_write_property_map_tag category;
 
  // exterior references
  Exact_point_map exact_point_map;
  Mesh* tm_ptr;
 
  // Converters
  CGAL::Cartesian_converter<K, EK> to_exact;
  CGAL::Cartesian_converter<EK, K> to_input;
 
  Exact_vertex_point_map()
    : tm_ptr(nullptr)
  {}
 
  Exact_vertex_point_map(const Exact_point_map& ep, Mesh& tm)
    : exact_point_map(ep)
    , tm_ptr(&tm)
  {
    for (Mesh::Vertex_index v : vertices(tm))
      exact_point_map[v]=to_exact(tm.point(v));
  }
 
  friend
  reference get(const Exact_vertex_point_map& map, key_type k)
  {
    CGAL_precondition(map.tm_ptr!=nullptr);
    return map.exact_point_map[k];
  }
 
  friend
  void put(const Exact_vertex_point_map& map, key_type k, const EK::Point_3& p)
  {
    CGAL_precondition(map.tm_ptr!=nullptr);
    map.exact_point_map[k]=p;
    // create the input point from the exact one
    map.tm_ptr->point(k)=map.to_input(p);
  }
};
#endif // CPP_MESHER_BOOLEAN_UTILS_H