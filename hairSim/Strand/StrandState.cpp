#include "StrandState.hh"
#include "ElasticStrandUtils.hh"
#include "BandMatrix.hh"

#include "../Utils/Distances.hh"
#include "../Utils/TextLog.hh"

#define EIGEN_PERMANENTLY_DISABLE_STUPID_WARNINGS
#include <Eigen/Sparse>

StrandState::StrandState( const VecXx& dofs, BendingMatrixBase& bendingMatrixBase ) :
        m_numVertices( dofs.getNumVertices() ),
        m_totalEnergy( 0 ),
        m_dofs( dofs ),
        m_edges( m_dofs ),
        m_lengths( m_edges ),
        m_tangents( m_edges, m_lengths ),
        m_referenceFrames1( m_tangents ),
        m_referenceFrames2( m_tangents, m_referenceFrames1 ),
        m_referenceTwists( m_tangents, m_referenceFrames1 ),
        m_twists( m_referenceTwists, m_dofs ),
        m_curvatureBinormals( m_tangents ),
        m_trigThetas( m_dofs ),
        m_materialFrames1( m_trigThetas, m_referenceFrames1, m_referenceFrames2 ),
        m_materialFrames2( m_trigThetas, m_referenceFrames1, m_referenceFrames2 ),
        m_kappas( m_curvatureBinormals, m_materialFrames1, m_materialFrames2 ),
        m_gradKappas( m_lengths, m_tangents, m_curvatureBinormals, m_materialFrame
                m_materialFrames2, m_kappas ),
        m_gradTwists( m_lengths, m_curvatureBinormals ),
        m_gradTwistsSquared( m_gradTwists ),
        m_hessKappas( m_lengths, m_tangents, m_curvatureBinormals, m_materialFrame
                m_materialFrames2, m_kappas ),
        m_hessTwists( m_tangents, m_lengths, m_curvatureBinormals ),
        m_thetaHessKappas( m_curvatureBinormals, m_materialFrames1, m_materialFrames2 ),
        m_bendingProducts( bendingMatrixBase, m_gradKappas ),
{}

StrandState::~StrandState()
{}

void StrandState::resizeSelf()
{
    const size_t ndofs = getDegreesOfFreedom().size();
    assert( ndofs % 4 == 3 );
    // dofs are 3 per vertex, one per edge
    assert( ndofs > 3 );
    // minimum two vertices per strand

    m_totalForce.resize( ndofs );
}

void StrandState::freeCachedQuantities()
{
    m_curvatureBinormals.free();
    m_trigThetas.free();
    m_gradKappas.free();
    m_gradTwists.free();
    m_gradTwistsSquared.free();
    m_hessKappas.free();
    m_hessTwists.free();
    m_thetaHessKappas.free();
    m_bendingProducts.free();
}

bool StrandState::hasSmallForces( const Scalar lTwoTol, const Scalar lInfTol ) const
{
    return ( ( m_totalForce.norm() / m_numVertices <= lTwoTol )
          || ( m_totalForce.lpNorm<Eigen::Infinity>() <= lInfTol ) );
}


//! Computes H and uFree such that v = H q + uFree
/*! where v is the world velocitiy of the point at (edge, alpha)
and q the non-fixed degrees of freedom .
Allocates H if pH ( pointer to H ) is NULL
*/
void StrandState::computeDeformationGradient( const unsigned edge, const Scalar alpha,
        const VecXx &velocities, SparseRowMatx* &pH, Vec3x &freeVel ) const
{
    // if ( pH )
    // {
    //     pH->resize( 3, getDegreesOfFreedom().rows() );
    // }
    // else
    // {
    //     pH = new SparseRowMatx( 3, getDegreesOfFreedom().rows() );
    // }
//        delete pH;
        pH = new SparseRowMatx( 3, getDegreesOfFreedom().rows() );


    SparseRowMatx& H = *pH;
    H.reserve( alpha > 0. ? 6 : 3 );
#pragma omp critical
{
    for ( unsigned k = 0; k < 3; ++k )
    {
        H.startVec( k );
        const unsigned col = 4 * edge + k;
        H.insertBackByOuterInner( k, col ) = ( 1. - alpha );

        if ( alpha > 0. )
        {
            // H.insertBackByOuterInner( k, col + 4 ) = alpha;
            H.insertBack( k, col + 4 ) = alpha;
        }
    }
}

//ORIGINAL
    // if ( !edge )
    //     return;

    // for ( unsigned k = 0; k < 3; ++k )
    // {
    //     H.startVec( k );
    //     const unsigned col = 4 * edge + k;
    //     if ( edge == 1 )
    //     {
    //         freeVel[k] += ( 1. - alpha ) * velocities[col];
    //     }
    //     else
    //     {
    //         H.insertBackByOuterInner( k, col ) = ( 1. - alpha );
    //     }

    //     if ( alpha > 0. )
    //     {
    //         H.insertBackByOuterInner( k, col + 4 ) = alpha;
    //     }
    // }
    H.finalize();
}

Vec3x StrandState::closestPoint( const Vec3x& x ) const
{
    Scalar mindist = std::numeric_limits<Scalar>::max();
    Vec3x winner;

    for ( int vtx = 0; vtx < m_numVertices - 1; ++vtx )
    {
        Vec3x y = ClosestPtPointSegment( x, getVertex( vtx ), getVertex( vtx + 1 ) );
        Scalar dist = ( y - x ).squaredNorm();
        if ( dist < mindist )
        {
            mindist = dist;
            winner = y;
        }
    }

    return winner;
}

