//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2008 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#ifndef _GLTexture_H_
#define _GLTexture_H_

#include "../api.h"
#include "Bitmap.h"
#include "OGLHelper.h"

#include "../base/Point.h"

#include <boost/shared_ptr.hpp>

namespace avg {

class AVG_API GLTexture {

public:
    GLTexture(const IntPoint& size, PixelFormat pf, bool bMipmap=false,
            unsigned wrapSMode=GL_CLAMP_TO_EDGE, unsigned wrapTMode=GL_CLAMP_TO_EDGE);
    virtual ~GLTexture();

    void activate(int textureUnit=GL_TEXTURE0);
    void generateMipmaps();

    const IntPoint& getSize() const;
    PixelFormat getPF() const;
    unsigned getID() const;

    static bool isFloatFormatSupported();
    static int getGLFormat(PixelFormat pf);
    static int getGLType(PixelFormat pf);
    int getGLInternalFormat() const;

private:
    IntPoint m_Size;
    PixelFormat m_pf;

    unsigned m_TexID;
};

typedef boost::shared_ptr<GLTexture> GLTexturePtr;

}

#endif
 


