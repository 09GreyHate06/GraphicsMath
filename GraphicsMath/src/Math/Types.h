#pragma once

namespace GM
{
	struct Vector
	{
		union 
		{
			struct
			{
				float x, y, z, w;
			};

			float f[4];
		};

		Vector(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{
		}

		Vector()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}

		float& operator[](int i)
		{
			return f[i];
		}

		float operator[](int i) const
		{
			return f[i];
		}
	};

	typedef Vector Quaternion;

	struct Matrix
	{
		union
		{
			float f[4][4];
			Vector v[4];
		};

		Matrix(
			float _00, float _01, float _02, float _03,
			float _10, float _11, float _12, float _13,
			float _20, float _21, float _22, float _23,
			float _30, float _31, float _32, float _33)
			:
			v{
				Vector(_00, _01, _02, _03),
				Vector(_10, _11, _12, _13),
				Vector(_20, _21, _22, _23),
				Vector(_30, _31, _32, _33)
		}
		{
		}

		constexpr Matrix(
			const Vector& _0,
			const Vector& _1,
			const Vector& _2,
			const Vector& _3)
			: v{ _0, _1, _2, _3 }
		{
		}

		Matrix() : v() {}

		// creates a vector from matrix' column
		Vector GetColumnVector(int column) const
		{
			return Vector
			(
				f[0][column],
				f[1][column],
				f[2][column],
				f[3][column]
			);
		}

		Vector& operator[](int i)
		{
			return v[i];
		}

		Vector operator[](int i) const
		{
			return v[i];
		}
	};
}