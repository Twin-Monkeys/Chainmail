#include "Chainmail.h"
#include "MathUtility.h"
#include <queue>

using namespace std;

constexpr float SENSITIVITY = 0.0158f;

/* constructor */
Chainmail::Chainmail(Volume& volume) : volume(volume) {}

/* member function */
void Chainmail::setPivotIdx(const Index& idx) 
{
	setPivotIdx(idx.width, idx.height, idx.depth);
}

void Chainmail::setPivotIdx(const int width, const int height, const int depth) 
{
	const int MAX_WIDTH = (volume.getWidth() - 1);
	const int MAX_HEIGHT = (volume.getHeight() - 1);
	const int MAX_DEPTH = (volume.getDepth() - 1);

	if (width < 0)
		pivotIdx.width = 0;
	else if (width > MAX_WIDTH)
		pivotIdx.width = MAX_WIDTH;
	else
		pivotIdx.width = width;

	if (height < 0)
		pivotIdx.height = 0;
	else if (height > MAX_HEIGHT)
		pivotIdx.height = MAX_HEIGHT;
	else
		pivotIdx.height = height;

	if (depth < 0)
		pivotIdx.depth = 0;
	else if (depth > MAX_DEPTH)
		pivotIdx.depth = MAX_DEPTH;
	else
		pivotIdx.depth = depth;
}

Index Chainmail::getPivotIdx() const 
{
	return pivotIdx; 
}

void Chainmail::setElasticity(const float elasticity)
{
	float _elasticity = elasticity;

	if (elasticity < 0.f)
		_elasticity = 0.f;
	else if (elasticity > 1.f)
		_elasticity = 1.f;

	_setPlasticity(_elasticity);
}

void Chainmail::_setPlasticity(const float elasticity) 
{
	_plasticity.x = MathUtility::min(constraint.shearX, ((constraint.dX.second - constraint.dX.first) * 0.5f));
	_plasticity.y = MathUtility::min(constraint.shearY, ((constraint.dY.second - constraint.dY.first) * 0.5f));
	_plasticity.z = MathUtility::min(constraint.shearZ, ((constraint.dZ.second - constraint.dZ.first) * 0.5f));

	_plasticity *= (1.f - powf(elasticity, 0.16f));
}

void Chainmail::propagate(const float x, const float y, const float z) 
{
	Voxel*** const pVol = volume.getPtr3D();

	Voxel* pivot = &pVol[pivotIdx.depth][pivotIdx.height][pivotIdx.width];
	pivot->position.x += (x * SENSITIVITY);
	pivot->position.y += (y * SENSITIVITY);
	pivot->position.z += (z * SENSITIVITY);

	Voxel* sponsor	= nullptr;

	Voxel* right	= nullptr;
	Voxel* left		= nullptr;
	Voxel* top		= nullptr;
	Voxel* bottom	= nullptr;
	Voxel* front	= nullptr;
	Voxel* back		= nullptr;

	const float MIN_DX  = constraint.dX.first;
	const float MAX_DX  = constraint.dX.second;
	const float MIN_DY  = constraint.dY.first;
	const float MAX_DY  = constraint.dY.second;
	const float MIN_DZ  = constraint.dZ.first;
	const float MAX_DZ  = constraint.dZ.second;
	const float SHEAR_X = constraint.shearX;
	const float SHEAR_Y = constraint.shearY;
	const float SHEAR_Z	= constraint.shearZ;

	float dX = 0.f;
	float dY = 0.f;
	float dZ = 0.f;

	bool isMoved = false;

	queue<Voxel*> sponsorQueue;
	sponsorQueue.emplace(pivot);

	//////////////////////////////////////////////////////////

	while (!sponsorQueue.empty()) 
	{
		isMoved = false;

		sponsor = sponsorQueue.front();
		sponsorQueue.pop();

		right	= sponsor->right;
		left	= sponsor->left;
		top		= sponsor->top;
		bottom	= sponsor->bottom;
		front	= sponsor->front;
		back	= sponsor->back;

		//////////////////////////////////////////////////////////

		if (right) 
		{
			if (right == pivot) 
			{
				sponsorQueue.emplace(right);
				continue;
			}

			dX = (right->position.x - sponsor->position.x);
			dY = (right->position.y - sponsor->position.y);
			dZ = (right->position.z - sponsor->position.z);
			
			if (MathUtility::greater(dX, MIN_DX)) 
			{
				right->position.x = (sponsor->position.x + MIN_DX);
				isMoved = true;
			}
			else if (MathUtility::greater(MAX_DX, dX)) 
			{
				right->position.x = (sponsor->position.x + MAX_DX);
				isMoved = true;
			}

			if (MathUtility::greater(dY, -SHEAR_Y))
			{
				right->position.y = (sponsor->position.y - SHEAR_Y);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Y, dY))
			{
				right->position.y = (sponsor->position.y + SHEAR_Y);
				isMoved = true;
			}

			if (MathUtility::greater(dZ, -SHEAR_Z))
			{
				right->position.z = (sponsor->position.z - SHEAR_Z);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Z, dZ))
			{
				right->position.z = (sponsor->position.z + SHEAR_Z);
				isMoved = true;
			}

			if (isMoved)
				sponsorQueue.emplace(right);
		}

		//////////////////////////////////////////////////////////

		if (left) 
		{
			if (left == pivot)
			{
				sponsorQueue.emplace(left);
				continue;
			}

			dX = (sponsor->position.x - left->position.x);
			dY = (left->position.y - sponsor->position.y);
			dZ = (left->position.z - sponsor->position.z);

			if (MathUtility::greater(dX, MIN_DX))
			{
				left->position.x = (sponsor->position.x - MIN_DX);
				isMoved = true;
			}
			else if (MathUtility::greater(MAX_DX, dX))
			{
				left->position.x = (sponsor->position.x - MAX_DX);
				isMoved = true;
			}

			if (MathUtility::greater(dY, -SHEAR_Y))
			{
				left->position.y = (sponsor->position.y - SHEAR_Y);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Y, dY))
			{
				left->position.y = (sponsor->position.y + SHEAR_Y);
				isMoved = true;
			}

			if (MathUtility::greater(dZ, -SHEAR_Z))
			{
				left->position.z = (sponsor->position.z - SHEAR_Z);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Z, dZ))
			{
				left->position.z = (sponsor->position.z + SHEAR_Z);
				isMoved = true;
			}

			if (isMoved)
				sponsorQueue.emplace(left);
		}

		//////////////////////////////////////////////////////////

		if (top) 
		{
			if (top == pivot)
			{
				sponsorQueue.emplace(top);
				continue;
			}

			dX = (top->position.x - sponsor->position.x);
			dY = (top->position.y - sponsor->position.y);
			dZ = (top->position.z - sponsor->position.z);

			if (MathUtility::greater(dX, -SHEAR_X))
			{
				top->position.x = (sponsor->position.x - SHEAR_X);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_X, dX))
			{
				top->position.x = (sponsor->position.x + SHEAR_X);
				isMoved = true;
			}

			if (MathUtility::greater(dY, MIN_DY))
			{
				top->position.y = (sponsor->position.y + MIN_DY);
				isMoved = true;
			}
			else if (MathUtility::greater(MAX_DY, dY))
			{
				top->position.y = (sponsor->position.y + MAX_DY);
				isMoved = true;
			}

			if (MathUtility::greater(dZ, -SHEAR_Z))
			{
				top->position.z = (sponsor->position.z - SHEAR_Z);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Z, dZ))
			{
				top->position.z = (sponsor->position.z + SHEAR_Z);
				isMoved = true;
			}

			if (isMoved)
				sponsorQueue.emplace(top);
		}

		//////////////////////////////////////////////////////////

		if (bottom) 
		{
			if (bottom == pivot)
			{
				sponsorQueue.emplace(bottom);
				continue;
			}

			dX = (bottom->position.x - sponsor->position.x);
			dY = (sponsor->position.y - bottom->position.y);
			dZ = (bottom->position.z - sponsor->position.z);

			if (MathUtility::greater(dX, -SHEAR_X))
			{
				bottom->position.x = (sponsor->position.x - SHEAR_X);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_X, dX))
			{
				bottom->position.x = (sponsor->position.x + SHEAR_X);
				isMoved = true;
			}

			if (MathUtility::greater(dY, MIN_DY))
			{
				bottom->position.y = (sponsor->position.y - MIN_DY);
				isMoved = true;
			}
			else if (MathUtility::greater(MAX_DY, dY))
			{
				bottom->position.y = (sponsor->position.y - MAX_DY);
				isMoved = true;
			}

			if (MathUtility::greater(dZ, -SHEAR_Z))
			{
				bottom->position.z = (sponsor->position.z - SHEAR_Z);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Z, dZ))
			{
				bottom->position.z = (sponsor->position.z + SHEAR_Z);
				isMoved = true;
			}

			if (isMoved)
				sponsorQueue.emplace(bottom);
		}

		//////////////////////////////////////////////////////////

		if (front) 
		{
			if (front == pivot)
			{
				sponsorQueue.emplace(front);
				continue;
			}

			dX = (front->position.x - sponsor->position.x);
			dY = (front->position.y - sponsor->position.y);
			dZ = (front->position.z - sponsor->position.z);

			if (MathUtility::greater(dX, -SHEAR_X))
			{
				front->position.x = (sponsor->position.x - SHEAR_X);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_X, dX))
			{
				front->position.x = (sponsor->position.x + SHEAR_X);
				isMoved = true;
			}

			if (MathUtility::greater(dY, -SHEAR_Y))
			{
				front->position.y = (sponsor->position.y - SHEAR_Y);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Y, dY))
			{
				front->position.y = (sponsor->position.y + SHEAR_Y);
				isMoved = true;
			}

			if (MathUtility::greater(dZ, MIN_DZ))
			{
				front->position.z = (sponsor->position.z + MIN_DZ);
				isMoved = true;
			}
			else if (MathUtility::greater(MAX_DZ, dZ))
			{
				front->position.z = (sponsor->position.z + MAX_DZ);
				isMoved = true;
			}
			
			if (isMoved)
				sponsorQueue.emplace(front);
		}

		//////////////////////////////////////////////////////////

		if (back) 
		{
			if (back == pivot)
			{
				sponsorQueue.emplace(back);
				continue;
			}

			dX = (back->position.x - sponsor->position.x);
			dY = (back->position.y - sponsor->position.y);
			dZ = (sponsor->position.z - back->position.z);

			if (MathUtility::greater(dX, -SHEAR_X))
			{
				back->position.x = (sponsor->position.x - SHEAR_X);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_X, dX))
			{
				back->position.x = (sponsor->position.x + SHEAR_X);
				isMoved = true;
			}

			if (MathUtility::greater(dY, -SHEAR_Y))
			{
				back->position.y = (sponsor->position.y - SHEAR_Y);
				isMoved = true;
			}
			else if (MathUtility::greater(SHEAR_Y, dY))
			{
				back->position.y = (sponsor->position.y + SHEAR_Y);
				isMoved = true;
			}

			if (MathUtility::greater(dZ, MIN_DZ))
			{
				back->position.z = (sponsor->position.z - MIN_DZ);
				isMoved = true;
			}
			else if (MathUtility::greater(MAX_DZ, dZ))
			{
				back->position.z = (sponsor->position.z - MAX_DZ);
				isMoved = true;
			}

			if (isMoved)
				sponsorQueue.emplace(back);
		}
	}

	//////////////////////////////////////////////////////////

	relax();
}

void Chainmail::relax() 
{
	Voxel*** const pVol = volume.getPtr3D();

	Voxel* pivot = &(pVol[pivotIdx.depth][pivotIdx.height][pivotIdx.width]);

	const int DEPTH		= volume.getDepth();
	const int HEIGHT	= volume.getHeight();
	const int WIDTH		= volume.getWidth();

	//////////////////////////////////////////////////////////

	bool isMoved = true;
	int cnt = 0;

	Point3f optPos;
	Vector3f dist;

	float optX = 0.f;
	float optY = 0.f;
	float optZ = 0.f;

	while (isMoved)
	{
		isMoved = false;

		for (int d = 0; d < DEPTH; ++d)
			for (int h = 0; h < HEIGHT; ++h)
				for (int w = 0; w < WIDTH; ++w)
				{
					cnt = 0;

					optX = 0.f;
					optY = 0.f;
					optZ = 0.f;

					Voxel& curVoxel = pVol[d][h][w];

					if (pivot == &curVoxel)
						continue;

					if (curVoxel.right)
					{
						optX += ((curVoxel.right->position.x) - 1.f);
						optY += (curVoxel.right->position.y);
						optZ += (curVoxel.right->position.z);
						++cnt;
					}

					if (curVoxel.left)
					{
						optX += ((curVoxel.left->position.x) + 1.f);
						optY += (curVoxel.left->position.y);
						optZ += (curVoxel.left->position.z);
						++cnt;
					}

					if (curVoxel.top)
					{
						optX += (curVoxel.top->position.x);
						optY += ((curVoxel.top->position.y) - 1.f);
						optZ += (curVoxel.top->position.z);
						++cnt;
					}

					if (curVoxel.bottom)
					{
						optX += (curVoxel.bottom->position.x);
						optY += ((curVoxel.bottom->position.y) + 1.f);
						optZ += (curVoxel.bottom->position.z);
						++cnt;
					}

					if (curVoxel.front)
					{
						optX += (curVoxel.front->position.x);
						optY += (curVoxel.front->position.y);
						optZ += ((curVoxel.front->position.z) - 1.f);
						++cnt;
					}

					if (curVoxel.back)
					{
						optX += (curVoxel.back->position.x);
						optY += (curVoxel.back->position.y);
						optZ += ((curVoxel.back->position.z) + 1.f);
						++cnt;
					}

					//////////////////////////////////////////////////////////

					if (!cnt)
						continue;

					//////////////////////////////////////////////////////////

					optPos.set(optX, optY, optZ);
					optPos /= static_cast<float>(cnt);

					dist = (optPos - curVoxel.position);

					if (MathUtility::greater(dist.x, -_plasticity.x))
					{
						curVoxel.position.x -= _plasticity.x;
						isMoved = true;
					}
					else if (MathUtility::greater(_plasticity.x, dist.x))
					{
						curVoxel.position.x += _plasticity.x;
						isMoved = true;
					}

					if (MathUtility::greater(dist.y, -_plasticity.y))
					{
						curVoxel.position.y -= _plasticity.y;
						isMoved = true;
					}
					else if (MathUtility::greater(_plasticity.y, dist.y))
					{
						curVoxel.position.y += _plasticity.y;
						isMoved = true;
					}

					if (MathUtility::greater(dist.z, -_plasticity.z))
					{
						curVoxel.position.z -= _plasticity.z;
						isMoved = true;
					}
					else if (MathUtility::greater(_plasticity.z, dist.z))
					{
						curVoxel.position.z += _plasticity.z;
						isMoved = true;
					}
				}
	}
}