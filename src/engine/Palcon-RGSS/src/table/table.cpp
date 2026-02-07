/*
** table.cpp
**
** Table class implementation for Palcon-RGSS
** Based on mkxp table.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "table.h"
#include <cstring>
#include <algorithm>

Table::Table(int x, int y, int z)
	: xsize(std::max(1, x))
	, ysize(std::max(1, y))
	, zsize(std::max(1, z))
{
	int size = xsize * ysize * zsize;
	data = new short[size];
	memset(data, 0, size * sizeof(short));
}

Table::~Table()
{
	delete[] data;
}

void Table::resize(int x, int y, int z)
{
	int newXsize = std::max(1, x);
	int newYsize = std::max(1, y);
	int newZsize = std::max(1, z);
	
	int newSize = newXsize * newYsize * newZsize;
	short* newData = new short[newSize];
	memset(newData, 0, newSize * sizeof(short));
	
	// Copy old data
	int copyX = std::min(xsize, newXsize);
	int copyY = std::min(ysize, newYsize);
	int copyZ = std::min(zsize, newZsize);
	
	for (int zz = 0; zz < copyZ; zz++)
	{
		for (int yy = 0; yy < copyY; yy++)
		{
			for (int xx = 0; xx < copyX; xx++)
			{
				int oldIdx = calcIndex(xx, yy, zz);
				int newIdx = xx + yy * newXsize + zz * newXsize * newYsize;
				newData[newIdx] = data[oldIdx];
			}
		}
	}
	
	delete[] data;
	data = newData;
	xsize = newXsize;
	ysize = newYsize;
	zsize = newZsize;
}

short Table::get(int x, int y, int z) const
{
	if (x < 0 || x >= xsize || y < 0 || y >= ysize || z < 0 || z >= zsize)
		return 0;
	
	return data[calcIndex(x, y, z)];
}

void Table::set(short value, int x, int y, int z)
{
	if (x < 0 || x >= xsize || y < 0 || y >= ysize || z < 0 || z >= zsize)
		return;
	
	data[calcIndex(x, y, z)] = value;
}

int Table::calcIndex(int x, int y, int z) const
{
	return x + y * xsize + z * xsize * ysize;
}
