#include "Constant.h"

namespace Constant
{
	namespace Screen
	{
		const int WIDTH = 800;
		const int HEIGHT = 800;
		extern const TCHAR* const TITLE = TEXT("Chainmail");
	}

	namespace Volume
	{
		// D3D ÁÂÇ¥°è: D(y), H(z), W(x)
		const int WIDTH = 6;
		const int HEIGHT = 6;
		const int DEPTH = 6;
	}

	namespace Camera
	{
		const D3DXVECTOR3 EYE = { 100.f, 100.f, -300.f };
		const D3DXVECTOR3 AT = { 0.f, 0.f, 0.f };
		const D3DXVECTOR3 UPVECTOR = { 0.f, 1.f, 0.f };
	}

	namespace Chainmail
	{
		const Index PIVOT_IDX = { 3, 0, 3 };
		const Rangef DX = { 0.7f, 1.3f };
		const Rangef DY = { 0.7f, 1.3f };
		const Rangef DZ = { 0.7f, 1.3f };
		const D3DVECTOR SHEAR = { 0.3f, 0.3f, 0.3f };
		const float ELASTICITY = 0.7f;
	}

	namespace Geometry
	{
		const int VERTEX_SIZE = 
			(Volume::DEPTH * Volume::HEIGHT * Volume::WIDTH);
		const int INDEX_SIZE =
			(((Volume::WIDTH - 1) * Volume::HEIGHT * Volume::DEPTH) +
			(Volume::WIDTH * (Volume::HEIGHT - 1) * Volume::DEPTH) +
			(Volume::WIDTH * Volume::HEIGHT * (Volume::DEPTH - 1)));
	}

	namespace Light
	{
		const D3DCOLORVALUE DIFFUSE = { 1.f, 1.f, 1.f, 1.f };
		const D3DCOLORVALUE SPECULAR = { 1.f, 1.f, 1.f, 1.f };
		const D3DVECTOR DIRECTION = { 10.f, -10.f, 10.f };
	}

	namespace Material
	{
		namespace Pivot
		{
			const D3DCOLORVALUE DIFFUSE = { 1.f, 0.1f, 0.1f, 1.f };
			const D3DCOLORVALUE SPECULAR = { 1.f, 1.f, 1.f, 1.f };
		}

		namespace Default
		{
			const D3DCOLORVALUE DIFFUSE = { 0.f, 0.4f, 0.f, 1.f };
			const D3DCOLORVALUE SPECULAR = { 1.f, 1.f, 1.f, 1.f };
		}
	}
}