// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MathLocal.h"
#include "Collision.h"
#include <iostream>

int main(int argc, char** argv)
{
	const float PiOver6 = Math::Pi / 6.0f;
	OBB o1 = OBB(Vector3::UnitX, Quaternion(Vector3::UnitZ, PiOver6), Vector3(10.0f, 10.0f, 10.0f));
	OBB o2 = OBB(Vector3::UnitX, Quaternion(Vector3::UnitZ, PiOver6), Vector3(20.0f, 20.0f, 20.0f));
	OBB o3 = OBB(Vector3(100.0f, 100.0f, 100.0f), Quaternion::Identity, Vector3(10.0f, 10.0f, 10.0f));
	std::cout << "\no1xo2\n";
	bool b12 = Intersect(o1, o2);
	std::cout << "\no1xo2 " << (b12 ? "" : "don't") << " intersect.\n";
	std::cout << "\no2xo3\n";
	bool b23 = Intersect(o2, o3);
	std::cout << "\no2xo3 " << (b23 ? "" : "don't") << " intersect.\n";
	std::cout << "\no3xo1\n";
	bool b31 = Intersect(o3, o1);
	std::cout << "\no3xo1 " << (b31 ? "" : "don't") << " intersect.\n";
	return 0;
}
