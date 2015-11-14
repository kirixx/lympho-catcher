#pragma once

class Cell {
public:
	virtual int getX() = 0;
	virtual int getY() = 0;
	virtual ~Cell() = 0;
};