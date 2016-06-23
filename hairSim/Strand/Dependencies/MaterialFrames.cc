#include "MaterialFrames.hh"

template<>
const char* MaterialFrames<1>::name() const
{
    return "MaterialFrames1";
}

template<>
const char* MaterialFrames<2>::name() const
{
    return "MaterialFrames2";
}

// GCC did not like this... split into two
//template<int FrameN>
//void MaterialFrames<FrameN>::compute()

template<>
inline Vec3x MaterialFrames<1>::linearMix( const Vec3x& u, const Vec3x& v, Scalar s, Scalar c )
{
    return c * u + s * v;
}

template<>
inline Vec3x MaterialFrames<2>::linearMix( const Vec3x& u, const Vec3x& v, Scalar s, Scalar c )
{
    return -s * u + c * v;
}

template<>
void MaterialFrames<1>::compute()
{
    m_value.resize( m_size );
    const Vec3xArray& referenceFrames1 = m_referenceFrames1.get();
    const Vec3xArray& referenceFrames2 = m_referenceFrames2.get();
    const VecXx& sinThetas = m_trigThetas.getSines();
    const VecXx& cosThetas = m_trigThetas.getCosines();

    for ( IndexType vtx = m_firstValidIndex; vtx < size(); ++vtx )
    {
        const Vec3x& u = referenceFrames1[vtx];
        const Vec3x& v = referenceFrames2[vtx];
        const Scalar s = sinThetas[vtx];
        const Scalar c = cosThetas[vtx];

        m_value[vtx] = linearMix( u, v, s, c );
    }

    setDependentsDirty();
}

template<>
void MaterialFrames<2>::compute()
{
    m_value.resize( m_size );
    const Vec3xArray& referenceFrames1 = m_referenceFrames1.get();
    const Vec3xArray& referenceFrames2 = m_referenceFrames2.get();
    const VecXx& sinThetas = m_trigThetas.getSines();
    const VecXx& cosThetas = m_trigThetas.getCosines();

    for ( IndexType vtx = m_firstValidIndex; vtx < size(); ++vtx )
    {
        const Vec3x& u = referenceFrames1[vtx];
        const Vec3x& v = referenceFrames2[vtx];
        const Scalar s = sinThetas[vtx];
        const Scalar c = cosThetas[vtx];

        m_value[vtx] = linearMix( u, v, s, c );
    }

    setDependentsDirty();
}
