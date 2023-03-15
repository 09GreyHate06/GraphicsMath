#pragma once

#include "Types.h"

#define GM_PI 3.14159265359f

namespace GM
{


	template<typename T>
	T ToRadians(T d)
	{
		return d * GM_PI / 180.0f;
	}

	template<typename T>
	T ToDegrees(T r)
	{
		return r * 180.0f / GM_PI;
	}




	// =========================================== Vector =================================================
	
	// = sqrt(x^2 + y^2 ... n^2)
	float Vec4Magnitude(const Vector& v);

	// = sqrt(x^2 + y^2 ... n^2)
	float Vec3Magnitude(const Vector& v);

	// = sqrt(x^2 + y^2 ... n^2)
	float Vec2Magnitude(const Vector& v);

	// v / |v|
	Vector Vec4Normalized(const Vector& v);

	// v / |v|
	Vector Vec3Normalized(const Vector& v);

	// v / |v|
	Vector Vec2Normalized(const Vector& v);


	// v0.x^2 + v0.y^2 + v0.z^2 + v0.w^2 == ||v0|| * ||v1|| * cos(a)
	float Vec4Dot(const Vector& v0, const Vector& v1);

	// v0.x^2 + v0.y^2 + v0.z^2== ||v0|| * ||v1|| * cos(a)
	float Vec3Dot(const Vector& v0, const Vector& v1);

	// v0.x^2 + v0.y^2 == ||v0|| * ||v1|| * cos(a)
	float Vec2Dot(const Vector& v0, const Vector& v1);


	// Pseudodeterminant
	// | i     j      k    |
	// | v0.x  v0.y   v0.z | 
	// | v1.x  v1.y   v1.z |
	// = i(v0.y * v1.z - v0.z * v1.y) - j(v0.x * v1.z - v0.z * v1.x) + k(v0.x * v1.y - v0.y * v1.x)
	// ||result|| = ||v0|| * ||v1|| * sin(a)
	Vector Vec3Cross(const Vector& v0, const Vector& v1);

	// ((v0 . v1) / ||v1||^2) * v1
	Vector Vec3ProjectionOfV0OntoV1(const Vector& v0, const Vector& v1);

	Vector Vec4Transform(const Vector& v, const Matrix& m);

	Vector Vec3Transform(const Vector& v, const Matrix& m);

	Vector Vec2Transform(const Vector& v, const Matrix& m);

	/// <summary>
	/// Rotate a vector using quaternion
	/// </summary>
	/// <param name="v">vector to rotate</param>
	/// <param name="q">unit quaternion</param>
	/// <returns></returns>
	Vector Vec3Rotate(const Vector& v, const Quaternion& q);

	Vector Vec3Lerp(const Vector& v0, const Vector& v1, float t);














	// =========================================== Quaternion =============================================

	Quaternion QuatIdentity();

	/// <summary>
	/// lets say that q0 = (s0 + v0) and q1 = (s1 + v1) s0 and s1 is the scalar part(the w component) and v0 and v1 is the vector part(the x, y, z component) then 
	/// q0*q1 = s0 * s1 - Dot(v0, v1) + s0 * v1 + s1 * v0 + Cross(v0, v1) 
	/// = Vector((s0 * v1 + s1 * v0 + Cross(v0, v1)).xyz, s0 * s1 - Dot(v0, v1))
	/// </summary>
	/// <param name="q0"></param>
	/// <param name="q1"></param>
	/// <returns></returns>
	Quaternion QuatMultiply(const Quaternion& q0, const Quaternion& q1);

	Quaternion QuatNormalized(const Quaternion& q);

	/// <summary>
	/// = (-q.x, -q.y, - q.z, q.w)
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	Quaternion QuatConjugate(const Quaternion& q);

	/// <summary>
	/// Inverse(q) = Conjugate(q)/|q|^2
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	Quaternion QuatInverse(const Quaternion& q);

	/// <summary>
	/// Rotation about an arbitrary axis
	/// </summary>
	/// <param name="axis">normalized axis</param>
	/// <param name="angle">angle in radians</param>
	/// <returns></returns>
	Quaternion QuatRotationAxis(const Vector& axis, float angle);

	/// <summary>
	/// Euler angles rotation
	/// </summary>
	/// <param name="pitch">rotation about x axis</param>
	/// <param name="yaw">rotation about y axis</param>
	/// <param name="roll">rotation about z axis</param>
	/// <returns></returns>
	Quaternion QuatRotationRollPitchYaw(float pitch, float yaw, float roll);

	Quaternion QuatLerp(const Quaternion& q1, const Quaternion q2, float t);

	Quaternion QuatSlerp(const Quaternion& q1, const Quaternion& q2, float t);















	// =========================================== Matrix =================================================

	Matrix MatIdentity();

	Matrix MatTranspose(const Matrix& m);

	Matrix MatSub(const Matrix& m, int row, int column, int n);

	/// <summary>
	/// Calculate the determinant using elementary row operations
	/// </summary>
	/// <param name="m"> Linearly independent matrix</param>
	/// <param name="n"> Size of row and column</param>
	/// <returns>Determinant of Matrix m</returns>
	float MatDeterminant(const Matrix& m, int n);

	/// <param name="m">Matrix</param>
	/// <param name="n">row and column of matrix</param>
	/// <param name="i">row of number to be calculated</param>
	/// <param name="j">column of number to be calculated</param>
	/// <returns></returns>
	float MatCofactor(const Matrix& m, int n, int i, int j);

	/// <param name="m">matrix</param>
	/// <param name="n">row and column size</param>
	/// <returns></returns>
	Matrix MatCofactorMatrix(const Matrix& m, int n);

	/// <summary>
	/// Algorithm:
	/// Mathematics for 3D Game Programming  and Computer Graphics; Page 42: Algorthm 3.11 Gauss Jordan Elimination
	/// https://canvas.projekti.info/ebooks/Mathematics%20for%203D%20Game%20Programming%20and%20Computer%20Graphics,%20Third%20Edition.pdf
	/// </summary>
	/// <param name="m"> Invertible 4x4 Matrix</param>
	/// <param name="n"> size row and column</param>
	/// <param name="elementaryRows"> nullptr if you dont want to get 'm' elementary rows</param>
	/// <returns></returns>
	Matrix MatInverse(const Matrix& m, int n);







	// Matrix Transformation

	Matrix MatScale(const Vector& v);

	Matrix MatScale(float x, float y, float z);


	Matrix MatTranslate(const Vector& v);

	Matrix MatTranslate(float x, float y, float z);



	Matrix MatRotationX(float angle);

	Matrix MatRotationY(float angle);

	Matrix MatRotationRollPitchYaw(float pitch, float yaw, float roll);

	Matrix MatRotationZ(float angle);

	/// <summary>
	/// Rodrigues' Rotation Formula 
	/// P_rot = P * cos(angle) + Cross(axis, P) * sin(angle) + axis * Dot(axis, P) * (1 - cos(angle))
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="axis"></param>
	/// <returns></returns>
	Matrix MatRotationAxis(float angle, const Vector& axis);

	/// <summary>
	/// Matrix representation of quaternion rotation. Read the "Quaternion section of Mathematics for 3D Game Programming and Computer Graphics"
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	Matrix MatRotationQuaternion(const Quaternion& q);

	Matrix MatOrthographic(float viewWidth, float viewHeight, float nearZ, float farZ);

	Matrix MatOrthographicOffCenter(float left, float right, float bottom, float top, float nearZ, float farZ);


	Matrix MatPerspective(float viewWidth, float viewHeight, float nearZ, float farZ);

	Matrix MatPerspectiveOffCenter(float left, float right, float bottom, float top, float nearZ, float farZ);

	Matrix MatPerspectiveFov(float fovAngleY, float aspectRatio, float nearZ, float farZ);






	Vector LinePlaneIntersection(const Vector& point, const Vector& dir, const Vector& plane);
	Frustum FrustumFov(float fovAngleY, float aspectRatio, float nearZ, float farZ);
}