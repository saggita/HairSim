#ifndef MATERIALFRAMES_HH_
#define MATERIALFRAMES_HH_

#include "ReferenceFrames.hh"

/**
 * Unit: no dimension
 */
template<int FrameN>
class MaterialFrames: public DependencyNode<Vec3xArray>
{
public:
    MaterialFrames( TrigThetas& trigThetas, ReferenceFrames1& referenceFrames1,
            ReferenceFrames2& referenceFrames2 ) :
            DependencyNode<Vec3xArray>( 0, referenceFrames1.size() ), m_trigThetas( trigThetas ), m_referenceFrames1(
                    referenceFrames1 ), m_referenceFrames2( referenceFrames2 )
    {
        m_trigThetas.addDependent( this );
        m_referenceFrames1.addDependent( this );
        m_referenceFrames2.addDependent( this );
    }

    virtual const char* name() const;

protected:
    virtual void compute();
    Vec3x linearMix( const Vec3x& u, const Vec3x& v, Scalar s, Scalar c );

    TrigThetas& m_trigThetas;
    ReferenceFrames1& m_referenceFrames1;
    ReferenceFrames2& m_referenceFrames2;
};

#endif /* MATERIALFRAMES_HH_ */
