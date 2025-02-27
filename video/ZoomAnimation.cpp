#include "ZoomAnimation.h"
#include "Surface.h"
#include <memory>

ZoomAnimation::ZoomAnimation( const float minZoom /*= 1.0f */, const float m_maxZoom /* = 1.0f */ )  :
    m_minZoom( minZoom ),
    m_maxZoom( m_maxZoom ),
    m_curZoom( minZoom )
{
}

ZoomAnimation::~ZoomAnimation(void)
{
}


void ZoomAnimation::setZoom( const float minZoom /* = 1.0f */, const float maxZoom /* = 1.0f */)
{
    m_minZoom = minZoom;
    m_maxZoom = maxZoom;
}

void    ZoomAnimation::OnAnimate( const unsigned long delta )
{
    if( getPause() )
        return;

    Animation::OnAnimate( delta );

    // zoom계산.
    m_curZoom = getCurrentFrame() * (m_maxZoom - m_minZoom) / getMaxFrames() + m_minZoom;
}

void    ZoomAnimation::OnRender()
{
    Surface * pSurface = getSurface();
    
    Dim zoomedDim = pSurface->GetDim().zoom(m_curZoom);    
    Dim srcDim = getSurface()->GetDim();
    Coord srcCoord = getCoord();
    
    // 그림이 확대, 축소되면 이미지 위치로 확대 축소된 값에 따라 변한다.
    int x = srcCoord.getX() + (srcDim.getW() - zoomedDim.getW() ) / 2;
    int y = srcCoord.getY() + (srcDim.getH() - zoomedDim.getH() ) / 2;
    
    Coord dstCoord(x, y);    
    pSurface->BlitScaled(dstCoord, zoomedDim);
    
}
