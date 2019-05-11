#pragma once

#include <d3dx9.h>
#include <tchar.h>
#include "Range.h"
#include "Index.h"

namespace Constant 
{
	namespace Screen 
	{
		extern const int WIDTH;
		extern const int HEIGHT;
		extern const TCHAR* const TITLE;
	}

	namespace Volume 
	{
		extern const int WIDTH;
		extern const int HEIGHT;
		extern const int DEPTH;
	}

	namespace Camera 
	{
		extern const D3DXVECTOR3 EYE;
		extern const D3DXVECTOR3 AT;
		extern const D3DXVECTOR3 UPVECTOR;
	}

	namespace Chainmail
	{
		extern const Index PIVOT_IDX;
		extern const Rangef DX;
		extern const Rangef DY;
		extern const Rangef DZ;
		extern const D3DVECTOR SHEAR;
		extern const float ELASTICITY;
	}

	namespace Geometry 
	{
		extern const int VERTEX_SIZE;
		extern const int INDEX_SIZE;
	}

	namespace Light 
	{
		extern const D3DCOLORVALUE DIFFUSE;
		extern const D3DCOLORVALUE SPECULAR;
		extern const D3DVECTOR DIRECTION;
	}

	namespace Material
	{
		namespace Pivot 
		{
			extern const D3DCOLORVALUE DIFFUSE;
			extern const D3DCOLORVALUE SPECULAR;
		}

		namespace Default 
		{
			extern const D3DCOLORVALUE DIFFUSE;
			extern const D3DCOLORVALUE SPECULAR;
		}
	}
}