#include "DOFScriptingController.h"

#include "StrandState.h"
#include "ElasticStrandUtils.h"
#include "../Math/BandMatrix.h"

DOFScriptingController::DOFScriptingController()
{}

DOFScriptingController::~DOFScriptingController()
{}

void DOFScriptingController::fixLHS( JacobianMatrixType& LHS ) const
{
    for( std::map<int, Scalar>::const_iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {
        LHS.fixFirstDOFs<1>( dof->first );
    }
}

void DOFScriptingController::fixRHS( VecXx& rhs ) const
{
    for( std::map<int, Scalar>::const_iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {        
        rhs[dof->first] = 0;
    }
}

void DOFScriptingController::enforceDisplacements( VecXx& displacements ) const
{
    for( std::map<int, Scalar>::const_iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {
        displacements[dof->first] = dof->second;
    }
}

void DOFScriptingController::enforceVelocities( VecXx& velocities, Scalar dt ) const
{
    for( std::map<int, Scalar>::const_iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {
        velocities[dof->first] = dof->second / dt;
    }
}

void DOFScriptingController::fixLHSAndRHS( JacobianMatrixType& LHS, VecXx& rhs, Scalar dt ) const
{
    VecXx velocities( VecXx::Zero( rhs.rows() ) );

    for( std::map<int, Scalar>::const_iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {
        velocities[dof->first] = dof->second / dt;
    }

    LHS.multiply( rhs, -1, velocities );

    fixLHS( LHS );

    for( std::map<int, Scalar>::const_iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {
        rhs[dof->first] = velocities[dof->first];
    }
}

void DOFScriptingController::computeRigidBodyMotion( VecXx& futureDOFs, const VecXx& currentDOFs )
{
    futureDOFs.resize( currentDOFs.size() );
    VecXx displacements( currentDOFs.size() );
    enforceDisplacements( displacements );

    const int m_ndof = currentDOFs.size();
    const Vec3 p0 = currentDOFs.segment<3>( 0 );
    const Vec3 p1 = currentDOFs.segment<3>( 4 );
    const Vec3 w0 = displacements.segment<3>( 0 );
    const Vec3 w1 = displacements.segment<3>( 4 );
    const Vec3 q0 = p0 + w0;
    const Vec3 q1 = p1 + w1;

    if( !isSmall( square( ( q1 - q0 ).squaredNorm() - ( p1 - p0 ).squaredNorm() ) ) ){
        std::cerr << "First edge length is not constant, diff "
                << fabs( ( q1 - q0 ).squaredNorm() - ( p1 - p0 ).squaredNorm() ) << std::endl;
    }

    Vec3 u = ( p1 - p0 );
    const Scalar un = u.norm();
    if( !isSmall( un ) )
        u /= un;

    Vec3 v = ( q1 - q0 );
    const Scalar vn = v.norm();
    if( !isSmall( vn ) )
        v /= vn;

    if ( isSmall( u.cross( v ).squaredNorm() ) ) // pure translation
    {
        for ( int i = 0; i < m_ndof; i += 4 )
        {
            futureDOFs.segment<3>( i ) = currentDOFs.segment<3>( i ) + w0;

        }
    }
    else // rigid motion
    {
        for ( int i = 0; i < m_ndof; i += 4 )
        {
            const Vec3 pi = currentDOFs.segment<3>( i );
            futureDOFs.segment<3>( i ) = q0 + parallelTransport( pi - p0, u, v );

        }
    }
    // Finally just copy the thetas
    for ( int i = 7; i < m_ndof; i += 4 )
    {
        futureDOFs[i] = currentDOFs[i];
    }
}

void DOFScriptingController::setToUnsimulatedPositions( VecXx& futureDOFs, const VecXx& currentDOFs, bool rootAsWell )
{
    VecXx displacements( currentDOFs.size() );
    displacements.setZero();
    enforceDisplacements( displacements );
    futureDOFs = currentDOFs + displacements;

    // Set thetas to zero
    for ( unsigned i = 7; i < currentDOFs.size(); i += 4 )
    {
        futureDOFs[i] = 0;
    }
    if ( rootAsWell )
    {
        futureDOFs[3] = 0;
    }

    for ( std::map<int, Scalar>::iterator dof = m_scriptedDisplacements.begin();
            dof != m_scriptedDisplacements.end(); 
            ++dof )
    {
        dof->second = 0;
    }

    std::cerr << "setToUnsimulatedPositions needs verification " << std::endl;
}
