#include "GeometryUtility.h"
#define _USE_MATH_DEFINES
#include <math.h>

Vector3 GeometryUtility::Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector3 GeometryUtility::Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

Vector3 GeometryUtility::divide(const Vector3& v, float s)
{
	Vector3 result;

	result.x = v.x / s;
	result.y = v.y / s;
	result.z = v.z / s;

	return result;
}


Vector3 GeometryUtility::Multiply(const float a, const Vector3& vector)
{
	Vector3 result;

	result.x = vector.x * a;
	result.y = vector.y * a;
	result.z = vector.z * a;

	return result;
}

float GeometryUtility::length(Vector3 distance)
{
	return sqrtf(powf(distance.x, 2.0f) + powf(distance.y, 2.0f) + powf(distance.z, 2.0f));
}

Vector3 GeometryUtility::normalize(Vector3 distance)
{
	return { distance.x / length(distance),distance.y / length(distance),distance.z / length(distance) };
}

float GeometryUtility::Dot(Vector3 c, Vector3 d)
{
	return c.x * d.x + c.y * d.y + c.z * d.z;
}

Vector3 GeometryUtility::cross(const Vector3& u, const Vector3& v)
{
	return Vector3(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

Matrix4x4 GeometryUtility::Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 GeometryUtility::MakeRotateXMatrix(float radian)
{
	Matrix4x4 result;

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosf(radian),sinf(radian),0.0f,
		0.0f,-sinf(radian),cosf(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}

Matrix4x4 GeometryUtility::MakeRotateYMatrix(float radian)
{
	Matrix4x4 result;

	result = {
		cosf(radian),0.0f,-sinf(radian),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sinf(radian),0.0f,cosf(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}

Matrix4x4 GeometryUtility::MakeRotateZMatrix(float radian)
{
	Matrix4x4 result;

	result = {
		cosf(radian),sinf(radian),0.0f,0.0f,
		-sinf(radian),cosf(radian),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}

Matrix4x4 GeometryUtility::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 result;
	result = {
		scale.x * rotateXYZMatrix.m[0][0],scale.x * rotateXYZMatrix.m[0][1],scale.x * rotateXYZMatrix.m[0][2],0.0f,
		scale.y * rotateXYZMatrix.m[1][0],scale.y * rotateXYZMatrix.m[1][1],scale.y * rotateXYZMatrix.m[1][2],0.0f,
		scale.z * rotateXYZMatrix.m[2][0],scale.z * rotateXYZMatrix.m[2][1],scale.z * rotateXYZMatrix.m[2][2],0.0f,
		translate.x,translate.y,translate.z,1.0f
	};
	return result;
}

Matrix4x4 GeometryUtility::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result;

	result = {
		1 / aspectRatio * (1 / tanf(fovY / 2)),0.0f,0.0f,0.0f,
		0.0f,(1 / tanf(fovY / 2)),0.0f,0.0f,
		0.0f,0.0f,farClip / (farClip - nearClip),1.0f,
		0.0f,0.0f,-farClip * nearClip / (farClip - nearClip),0.0f
	};

	return result;
}

Matrix4x4 GeometryUtility::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result;
	result = {
	2 / (right - left),0.0f,0.0f,0.0f,
	0.0f,2 / (top - bottom),0.0f,0.0f,
	0.0f,0.0f,1 / (farClip - nearClip),0.0f,
	(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1.0f
	};
	return result;
}

Matrix4x4 GeometryUtility::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result;

	result = {
	width / 2,0.0f,0.0f,0.0f,
	0.0f,-(height / 2),0.0f,0.0f,
	0.0f,0.0f,maxDepth - minDepth,0.0f,
	left + (width / 2),top + (height / 2),minDepth,1.0f
	};


	return result;
}

Vector3 GeometryUtility::Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);

	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	result.x /= w;
	result.y /= w;
	result.z /= w;


	return result;
}

Matrix4x4 GeometryUtility::Inverse(const Matrix4x4& m)
{
	Matrix4x4 result;
	float A;
	A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[0][0] = 1 / A * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][1] = 1 / A * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][2] = 1 / A * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	result.m[0][3] = 1 / A * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	result.m[1][0] = 1 / A * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][1] = 1 / A * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][2] = 1 / A * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	result.m[1][3] = 1 / A * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

	result.m[2][0] = 1 / A * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][1] = 1 / A * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][2] = 1 / A * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	result.m[2][3] = 1 / A * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

	result.m[3][0] = 1 / A * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][1] = 1 / A * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][2] = 1 / A * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	result.m[3][3] = 1 / A * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
	return result;
}

Matrix4x4 GeometryUtility::MakeLookAtMatrix4x4(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = normalize(Subtract(target, eye));    // Forward
	Vector3 xaxis = normalize(cross(up, zaxis));         // Right
	Vector3 yaxis = cross(zaxis, xaxis);                 // Up

	Matrix4x4 viewMatrix = {
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		-Dot(xaxis, eye), -Dot(yaxis, eye), -Dot(zaxis, eye), 1.0f
	};

	return viewMatrix;
}

Vector3 GeometryUtility::SphericalToCartesian(float radius, float theta, float phi)
{
	Vector3 cartesian;
	cartesian.x = radius * sinf(theta) * cosf(phi);
	cartesian.y = radius * sinf(theta) * sinf(phi);
	cartesian.z = radius * cosf(theta);
	return cartesian;
}

Vector3 GeometryUtility::Project(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	float a = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
	float b = ((v2.x * v2.x) + (v2.y * v2.y) + (v2.z * v2.z));
	result.x = a / b * v2.x;
	result.y = a / b * v2.y;
	result.z = a / b * v2.z;

	return result;
}

Vector3 GeometryUtility::ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);

	project = Add(project, segment.origin);

	return project;
}

Vector3 GeometryUtility::Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

void GeometryUtility::DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void GeometryUtility::DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjection, const Matrix4x4 viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 16;
	const float kLonEvery = float(M_PI * 2) / kSubdivision;
	const float kLatEvery = float(M_PI) / kSubdivision;
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = float(-M_PI / 2.0f) + (kLatEvery * latIndex);

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			float lon = float(lonIndex * kLonEvery);

			Vector3 a, b, c;

			a = { (cosf(lat) * cosf(lon) * sphere.radius + sphere.center.x),(sinf(lat) * sphere.radius + sphere.center.y) ,(cosf(lat) * sinf(lon) * sphere.radius + sphere.center.z) };
			b = { (cosf(lat + kLatEvery) * cosf(lon) * sphere.radius + sphere.center.x) ,(sinf(lat + kLatEvery) * sphere.radius + sphere.center.y) ,(cosf(lat + kLatEvery) * sinf(lon) * sphere.radius + sphere.center.z) };
			c = { (cosf(lat) * cosf(lon + kLonEvery) * sphere.radius + sphere.center.x) ,(sinf(lat) * sphere.radius + sphere.center.y) ,(cosf(lat) * sinf(lon + kLonEvery) * sphere.radius + sphere.center.z) };

			Vector3 VecA = Transform(Transform(a, viewProjection), viewportMatrix);
			Vector3 VecB = Transform(Transform(b, viewProjection), viewportMatrix);
			Vector3 VecC = Transform(Transform(c, viewProjection), viewportMatrix);

			Novice::DrawLine(int(VecA.x), int(VecA.y), int(VecB.x), int(VecB.y), color);
			Novice::DrawLine(int(VecA.x), int(VecA.y), int(VecC.x), int(VecC.y), color);

		}
	}
}

void GeometryUtility::EnsureAABB(AABB& aabb)
{
	aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
	aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
	aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
	aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
	aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
	aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
}

void GeometryUtility::DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	Vector3 VecA = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 VecB = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 VecC = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	uint32_t color = 0xFFFFFFFF;

	Novice::DrawLine(int(VecA.x), int(VecA.y), int(VecB.x), int(VecB.y), color);
	Novice::DrawLine(int(VecB.x), int(VecB.y), int(VecC.x), int(VecC.y), color);
	Novice::DrawLine(int(VecC.x), int(VecC.y), int(VecA.x), int(VecA.y), color);
}

void GeometryUtility::DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f; 
	const uint32_t kSubdivision = 10;  
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); 
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
			

		Vector3 startPoint(kGridHalfWidth - (kGridEvery * xIndex), 0.0f, -kGridHalfWidth);
		Vector3 endPoint(kGridHalfWidth - (kGridEvery * xIndex), 0, kGridHalfWidth);

		Vector3 screenStartPoint = Transform(startPoint, viewProjectionMatrix);
		Vector3 screenEndPoint = Transform(endPoint, viewProjectionMatrix);

		Vector3 screenStartPoint1 = Transform(screenStartPoint, viewportMatrix);
		Vector3 screenEndPoint1 = Transform(screenEndPoint, viewportMatrix);

		Novice::DrawLine(int(screenStartPoint1.x), int(screenStartPoint1.y), int(screenEndPoint1.x), int(screenEndPoint1.y), 0xAAAAAAFF);


	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {

		Vector3 startPoint(-kGridHalfWidth, 0.0f, kGridHalfWidth - (kGridEvery * zIndex));
		Vector3 endPoint(kGridHalfWidth, 0, kGridHalfWidth - (kGridEvery * zIndex));

		Vector3 screenStartPoint = Transform(startPoint, viewProjectionMatrix);
		Vector3 screenEndPoint = Transform(endPoint, viewProjectionMatrix);

		Vector3 screenStartPoint1 = Transform(screenStartPoint, viewportMatrix);
		Vector3 screenEndPoint1 = Transform(screenEndPoint, viewportMatrix);

		Novice::DrawLine(int(screenStartPoint1.x), int(screenStartPoint1.y), int(screenEndPoint1.x), int(screenEndPoint1.y), 0xAAAAAAFF);

	}
}

void GeometryUtility::DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewMatrix, uint32_t color)
{
	Vector3 vertices[8] = {
	   {aabb.min.x, aabb.min.y, aabb.min.z},
	   {aabb.max.x, aabb.min.y, aabb.min.z},
	   {aabb.min.x, aabb.max.y, aabb.min.z},
	   {aabb.max.x, aabb.max.y, aabb.min.z},
	   {aabb.min.x, aabb.min.y, aabb.max.z},
	   {aabb.max.x, aabb.min.y, aabb.max.z},
	   {aabb.min.x, aabb.max.y, aabb.max.z},
	   {aabb.max.x, aabb.max.y, aabb.max.z}
	};
	for (int i = 0; i < 8; ++i) {
		vertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewMatrix);
	}
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[1].x), int(vertices[1].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[2].x), int(vertices[2].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[4].x), int(vertices[4].y), color);
	Novice::DrawLine(int(vertices[1].x), int(vertices[1].y), int(vertices[3].x), int(vertices[3].y), color);
	Novice::DrawLine(int(vertices[1].x), int(vertices[1].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[2].x), int(vertices[2].y), int(vertices[3].x), int(vertices[3].y), color);
	Novice::DrawLine(int(vertices[2].x), int(vertices[2].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[7].x), int(vertices[7].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[5].x), int(vertices[5].y), int(vertices[7].x), int(vertices[7].y), color);
	Novice::DrawLine(int(vertices[6].x), int(vertices[6].y), int(vertices[7].x), int(vertices[7].y), color);
}

Vector3 GeometryUtility::ClosestPointOnAABB(const Vector3& point, const AABB& aabb)
{
	Vector3 closestPoint;
	closestPoint.x = std::clamp(point.x, aabb.min.x, aabb.max.x);
	closestPoint.y = std::clamp(point.y, aabb.min.y, aabb.max.y);
	closestPoint.z = std::clamp(point.z, aabb.min.z, aabb.max.z);
	return closestPoint;
}

Vector3 GeometryUtility::Lerp(const Vector3& a, const Vector3& b, float t)
{
	Vector3 result;
	result.x = t * a.x + (1.0f - t) * b.x;
	result.y = t * a.y + (1.0f - t) * b.y;
	result.z = t * a.z + (1.0f - t) * b.z;
	return result;
}


