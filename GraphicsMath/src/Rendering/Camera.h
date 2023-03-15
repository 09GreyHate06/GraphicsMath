#pragma once
#include "Math/GMMath.h"
#include "Core/NativeWindow.h"
#include <d3d11.h>
#include <wrl.h>

namespace GM
{
	struct CameraDesc
	{
		float fov;
		float width;
		float height;
		float nearZ;
		float farZ;

		Vector position;
		Vector rotation;
	};

	class Camera
	{
	public:
		Camera();
		Camera(const CameraDesc& desc, ID3D11Device* device);

		void Set(const CameraDesc& desc, ID3D11Device* device);
		void SetFov(float fov);
		void SetSize(float width, float height);
		void SetNearFarZ(float nearZ, float farZ);
		void SetPosition(const Vector& pos);
		void SetPosition(float x, float y, float z);
		void SetRotation(const Vector& rot);
		void SetRotation(float pitch, float yaw, float roll);

		const Matrix& GetView() const;
		const Matrix& GetProjection() const;
		const Matrix& GetViewProjection() const;

		Quaternion GetOrientation() const;

		Vector GetRightDirection() const;
		Vector GetUpDirection() const;
		Vector GetForwardDirection() const;

		const CameraDesc& GetDesc() const;

		ID3D11Buffer* GetFrustumVB() const;
		ID3D11Buffer* GetFrustumIB() const;

	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateFrustumBuffer();

		CameraDesc m_desc;

		Matrix m_view;
		Matrix m_projection;
		Matrix m_viewProjection;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_frustumVB;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_frustumIB;
		ID3D11Device* m_device;
	};
}