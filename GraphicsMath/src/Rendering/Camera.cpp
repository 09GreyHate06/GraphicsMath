#include "Camera.h"
#include <array>
#include "Core/GMException.h"

namespace GM
{
	Camera::Camera()
		: m_desc(), m_device()
	{
	}

	Camera::Camera(const CameraDesc& desc, ID3D11Device* device)
		: m_desc(desc), m_device(device)
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
		m_viewProjection = m_view * m_projection;

		UpdateFrustumBuffer();
	}

	void Camera::Set(const CameraDesc& desc, ID3D11Device* device)
	{
		m_desc = desc;
		m_device = device;
		UpdateViewMatrix();
		UpdateProjectionMatrix();
		m_viewProjection = m_view * m_projection;

		UpdateFrustumBuffer();
	}

	void Camera::SetFov(float fov)
	{
		m_desc.fov = fov;
		UpdateProjectionMatrix();
		m_viewProjection = m_view * m_projection;

		UpdateFrustumBuffer();
	}

	void Camera::SetSize(float width, float height)
	{
		m_desc.width = width;
		m_desc.height = height;
		UpdateProjectionMatrix();
		m_viewProjection = m_view * m_projection;

		UpdateFrustumBuffer();
	}

	void Camera::SetNearFarZ(float nearZ, float farZ)
	{
		m_desc.nearZ = nearZ;
		m_desc.farZ = farZ;
		UpdateProjectionMatrix();
		m_viewProjection = m_view * m_projection;

		UpdateFrustumBuffer();
	}

	void Camera::SetPosition(const Vector& pos)
	{
		m_desc.position = pos;
		UpdateViewMatrix();
		m_viewProjection = m_view * m_projection;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		SetPosition(Vector(x, y, z, 1.0f));
	}

	void Camera::SetRotation(const Vector& rot)
	{
		m_desc.rotation = rot;
		UpdateViewMatrix();
		m_viewProjection = m_view * m_projection;
	}

	void Camera::SetRotation(float pitch, float yaw, float roll)
	{
		SetRotation(Vector(pitch, yaw, roll, 0.0f));
	}

	const Matrix& Camera::GetView() const
	{
		return m_view;
	}

	const Matrix& Camera::GetProjection() const
	{
		return m_projection;
	}

	const Matrix& Camera::GetViewProjection() const
	{
		return m_viewProjection;
	}

	Quaternion Camera::GetOrientation() const
	{
		return QuatRotationRollPitchYaw(ToRadians(m_desc.rotation.x), ToRadians(m_desc.rotation.y), ToRadians(m_desc.rotation.z));
	}

	Vector Camera::GetRightDirection() const
	{
		return Vec3Rotate(Vector(1.0f, 0.0f, 0.0f, 0.0f), GetOrientation());
	}

	Vector Camera::GetUpDirection() const
	{
		return Vec3Rotate(Vector(0.0f, 1.0f, 0.0f, 0.0f), GetOrientation());
	}

	Vector Camera::GetForwardDirection() const
	{
		return Vec3Rotate(Vector(0.0f, 0.0f, 1.0f, 0.0f), GetOrientation());;
	}

	const CameraDesc& Camera::GetDesc() const
	{
		return m_desc;
	}

	ID3D11Buffer* Camera::GetFrustumVB() const
	{
		return m_frustumVB.Get();
	}

	ID3D11Buffer* Camera::GetFrustumIB() const
	{
		return m_frustumIB.Get();
	}

	void Camera::UpdateViewMatrix()
	{
		Vector rotQuat = QuatRotationRollPitchYaw(ToRadians(m_desc.rotation.x), ToRadians(m_desc.rotation.y), ToRadians(m_desc.rotation.z));
		m_view = MatInverse(MatRotationQuaternion(rotQuat) * MatTranslate(m_desc.position.x, m_desc.position.y, m_desc.position.z), 4);
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_projection = MatPerspectiveFov(ToRadians(m_desc.fov), m_desc.width / m_desc.height, m_desc.nearZ, m_desc.farZ);
	}

	void Camera::UpdateFrustumBuffer()
	{
		Frustum f = FrustumFov(ToRadians(m_desc.fov), m_desc.width / m_desc.height, m_desc.nearZ, m_desc.farZ);

		Vector topLeft = Vec3Cross(f.top, f.left);
		Vector topRight = Vec3Cross(f.right, f.top);
		Vector bottomRight = Vec3Cross(f.bottom, f.right);
		Vector bottomLeft = Vec3Cross(f.left, f.bottom);

		Vector trNearInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), topRight, f.nearZ);
		Vector tlNearInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), topLeft, f.nearZ);
		Vector blNearInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), bottomLeft, f.nearZ);
		Vector brNearInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), bottomRight, f.nearZ);

		Vector trFarInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), topRight, f.farZ);
		Vector tlFarInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), topLeft, f.farZ);
		Vector blFarInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), bottomLeft, f.farZ);
		Vector brFarInt = LinePlaneIntersection(Vector(0.0f, 0.0f, 0.0f, 0.0f), bottomRight, f.farZ);

		std::array<float, 72> vertices = {
			0.0f, 0.0f, 0.0f,
			tlFarInt.x, tlFarInt.y, tlFarInt.z,

			0.0f, 0.0f, 0.0f,
			trFarInt.x, trFarInt.y, trFarInt.z,

			0.0f, 0.0f, 0.0f,
			brFarInt.x, brFarInt.y, brFarInt.z,

			0.0f, 0.0f, 0.0f,
			blFarInt.x, blFarInt.y, blFarInt.z,



			trNearInt.x, trNearInt.y, trNearInt.z,
			tlNearInt.x, tlNearInt.y, tlNearInt.z,

			tlNearInt.x, tlNearInt.y, tlNearInt.z,
			blNearInt.x, blNearInt.y, blNearInt.z,

			blNearInt.x, blNearInt.y, blNearInt.z,
			brNearInt.x, brNearInt.y, brNearInt.z,

			brNearInt.x, brNearInt.y, brNearInt.z,
			trNearInt.x, trNearInt.y, trNearInt.z,



			trFarInt.x, trFarInt.y, trFarInt.z,
			tlFarInt.x, tlFarInt.y, tlFarInt.z,

			tlFarInt.x, tlFarInt.y, tlFarInt.z,
			blFarInt.x, blFarInt.y, blFarInt.z,

			blFarInt.x, blFarInt.y, blFarInt.z,
			brFarInt.x, brFarInt.y, brFarInt.z,

			brFarInt.x, brFarInt.y, brFarInt.z,
			trFarInt.x, trFarInt.y, trFarInt.z,
		};

		std::array<uint32_t, 72> indices;
		for (int i = 0; i < indices.size(); i++)
			indices[i] = i;

		{
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.ByteWidth = (uint32_t)vertices.size() * sizeof(float);
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 3 * sizeof(float);
			desc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA srd = {};
			srd.pSysMem = vertices.data();
			HR(m_device->CreateBuffer(&desc, &srd, &m_frustumVB));
		}

		{
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.ByteWidth = (uint32_t)indices.size() * sizeof(uint32_t);
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = sizeof(uint32_t);
			desc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA srd = {};
			srd.pSysMem = indices.data();
			HR(m_device->CreateBuffer(&desc, &srd, &m_frustumIB));
		}
	}
}