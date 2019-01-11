#include "intersect_mesh.h"
#include <iostream>

// ------------------------------------------- INITIALISATION ---------------------------------------------------------------

// Constructeur

using MeshT = IntersectMesh::MeshT;
using FaceHandle = MeshT::FaceHandle;

using namespace std;

IntersectMesh::IntersectMesh(MeshT & mesh01, MeshT & mesh02)
    : mesh1{mesh01}
    , mesh2{mesh02}
{
    mesh1.add_property(is_checked, "is_checked");
    mesh2.add_property(is_checked, "is_checked");

    auto initCheckAttribute = [this](auto & mesh)
    {
         std::for_each(mesh.faces_begin(), mesh.faces_end(),
                       [this, &mesh](auto const& fh)//
         {
            mesh.property(is_checked, fh) = 0;
         }
                       );
    };

    initCheckAttribute(mesh1);
    initCheckAttribute(mesh2);


}
#if 0
test_mesh::test_mesh()
{
//    this -> mesh1 = mesh01;
//    this -> mesh2 = mesh02;

    mesh1.request_vertex_colors() ; // Si on a besoin des couleurs
    mesh2.request_vertex_colors() ;

    genere_mesh1();
    genere_mesh2();

    // Propriété : 0 par défaut, 1 si triangle intersectant, 2 si boucle définie

    mesh1.add_property(is_checked, "is_checked");
    mesh2.add_property(is_checked, "is_checked");

    MeshT::FaceIter f_it ;
    std::vector <MeshT::FaceHandle> tab1;
    std::vector <MeshT::FaceHandle> tab2;

    for (f_it = mesh1.faces_begin(); f_it != mesh1.faces_end(); ++f_it)
    {
        mesh1.property(is_checked,*f_it) = 0;
        tab1.push_back(*f_it);
    }

    for (f_it = mesh2.faces_begin(); f_it != mesh2.faces_end(); ++f_it)
    {
        mesh2.property(is_checked,*f_it) = 0;
        tab2.push_back(*f_it);
    }

    // Calcul intersections

    genere_intersection(tab1, tab2);

    // Calcul boucles

    genere_boucles(intersection1[0], 2);

    // Affichage

    MeshT::VertexIter v_it ;
    MeshT::FaceVertexIter fvIt1 ;
    MeshT::FaceVertexIter fvIt2 ;

    for (v_it = mesh1.vertices_begin(); v_it != mesh1.vertices_end(); ++v_it)
    {
        mesh1.set_color(v_it, MeshT::Color(0.0f,1.0f,0.0f));
    }

    for (v_it = mesh2.vertices_begin(); v_it != mesh2.vertices_end(); ++v_it)
    {
        mesh2.set_color(v_it, MeshT::Color(0.0f,0.0f,1.0f));
    }

    //Affiches props

    for (f_it = mesh1.faces_begin(); f_it != mesh1.faces_end(); ++f_it)
    {
        fvIt1 = mesh1.fv_iter(*f_it);
        if (mesh1.property(is_checked,*f_it) == 1)
        {
            mesh1.set_color(*fvIt1++, MeshT::Color(0.0f,0.0f,0.0f));
            mesh1.set_color(*fvIt1++, MeshT::Color(0.0f,0.0f,0.0f));
            mesh1.set_color(*fvIt1++, MeshT::Color(0.0f,0.0f,0.0f));
        }
    }

    for (f_it = mesh1.faces_begin(); f_it != mesh1.faces_end(); ++f_it)
    {
        fvIt1 = mesh1.fv_iter(*f_it);
        if (mesh1.property(is_checked,*f_it) == 2)
        {
            mesh1.set_color(*fvIt1++, MeshT::Color(1.0f,1.0f,1.0f));
            mesh1.set_color(*fvIt1++, MeshT::Color(1.0f,1.0f,1.0f));
            mesh1.set_color(*fvIt1++, MeshT::Color(1.0f,1.0f,1.0f));
        }
    }

    for (f_it = mesh2.faces_begin(); f_it != mesh2.faces_end(); ++f_it)
    {
        fvIt1 = mesh2.fv_iter(*f_it);
        if (mesh2.property(is_checked,*f_it) == 1)
        {
            mesh2.set_color(*fvIt1++, MeshT::Color(0.0f,0.0f,0.0f));
            mesh2.set_color(*fvIt1++, MeshT::Color(0.0f,0.0f,0.0f));
            mesh2.set_color(*fvIt1++, MeshT::Color(0.0f,0.0f,0.0f));
        }
    }
}
#endif

// Getter

MeshT IntersectMesh::get_mesh1()
{
   return mesh1;
}

MeshT IntersectMesh::get_mesh2()
{
   return mesh2;
}

// ------------------------------------------- METHODES UTILES ---------------------------------------------------------------


// Intersection de triangles

bool IntersectMesh::intersect(FaceHandle fh1,FaceHandle fh2)
{
    // ---- Intialisation ----

    MeshT::FaceVertexIter fvIt1 = mesh1.fv_iter(fh1);
    MeshT::FaceVertexIter fvIt2 = mesh2.fv_iter(fh2);

    MeshT::Point v1[3] ;
    MeshT::Point v2[3] ;

    float d_v1[3] ;
    float d_v2[3] ;

    float p_v1[3] ;
    float p_v2[3] ;
    
    v1[0] = mesh1.point(*fvIt1++) ; // Triangle 1 T1
    v1[1] = mesh1.point(*fvIt1++) ;
    v1[2] = mesh1.point(*fvIt1++) ;
    
    v2[0] = mesh2.point(*fvIt2++) ; // Triangle 2 T2
    v2[1] = mesh2.point(*fvIt2++) ;
    v2[2] = mesh2.point(*fvIt2++) ;


    // ---- Definition des plans associés aux triangles ----

    MeshT::Point n1 = (v1[1] - v1[0]) % (v1[2] - v1[0]) ; // Normale T1
    float d1 = -n1|v1[0]; // Distance à 0 de T1

    MeshT::Point n2 = (v2[1] - v2[0]) % (v2[2] - v2[0]) ; // Idem T2
    float d2 = -n2|v2[0];
    
    // ---- Calculs des segments d'intersection (Si existant) ----

    d_v1[0] = (n2|v1[0]) + d2; // Distance (relative) des sommets de T1 au plan de T2
    d_v1[1] = (n2|v1[1]) + d2;
    d_v1[2] = (n2|v1[2]) + d2;

    d_v2[0] = (n1|v2[0]) + d1; // Idem T2
    d_v2[1] = (n1|v2[1]) + d1;
    d_v2[2] = (n1|v2[2]) + d1;

    if ((d_v1[0] * d_v1[1] < 0 || d_v1[1] * d_v1[2] < 0 || d_v1[0] * d_v1[2] < 0) && (d_v2[0] * d_v2[1] < 0 || d_v2[1] * d_v2[2] < 0 || d_v2[0] * d_v2[2] < 0))
    { // Si on a pas un triangle strictement "au dessus ou en dessous" de l'autre (Cas sans intersections)
        MeshT::Point tmp;
        float tmp2;

        if (d_v1[0] * d_v1[1] >= 0) // On impose le 2d sommet de T1 comme opposés au 2 autres selon T2 (Permutation si besoin)
        {
            tmp = v1[2]; v1[2] = v1[1]; v1[1] = tmp;
            tmp2 = d_v1[2] ; d_v1[2] = d_v1[1]; d_v1[1] = tmp2;
        }
        else if (d_v1[2] * d_v1[1] >= 0)
        {
            tmp = v1[0]; v1[0] = v1[1]; v1[1] = tmp;
            tmp2 = d_v1[0] ; d_v1[0] = d_v1[1]; d_v1[1] = tmp2;
        }

        if (d_v2[0] * d_v2[1] >= 0) // Idem T2
        {
            tmp = v2[2]; v2[2] = v2[1]; v2[1] = tmp;
            tmp2 = d_v2[2] ; d_v2[2] = d_v2[1]; d_v2[1] = tmp2;
        }
        else if (d_v2[2] * d_v2[1] >= 0)
        {
            tmp = v2[0]; v2[0] = v2[1]; v2[1] = tmp;
            tmp2 = d_v2[0] ; d_v2[0] = d_v2[1]; d_v2[1] = tmp2;
        }

        MeshT::Point d = n1 % n2 ; // Direction de l'intersection
        d.normalize();

        p_v1[0] = d|v1[0]; // Projetés des sommets de T1 sur la droite d'intersection
        p_v1[1] = d|v1[1];
        p_v1[2] = d|v1[2];

        p_v2[0] = d|v2[0]; // Idem T2
        p_v2[1] = d|v2[1];
        p_v2[2] = d|v2[2];

        float t1_1 = p_v1[0] + (p_v1[1] - p_v1[0]) * (d_v1[0] / (d_v1[0] - d_v1[1])); // Points limites d'intersection de T1 avec T2
        float t1_2 = p_v1[2] + (p_v1[1] - p_v1[2]) * (d_v1[2] / (d_v1[2] - d_v1[1]));

        float t2_1 = p_v2[0] + (p_v2[1] - p_v2[0]) * (d_v2[0] / (d_v2[0] - d_v2[1])); // Idem T2
        float t2_2 = p_v2[2] + (p_v2[1] - p_v2[2]) * (d_v2[2] / (d_v2[2] - d_v2[1]));

        if (t1_1 > t1_2) { tmp2 = t1_1 ; t1_1 = t1_2; t1_2 = tmp2;} // Ordonnancement
        if (t2_1 > t2_2) { tmp2 = t2_1 ; t2_1 = t2_2; t2_2 = tmp2;}

        cout << "T1: " << v1[0] << endl;
        cout << "T2: " << v1[1] << endl;
        cout << "T3: " << v1[2] << endl;
        cout << "T4: " << v2[0] << endl;
        cout << "T5: " << v2[1] << endl;
        cout << "T6: " << v2[2] << endl;
        cout << "n1: " << n1 << endl;
        cout << "n2: " << n2 << endl;
        cout << "d1: " << d1 << endl;
        cout << "d2: " << d2 << endl;
        cout << "dv1: " << d_v1[0] << " " << d_v1[1] << " " << d_v1[2] << endl;
        cout << "dv2: " << d_v2[0] << " " << d_v2[1] << " " << d_v2[2] << endl;
        cout << "d: " << d << endl;
        cout << "v1[0]: " << v1[0] << endl;
        cout << "v1[1]: " << v1[1] << endl;
        cout << "v1[2]: " << v1[2] << endl;
        cout << "v2[0]: " << v2[0] << endl;
        cout << "v2[1]: " << v2[1] << endl;
        cout << "v2[2]: " << v2[2] << endl;
        cout << "p_v1[0]: " << p_v1[0] << endl;
        cout << "p_v1[1]: " << p_v1[1] << endl;
        cout << "p_v1[2]: " << p_v1[2] << endl;
        cout << "Segment 1: " << t1_1 << " " << t1_2 << endl;
        cout << "Segment 2: " << t2_1 << " " << t2_2  << endl;

        return !(t1_2 <= t2_1 || t2_2 <= t1_1); // Segment non intersectés
    }

    return false;
}


void IntersectMesh::genere_intersection(std::vector<MeshT::FaceHandle> const& t1, std::vector<MeshT::FaceHandle> const& t2)
{
    for (uint i = 0; i < t1.size(); i++) // t1.size()
    {
       for (uint j = 0; j < t2.size(); j++) // t2.size()
       {
           cout << "( " << i << ", " << j << " )" << endl;
           if (intersect(t1[i], t2[j])) // Si les triangles s'intersectent
           {
               if (mesh1.property(is_checked, t1[i]) == 0) // Si son status n'était pas connu
               {
                   intersection1.push_back(t1[i]);    
                   mesh1.property(is_checked, t1[i]) = 1;
               }
               if (mesh2.property(is_checked, t2[j]) == 0)
               {
                   intersection2.push_back(t2[j]);
                   mesh2.property(is_checked, t2[j]) = 1;
               }
           }
       }
    }
}


void IntersectMesh::genere_boucles(MeshT::FaceHandle fh1, int indice)
{
    MeshT::FaceHalfedgeIter he_it;
    MeshT::FaceHandle fh2;
    MeshT::HalfedgeHandle ohe_h;

    if (intersection1.size() > 0)
    {
        auto it = std::find(intersection1.begin(), intersection1.end(), fh1); // Suppression de la face courante de la liste
        swap(*it, intersection1.back());
        intersection1.pop_back();

        he_it = mesh1.fh_iter(fh1);

        for (int i = 0; i < 3; i++) // Pour chaque face voisine
        {
            ohe_h = mesh1.opposite_halfedge_handle(*he_it);
            fh2 = mesh1.face_handle(ohe_h); // Face voisine

            if (mesh1.property(is_checked, fh2) == 1) // Intersecte, mais n'a pas encore été associé à une boucle
            {
                mesh1.property(is_checked, fh2) = indice ;
                genere_boucles(fh2, indice);
            }

            he_it++ ;
        }

        indice ++ ; // Si toutes les faces voisines sont traitées
        genere_boucles(intersection1[0], indice); // On passe à la boucle suivante
    }
}



// ------------------------------------------- GENERATION MESH ---------------------------------------------------------------

void IntersectMesh::genere_mesh1()
{
    // create 12 vertices of a icosahedron
    float t = (1.0 + sqrt(5.0)) / 2.0;
    std::vector<MeshT::VertexHandle>  face_vhandles;
    MeshT::VertexHandle vhandle[12];

    vhandle[0] = mesh1.add_vertex(MeshT::Point(-1,  t,  0));
    vhandle[1] = mesh1.add_vertex(MeshT::Point( 1,  t,  0));
    vhandle[2] = mesh1.add_vertex(MeshT::Point(-1, -t,  0));
    vhandle[3] = mesh1.add_vertex(MeshT::Point( 1, -t,  0));

    vhandle[4] = mesh1.add_vertex(MeshT::Point( 0, -1,  t));
    vhandle[5] = mesh1.add_vertex(MeshT::Point( 0,  1,  t));
    vhandle[6] = mesh1.add_vertex(MeshT::Point( 0, -1, -t));
    vhandle[7] = mesh1.add_vertex(MeshT::Point( 0,  1, -t));

    vhandle[8] = mesh1.add_vertex(MeshT::Point( t,  0, -1));
    vhandle[9] = mesh1.add_vertex(MeshT::Point( t,  0,  1));
    vhandle[10] = mesh1.add_vertex(MeshT::Point(-t,  0, -1));
    vhandle[11] = mesh1.add_vertex(MeshT::Point(-t,  0,  1));


    // 5 faces around point 0
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[11]);
    face_vhandles.push_back(vhandle[5]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[1]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[7]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[10]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[10]);
    face_vhandles.push_back(vhandle[11]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    // 5 adjacent faces

    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[9]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[11]);
    face_vhandles.push_back(vhandle[4]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[11]);
    face_vhandles.push_back(vhandle[10]);
    face_vhandles.push_back(vhandle[2]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[10]);
    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[6]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[8]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    // 5 faces around point 3

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[9]);
    face_vhandles.push_back(vhandle[4]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[2]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[6]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[8]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[8]);
    face_vhandles.push_back(vhandle[9]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    // 5 adjacent faces

    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[9]);
    face_vhandles.push_back(vhandle[5]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[11]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[10]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[8]);
    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[7]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[9]);
    face_vhandles.push_back(vhandle[8]);
    face_vhandles.push_back(vhandle[1]);
    mesh1.add_face(face_vhandles);
    face_vhandles.clear();
}


void IntersectMesh::genere_mesh2()
{
    // create 12 vertices of a icosahedron
    float t = (1.0 + sqrt(5.0)) / 2.0;
    std::vector<MeshT::VertexHandle>  face_vhandles;
    MeshT::VertexHandle vhandle[12];

    float t1 = 0.83;
    float t2 = 0.92;
    float t3 = 0.71;

    float a1 = 2.03;
    float a2 = 0.22;
    float a3 = 0.23;

    vhandle[0] = mesh2.add_vertex(MeshT::Point( a1 * (-1 + t1) - 1.f, a2 * ( t + t2), a3 *( 0 + t3)));
    vhandle[1] = mesh2.add_vertex(MeshT::Point( a1 * ( 1 + t1) - 1.f, a2 * ( t + t2), a3 *( 0 + t3)));
    vhandle[2] = mesh2.add_vertex(MeshT::Point( a1 * (-1 + t1)- 1.f, a2 * (-t + t2), a3 *( 0 + t3)));
    vhandle[3] = mesh2.add_vertex(MeshT::Point( a1 * ( 1 + t1)- 1.f, a2 * (-t + t2), a3 *( 0 + t3)));

    vhandle[4] = mesh2.add_vertex(MeshT::Point( a1 * (0 + t1)- 1.f, a2 * (-1 + t2), a3 *( t + t3)));
    vhandle[5] = mesh2.add_vertex(MeshT::Point( a1 * (0 + t1)- 1.f, a2 * ( 1 + t2), a3 *( t + t3)));
    vhandle[6] = mesh2.add_vertex(MeshT::Point( a1 * (0 + t1)- 1.f, a2 * (-1 + t2), a3 *(-t + t3)));
    vhandle[7] = mesh2.add_vertex(MeshT::Point( a1 * (0 + t1)- 1.f, a2 * ( 1 + t2), a3 *(-t + t3)));

    vhandle[8] = mesh2.add_vertex(MeshT::Point( a1 * (t + t1)- 1.f, a2 * ( 0 + t2), a3 *(-1 + t3)));
    vhandle[9] = mesh2.add_vertex(MeshT::Point( a1 * (t + t1)- 1.f, a2 * ( 0 + t2), a3 *( 1 + t3)));
    vhandle[10] = mesh2.add_vertex(MeshT::Point(a1 * (-t + t1)- 1.f,a2 * (  0 + t2),a3 *( -1 + t3)));
    vhandle[11] = mesh2.add_vertex(MeshT::Point(a1 * (-t + t1)- 1.f,a2 * (  0 + t2),a3 *(  1 + t3)));

    // 5 faces around point 0
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[11]);
    face_vhandles.push_back(vhandle[5]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[1]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[7]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[10]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[0]);
    face_vhandles.push_back(vhandle[10]);
    face_vhandles.push_back(vhandle[11]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    // 5 adjacent faces

    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[9]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[5]);
    face_vhandles.push_back(vhandle[11]);
    face_vhandles.push_back(vhandle[4]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[11]);
    face_vhandles.push_back(vhandle[10]);
    face_vhandles.push_back(vhandle[2]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[10]);
    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[6]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[7]);
    face_vhandles.push_back(vhandle[1]);
    face_vhandles.push_back(vhandle[8]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    // 5 faces around point 3

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[9]);
    face_vhandles.push_back(vhandle[4]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[2]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[6]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[8]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[3]);
    face_vhandles.push_back(vhandle[8]);
    face_vhandles.push_back(vhandle[9]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    // 5 adjacent faces

    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[9]);
    face_vhandles.push_back(vhandle[5]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[4]);
    face_vhandles.push_back(vhandle[11]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[2]);
    face_vhandles.push_back(vhandle[10]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[8]);
    face_vhandles.push_back(vhandle[6]);
    face_vhandles.push_back(vhandle[7]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

    face_vhandles.push_back(vhandle[9]);
    face_vhandles.push_back(vhandle[8]);
    face_vhandles.push_back(vhandle[1]);
    mesh2.add_face(face_vhandles);
    face_vhandles.clear();

}

// Intersection de triangles

//bool test_mesh::intersect(FaceHandle fh1,FaceHandle fh2)
//{
//    // ---- Intialisation ----

//    MyMesh::FaceVertexIter fvIt1 = mesh1.fv_iter(fh1);
//    MyMesh::FaceVertexIter fvIt2 = mesh2.fv_iter(fh2);

//    MyMesh::Point v1[3] ;
//    MyMesh::Point v2[3] ;

//    float d_v1[3] ;
//    float d_v2[3] ;

//    float p_v1[3] ;
//    float p_v2[3] ;

//    v1[0] = mesh1.point(*fvIt1++) ; // Triangle 1 T1
//    v1[1] = mesh1.point(*fvIt1++) ;
//    v1[2] = mesh1.point(*fvIt1++) ;

//    v2[0] = mesh2.point(*fvIt2++) ; // Triangle 2 T2
//    v2[1] = mesh2.point(*fvIt2++) ;
//    v2[2] = mesh2.point(*fvIt2++) ;

//    MyMesh::Point sol[2];
//    sol[0] = MyMesh::Point(0,0,0);
//    sol[1] = MyMesh::Point(0,0,0);

//    // ---- Definition des plans associés aux triangles ----

//    MyMesh::Point n1 = (v1[1] - v1[0]) % (v1[2] - v1[0]) ; // Normale T1
//    float d1 = -n1|v1[0]; // Distance à 0 de T1

//    MyMesh::Point n2 = (v2[1] - v2[0]) % (v2[2] - v2[0]) ; // Idem T2
//    float d2 = -n2|v2[0];

//    // ---- Calculs des segments d'intersection (Si existant) ----

//    d_v1[0] = (n2|v1[0]) + d2; // Distance (relative) des sommets de T1 au plan de T2
//    d_v1[1] = (n2|v1[1]) + d2;
//    d_v1[2] = (n2|v1[2]) + d2;

//    d_v2[0] = (n1|v2[0]) + d1; // Idem T2
//    d_v2[1] = (n1|v2[1]) + d1;
//    d_v2[2] = (n1|v2[2]) + d1;

//    cout << endl << "T1: " << v1[0] << endl;
//    cout  << "T2: " << v1[1] << endl;
//    cout  << "T3: " << v1[2] << endl;
//    cout  << "T4: " << v2[0] << endl;
//    cout  << "T5: " << v2[1] << endl;
//    cout  << "T6: " << v2[2] << endl;

//    cout << endl << "n1: " << n1 << endl;
//    cout << "n2: " << n2 << endl;
//    cout << endl << "d1: " << d1 << endl;
//    cout << "d2: " << d2 << endl;
//    cout << endl << "dv1: " << d_v1[0] << " " << d_v1[1] << " " << d_v1[2] << endl;
//    cout << "dv2: " << d_v2[0] << " " << d_v2[1] << " " << d_v2[2] << endl;

//    if ((d_v1[0] * d_v1[1] < 0 || d_v1[1] * d_v1[2] < 0 || d_v1[0] * d_v1[2] < 0) && (d_v2[0] * d_v2[1] < 0 || d_v2[1] * d_v2[2] < 0 || d_v2[0] * d_v2[2] < 0))
//    { // Si on a pas un triangle strictement "au dessus ou en dessous" de l'autre (Cas sans intersections)
//        MyMesh::Point tmp;
//        float tmp2;

//        if (d_v1[0] * d_v1[1] >= 0) // On impose le 2d sommet de T1 comme opposés au 2 autres selon T2 (Permutation si besoin)
//        {
//            tmp = v1[2]; v1[2] = v1[1]; v1[1] = tmp;
//            tmp2 = d_v1[2] ; d_v1[2] = d_v1[1]; d_v1[1] = tmp2;
//        }
//        else if (d_v1[2] * d_v1[1] >= 0)
//        {
//            tmp = v1[0]; v1[0] = v1[1]; v1[1] = tmp;
//            tmp2 = d_v1[0] ; d_v1[0] = d_v1[1]; d_v1[1] = tmp2;
//        }

//        if (d_v2[0] * d_v2[1] >= 0) // Idem T2
//        {
//            tmp = v2[2]; v2[2] = v2[1]; v2[1] = tmp;
//            tmp2 = d_v2[2] ; d_v2[2] = d_v2[1]; d_v2[1] = tmp2;
//        }
//        else if (d_v2[2] * d_v2[1] >= 0)
//        {
//            tmp = v2[0]; v2[0] = v2[1]; v2[1] = tmp;
//            tmp2 = d_v2[0] ; d_v2[0] = d_v2[1]; d_v2[1] = tmp2;
//        }

//        MyMesh::Point d = n1 % n2 ; // Direction de l'intersection
//        d.normalize();
//        MyMesh::Point A;

//        if (abs(d[0] * d[1] * d[2]) > 0) // Calcul d'un point de l'intersection par Cramer avec z = 0
//        {
//            A[0] = (d2 * n1[1] - d1 * n2[1]) / (n1[0] * n2[1] - n2[0] * n1[1]);
//            A[1] = (d1 * n2[0] - d2 * n1[0]) / (n1[0] * n2[1] - n2[0] * n1[1]);
//            A[2] = 0;
//        }

//        p_v1[0] = d|(v1[0] - A); // Projetés des sommets de T1 sur la droite d'intersection
//        p_v1[1] = d|(v1[1] - A);
//        p_v1[2] = d|(v1[2] - A);

//        p_v2[0] = d|(v2[0] - A); // Idem T2
//        p_v2[1] = d|(v2[1] - A);
//        p_v2[2] = d|(v2[2] - A);

//        float t1_1 = p_v1[0] + (p_v1[1] - p_v1[0]) * (d_v1[0] / (d_v1[0] - d_v1[1])); // Points limites d'intersection de T1 avec T2
//        float t1_2 = p_v1[2] + (p_v1[1] - p_v1[2]) * (d_v1[2] / (d_v1[2] - d_v1[1]));

//        float t2_1 = p_v2[0] + (p_v2[1] - p_v2[0]) * (d_v2[0] / (d_v2[0] - d_v2[1])); // Idem T2
//        float t2_2 = p_v2[2] + (p_v2[1] - p_v2[2]) * (d_v2[2] / (d_v2[2] - d_v2[1]));

//        if (t1_1 > t1_2) { tmp2 = t1_1 ; t1_1 = t1_2; t1_2 = tmp2;} // Ordonnancement
//        if (t2_1 > t2_2) { tmp2 = t2_1 ; t2_1 = t2_2; t2_2 = tmp2;}

//        cout << "A: " << A << endl;
//        cout << "d: " << d << endl;
//        cout << "v1[0]: " << v1[0] << endl;
//        cout << "v1[1]: " << v1[1] << endl;
//        cout << "v1[2]: " << v1[2] << endl;
//        cout << "p_v1[0]: " << p_v1[0] << endl;
//        cout << "p_v1[1]: " << p_v1[1] << endl;
//        cout << "p_v1[2]: " << p_v1[2] << endl;
//        cout << "Segment 1: " << t1_1 << " " << t1_2 << endl;
//        cout << "Segment 2: " << t2_1 << " " << t2_2  << endl;

//        // Segment d'intersection éventuel

//        if (t1_1 > t2_1)
//        {
//            if (t1_2 < t2_2)
//            {
//               sol[0] = A + t1_1 * d;
//               sol[1] = A + t1_2 * d;
//            }
//            else if (t1_1 < t2_2)
//            {
//               sol[0] = A + t1_1 * d;
//               sol[1] = A + t2_2 * d;
//            }
//        }
//        else
//        {
//            if (t2_2 < t1_2)
//            {
//              sol[0] = A + t2_1 * d;
//              sol[1] = A + t2_2 * d;
//            }
//            else if (t2_1 < t1_2)
//            {
//              sol[0] = A + t2_1 * d;
//              sol[1] = A + t1_2 * d;
//            }
//        }
//        cout << "TEST";
//    }
//    else cout << "Cas d'arrêt" ;
//    cout << "BASIQUE TEST";

//    return (sol[0] != MyMesh::Point(0, 0, 0) || sol[1] != MyMesh::Point(0, 0, 0));
//}
