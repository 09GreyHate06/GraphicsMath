#include "Operators.h"

namespace GM
{
	float Vec4Dot(const Vector&, const Vector&);

	// =========================================== Vector =================================================

	Vector operator+(const Vector& v0, const Vector& v1)
	{
		return Vector(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w);
	}

	Vector operator-(const Vector& v0, const Vector& v1)
	{
		return Vector(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w);
	}

	Vector operator*(const Vector& v, float s)
	{
		return Vector(v.x * s, v.y * s, v.z * s, v.w * s);
	}

	Vector operator*(float s, const Vector& v)
	{
		return Vector(v.x * s, v.y * s, v.z * s, v.w * s);
	}

	Vector operator/(const Vector& v, float s)
	{
		return Vector(v.x / s, v.y / s, v.z / s, v.w / s);
	}


	Vector& operator+=(Vector& v0, const Vector& v1)
	{
		v0.x += v1.x;
		v0.y += v1.y;
		v0.z += v1.z;
		v0.w += v1.w;

		return v0;
	}

	Vector& operator-=(Vector& v0, const Vector& v1)
	{
		v0.x -= v1.x;
		v0.y -= v1.y;
		v0.z -= v1.z;
		v0.w -= v1.w;

		return v0;
	}

	Vector& operator*=(Vector& v0, float s)
	{
		v0.x *= s;
		v0.y *= s;
		v0.z *= s;
		v0.w *= s;

		return v0;
	}

	Vector& operator/=(Vector& v0, float s)
	{
		v0.x /= s;
		v0.y /= s;
		v0.z /= s;
		v0.w /= s;

		return v0;
	}


	Vector operator-(const Vector& v)
	{
		return Vector(-v.x, -v.y, -v.z, -v.w);
	}


	std::ostream& operator<<(std::ostream& os, const Vector& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return os;
	}







	// =========================================== Matrix ================================================= 


	Matrix operator+(const Matrix& m0, const Matrix& m1)
	{
		return Matrix(m0[0] + m1[0], m0[1] + m1[1], m0[2] + m1[2], m0[3] + m1[3]);
	}

	Matrix operator-(const Matrix& m0, const Matrix& m1)
	{
		return Matrix(m0[0] - m1[0], m0[1] - m1[1], m0[2] - m1[2], m0[3] - m1[3]);
	}

	Matrix operator-(const Matrix& m)
	{
		return Matrix(-m[0], -m[1], -m[2], -m[3]);
	}

	Matrix operator*(const Matrix& m, float s)
	{
		return Matrix(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
	}

	Matrix operator*(float s, const Matrix& m)
	{
		return Matrix(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
	}

	Matrix operator*(const Matrix& m0, const Matrix& m1)
	{
		Matrix result;

		// algorithm
		//for (int i = 0; i < 4; i++)
		//{
		//	for (int j = 0; j < 4; j++)
		//	{
		//		float ij = 0.0f;
		//		for (int k = 0; k < 4; k++)
		//		{
		//			ij += m0[i][k] * m1[k][j];
		//		}
		//
		//		ab[i][j] = ij;
		//	}
		//}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result[i][j] = Vec4Dot(m0[i], m1.GetColumnVector(j));
			}
		}

		return result;
	}

	std::ostream& operator<<(std::ostream& os, const Matrix& m)
	{
		size_t maxLenPerCol[4] = { 0, 0, 0, 0 };

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				size_t curLength = std::to_string(m[x][y]).size();
				if (curLength > maxLenPerCol[y])
					maxLenPerCol[y] = curLength;
			}
		}

		for (int x = 0; x < 4; x++)
		{
			os << "|";
			for (int y = 0; y < 4; y++)
			{
				os << std::setw(maxLenPerCol[y]) << m[x][y];
				if (y == 3)
					os << "|";
			}

			os << '\n';
		}

		//os << "****** Matrix ******\n";
		//os << m[0] << '\n' << m[1] << '\n' << m[2] << '\n' << m[3] << '\n';
		//os << "********************";

		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Frustum& f)
	{
		os  << "Near   : " << f.nearZ << '\n'
			<< "Far    : " << f.farZ << '\n'
			<< "Left   : " << f.left << '\n'
			<< "Right  : " << f.right << '\n'
			<< "Bottom : " << f.bottom << '\n'
			<< "Top    : " << f.top << '\n';

		return os;
	}
}