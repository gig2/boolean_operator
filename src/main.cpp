
#include "gui/simpleWindow.h"
#include "gui/mainWindow.h"

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <QApplication>

#include <iostream>



using MyMesh = OpenMesh::PolyMesh_ArrayKernelT<>;

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    SimpleWindowWithGL window;

    window.show();

    return app.exec();
}
