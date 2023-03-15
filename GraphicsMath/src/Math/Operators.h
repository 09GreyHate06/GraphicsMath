#pragma once

#include "Types.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace GM
{
	// =========================================== Vector =================================================

	Vector operator+(const Vector& v0, const Vector& v1);

	Vector operator-(const Vector& v0, const Vector& v1);

	Vector operator*(const Vector& v, float s);

	Vector operator*(float s, const Vector& v);

	Vector operator/(const Vector& v, float s);


	Vector& operator+=(Vector& v0, const Vector& v1);

	Vector& operator-=(Vector& v0, const Vector& v1);

	Vector& operator*=(Vector& v0, float s);

	Vector& operator/=(Vector& v0, float s);


	Vector operator-(const Vector& v);


	std::ostream& operator<<(std::ostream& os, const Vector& v);







	// =========================================== Matrix ================================================= 


	Matrix operator+(const Matrix& m0, const Matrix& m1);

	Matrix operator-(const Matrix& m0, const Matrix& m1);

	Matrix operator-(const Matrix& m);

	Matrix operator*(const Matrix& m, float s);

	Matrix operator*(float s, const Matrix& m);

	Matrix operator*(const Matrix& m0, const Matrix& m1);

	std::ostream& operator<<(std::ostream& os, const Matrix& m);








	std::ostream& operator<<(std::ostream& os, const Frustum& f);
}