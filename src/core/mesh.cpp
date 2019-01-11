#include "mesh.h"

#include <algorithm>
#include <functional>
#include <stdexcept>

Mesh::Mesh( std::string filename )
{
    load( filename );
}

void Mesh::load( std::string filename )
{
    if ( !OpenMesh::IO::read_mesh( mesh, filename ) )
    {
        throw std::runtime_error( "Mesh loading error" );
    }
}

void Mesh::refreshBuffer()
{
    int const numberColors          = 3;
    int const verticesNumberPerFace = 3;

    auto numIndexes = mesh.n_faces() * verticesNumberPerFace;

    indexes_.clear();
    indexes_.reserve( numIndexes );


    std::for_each( mesh.faces_begin(), mesh.faces_end(), [this]( auto const &faces ) {
        std::transform( mesh.cfv_begin( faces ), mesh.cfv_end( faces ),
                        std::back_inserter( indexes_ ),
                        []( auto const &faceVertex ) { return faceVertex.idx(); } );
    } );


    // let's put a color
    colors_.clear();
    colors_.reserve( mesh.n_vertices() * numberColors );


    std::for_each( mesh.vertices_begin(), mesh.vertices_end(), [this]( auto const &vertex ) {
        auto const &color = mesh.color( vertex );

        std::copy( std::begin( color ), std::end( color ), std::back_inserter( colors_ ) );
    } );
}
