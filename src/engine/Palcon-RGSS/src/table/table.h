/*
** table.h
**
** Table class for Palcon-RGSS
** Based on mkxp table.h
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>

class Table
{
public:
	Table(int x, int y = 1, int z = 1);
	~Table();

	void resize(int x, int y = 1, int z = 1);

	int xSize() const { return xsize; }
	int ySize() const { return ysize; }
	int zSize() const { return zsize; }

	short get(int x, int y = 0, int z = 0) const;
	void set(short value, int x, int y = 0, int z = 0);

private:
	int xsize, ysize, zsize;
	short* data;
	
	int calcIndex(int x, int y, int z) const;
};

#endif // TABLE_H
