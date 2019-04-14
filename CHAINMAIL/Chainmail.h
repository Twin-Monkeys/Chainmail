#pragma once

#include "Constraint.h"
#include "Volume.h"
#include "Index.h"
#include "Plasticity.h"

class Chainmail 
{
public:
	/* constructor */
	explicit Chainmail(Volume& volume);

	/* member function */
	void setPivotIdx(const Index& idx);
	void setPivotIdx(const int width, const int height, const int depth = 0);
	Index getPivotIdx() const;

	void setElasticity(const float elasticity);

	void propagate(const float x, const float y, const float z);
	void relax();

	/* member variable */
	Constraint constraint;

private:
	/* member function */
	void _setPlasticity(const float elasticity);

	/* member variable */
	Index pivotIdx;
	Plasticity _plasticity;

	Volume& volume;
};