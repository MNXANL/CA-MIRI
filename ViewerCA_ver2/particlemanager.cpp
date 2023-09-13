#include "particlemanager.h"

#include "./mesh_io.h"
#include "./triangle_mesh.h"
using namespace std;


// Get all Q[v] matrices, for each v in vertices
void ParticleManager::calcQMatrices( std::vector<float>& vtx, std::vector<float>& norm ) {
    int SIZE = vtx.size() / 3;
    QMatrixPerVert.resize( 0 );
    for (int i = 0; i < SIZE; ++i) {
        Eigen::Vector3f myVec, myNorm;
        myVec = Eigen::Vector3f( vtx[3*i + 0], vtx[3*i + 1], vtx[3*i + 2] );
        myNorm = Eigen::Vector3f( norm[3*i + 0], norm[3*i + 1], norm[3*i + 2] );

        float myDotProd = myNorm.dot( myVec );

        Eigen::Vector4f Pv( norm[3*i + 0], norm[3*i + 1], norm[3*i + 2], -myDotProd );
        QMatrixPerVert.push_back( Pv * Pv.transpose() );
    }
}

// Get the new normals for a new set of vertices and faces
// similar to Mesh_IO::ComputeFaceNormals except with some optimizations
void ParticleManager::getNewNormals(const std::vector<float> &newVtx, const std::vector<int> &newFaces,   std::vector<float> &newNormals  ) {
    std::vector< float > faceNormals( newFaces.size(), 0.0f );
    //std::vector< float > newNormals( newVtx.size(), 0.0f);


    //for each face grab vtx 3-by-3, get the normal per face
    for (int i = 0; i < (int) newFaces.size(); i+=3) {
        // get vertices from face
        Eigen::Vector3f vtx1( newVtx[ 3*newFaces[i+0] + 0 ], newVtx[ 3*newFaces[i+0] + 1 ], newVtx[ 3*newFaces[i+0] + 2 ] );
        Eigen::Vector3f vtx2( newVtx[ 3*newFaces[i+1] + 0 ], newVtx[ 3*newFaces[i+1] + 1 ], newVtx[ 3*newFaces[i+1] + 2 ] );
        Eigen::Vector3f vtx3( newVtx[ 3*newFaces[i+2] + 0 ], newVtx[ 3*newFaces[i+2] + 1 ], newVtx[ 3*newFaces[i+2] + 2 ] );

        Eigen::Vector3f tri[3] = {vtx1, vtx2, vtx3};

        //get the face's normal via cross-product
        Eigen::Vector3f vtx1to2 = vtx2 - vtx1;
        Eigen::Vector3f vtx1to3 = vtx3 - vtx1;
        Eigen::Vector3f nrml = vtx1to2.cross( vtx1to3 );

        double norma = nrml.norm();
        if ( norma >= 1e-9 ) {
            //Sum normalized normal to faceNormals
            nrml.normalize();
            faceNormals[i + 0] = nrml[0];
            faceNormals[i + 1] = nrml[1];
            faceNormals[i + 2] = nrml[2];
        }
        //else  skip, already 0!

        // Angle per each triangle vertex
        for (int j = 0 ; j < 3 ; ++j) {
            // rotation: pivot per iteration is [ j -> j1 -> j2 ]

            //int j0 = (j+0) % 3;
            int j1 = (j+1) % 3;
            int j2 = (j+2) % 3;

            //get its normal
            vtx1to2 = tri[ j1 ] - tri[ j ];
            vtx1to3 = tri[ j2 ] - tri[ j ];

            float dotProd = vtx1to2.dot( vtx1to3 );
            dotProd = -(dotProd < 0.0f) * dotProd;
            double phi = acos( dotProd / (vtx1to3.norm()*vtx1to2.norm()) );

            //Sum faceNormals to the new normals w/ respective angle ponderation
            newNormals[ 3*newFaces[ i+j ] + 0 ] += phi*faceNormals[ i + 0 ];
            newNormals[ 3*newFaces[ i+j ] + 1 ] += phi*faceNormals[ i + 1 ];
            newNormals[ 3*newFaces[ i+j ] + 2 ] += phi*faceNormals[ i + 2 ];

        }
    }

    // Finally, normalize every single norm
    for (int i = 0; i < (int) newNormals.size(); i+=3) {
        Eigen::Vector3f nrml( newNormals[i + 0], newNormals[ i + 1 ], newNormals[ i + 2 ] );

        double norma = nrml.norm();
        if ( norma >= 1e-9 ) {
            //Sum normalized normal to newNormals
            nrml.normalize();
            newNormals[ i + 0 ] = nrml[0];
            newNormals[ i + 1 ] = nrml[1];
            newNormals[ i + 2 ] = nrml[2];
        }
    }
    //return newNormals;
}


bool vtxLEQ( Eigen::Vector3f& A, Eigen::Vector3f& B ) {
    bool X = A[0] <= B[0];
    bool Y = A[1] <= B[1];
    bool Z = A[2] <= B[2];
    return (X and Y and Z);
}





int ParticleManager::computeParticlePositions( std::vector<float>& vtx, std::vector<int>& faces, std::vector<float>& normals,
                                     Eigen::Vector3f& min, Eigen::Vector3f& max, std::string method )
{
    //Set up data structures
    NUM_PARTICLES = 1;
    vtxPerLOD.resize(NUM_PARTICLES);
    facesPerLOD.resize(NUM_PARTICLES);
    normPerLOD.resize(NUM_PARTICLES);

    // Use actual mesh values as LOD 0 (max detail)
    vtxPerLOD[0] = vtx ;
    facesPerLOD[0] = faces ;
    normPerLOD[0] = normals ;

    return NUM_PARTICLES;
}


