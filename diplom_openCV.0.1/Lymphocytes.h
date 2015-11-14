#pragma once
#include "Cell.h"

namespace Lympho {
	class Lymphocytes : public Cell {
	private :
		int x_;
		int y_;
	public:
		Lymphocytes(const int& x, const int& y) {
			this->x_ = x;
			this->y_ = y;
		}
		~Lymphocytes() {}
		virtual inline int getX() { return x_; }
		virtual inline int getY() { return y_; }

	};
}