/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "CmD3D9TextureManager.h"
#include "CmD3D9Texture.h"
#include "CmD3D9RenderTexture.h"
#include "CmException.h"
#include "CmD3D9Mappings.h"
#include "CmD3D9RenderSystem.h"
#include "CmD3D9MultiRenderTexture.h"

namespace CamelotFramework 
{
	D3D9TextureManager::D3D9TextureManager()
		:TextureManager()
	{		
	}
	
	D3D9TextureManager::~D3D9TextureManager()
	{

	}

    TexturePtr D3D9TextureManager::createTextureImpl()
    {
		D3D9Texture* tex = CM_NEW(D3D9Texture, PoolAlloc) D3D9Texture();

		return TexturePtr(tex, &CoreObject::_deleteDelayed<D3D9Texture, PoolAlloc>);
    }

	RenderTexturePtr D3D9TextureManager::createRenderTextureImpl()
	{
		D3D9RenderTexture* tex = CM_NEW(D3D9RenderTexture, PoolAlloc) D3D9RenderTexture();

		return RenderTexturePtr(tex, &CoreObject::_deleteDelayed<D3D9RenderTexture, PoolAlloc>);
	}

	MultiRenderTexturePtr D3D9TextureManager::createMultiRenderTextureImpl()
	{
		D3D9MultiRenderTexture* tex = CM_NEW(D3D9MultiRenderTexture, PoolAlloc) D3D9MultiRenderTexture();

		return MultiRenderTexturePtr(tex, &CoreObject::_deleteDelayed<D3D9MultiRenderTexture, PoolAlloc>);
	}

	PixelFormat D3D9TextureManager::getNativeFormat(TextureType ttype, PixelFormat format, int usage)
	{
		// Basic filtering
		D3DFORMAT d3dPF = D3D9Mappings::_getPF(D3D9Mappings::_getClosestSupportedPF(format));

		// Calculate usage
		DWORD d3dusage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;
		if (usage & TU_RENDERTARGET) 
		{
			d3dusage |= D3DUSAGE_RENDERTARGET;
			pool = D3DPOOL_DEFAULT; 
		}
		if(usage & TU_DEPTHSTENCIL)
		{
			d3dusage |= D3DUSAGE_DEPTHSTENCIL;
			pool = D3DPOOL_DEFAULT;
		}
		if (usage & TU_DYNAMIC)
		{
			d3dusage |= D3DUSAGE_DYNAMIC;
			pool = D3DPOOL_DEFAULT;
		}

		IDirect3DDevice9* pCurDevice = D3D9RenderSystem::getActiveD3D9Device();

		// Use D3DX to adjust pixel format
		switch(ttype)
		{
		case TEX_TYPE_1D:
		case TEX_TYPE_2D:
			D3DXCheckTextureRequirements(pCurDevice, NULL, NULL, NULL, d3dusage, &d3dPF, pool);
			break;
		case TEX_TYPE_3D:
			D3DXCheckVolumeTextureRequirements(pCurDevice, NULL, NULL, NULL, NULL, d3dusage, &d3dPF, pool);
			break;
		case TEX_TYPE_CUBE_MAP:
			D3DXCheckCubeTextureRequirements(pCurDevice, NULL, NULL, d3dusage, &d3dPF, pool);
			break;
		};

		return D3D9Mappings::_getPF(d3dPF);
	}
}
