
#include "camera.h"
#include "viewport.h"
#include "../utility/error.h"

namespace ysl
{
	namespace  vpl
	{
		Camera_Impl::Camera_Impl(const ysl::Point3f& position, ysl::Vector3f up, const ysl::Point3f& center) :
			m_position(position),
			m_front(center - position),
			m_worldUp(up),
			m_movementSpeed(SPEED),
			m_mouseSensitivity(SENSITIVITY),
			m_center(center),
			m_zoom(ZOOM)
		{
			m_right = ysl::Vector3f::Cross(m_front, m_worldUp);
			m_up = ysl::Vector3f::Cross(m_right, m_front);
			//updateCameraVectors(QVector3D(0,1,0),QVector3D(0,0,0),0);
		}

		void Camera_Impl::setCenter(const ysl::Point3f& center)
		{
			m_center = center;
			m_front = (m_center - m_position).Normalized();
			m_right = ysl::Vector3f::Cross(m_front, m_worldUp).Normalized();
			m_up = ysl::Vector3f::Cross(m_right, m_front).Normalized();
		}

		void Camera_Impl::movement(const ysl::Vector3f& direction, float deltaTime)
		{
			const auto velocity = m_movementSpeed * direction * deltaTime;
			m_position += velocity;
		}

		void Camera_Impl::rotation(float xoffset, float yoffset)
		{
			xoffset *= m_mouseSensitivity;
			yoffset *= m_mouseSensitivity;
			const auto theta = 4.0 * (std::fabs(xoffset) + std::fabs(yoffset));
			const auto v = ((m_right * xoffset) + (m_up * yoffset));
			const auto axis = ysl::Vector3f::Cross(v, -m_front).Normalized();
			updateCameraVectors(axis, theta);
		}

		void Camera_Impl::processMouseScroll(float yoffset)
		{
			if (m_zoom >= 1.0f && m_zoom <= 45.0f)
				m_zoom -= yoffset;
			if (m_zoom <= 1.0f)
				m_zoom = 1.0f;
			if (m_zoom >= 45.0f)
				m_zoom = 45.0f;
		}

		void Camera_Impl::updateCameraVectors(const ysl::Vector3f& axis, double theta)
		{
			ysl::Transform rotation;
			rotation.SetRotate(axis, theta);
			ysl::Transform translation;
			translation.SetTranslate(-m_center.x, -m_center.y, -m_center.z);
			m_position = translation.Inversed() * (rotation * (translation * m_position));
			m_front = (rotation * m_front.Normalized());
			m_up = (rotation * m_up.Normalized());
			m_right = ysl::Vector3f::Cross(m_front, m_up);
			m_up = ysl::Vector3f::Cross(m_right, m_front);
			m_front.Normalize();
			m_right.Normalize();
			m_up.Normalize();
		}


		void CameraManipulator::ResizeEvent(int w, int h)
		{
			//assert(camera);
			if (camera) 
			{
				auto viewport = camera->GetViewport();
				assert(viewport);
				if (viewport)
				{
					viewport->SetViewportSize(w, h);
					Transform proj;
					proj.SetGLPerspective(45.f, float(w) / float(h), 0.01, 100);
					camera->SetProjectionMatrix(proj);
				}
			}

		}

		void CameraManipulator::AddedEvent(RenderContext* context)
		{
			SetContext(context);
		}

		void CameraManipulator::DeletedEvent(RenderContext* context)
		{
			SetContext(nullptr);
		}

		void CameraManipulator::MousePressEvent(EMouseButton button, int xpos, int ypos)
		{
			lastMousePos.x = xpos;
			lastMousePos.y = ypos;
		}

		void CameraManipulator::MouseMoveEvent(EMouseButton button, int xpos, int ypos)
		{
			if (camera != nullptr)
			{
				//Debug("CameraManipulator::MouseMoveEvent");
				// Update Camera
				const float dx = xpos - lastMousePos.x;
				const float dy = lastMousePos.y - ypos;
				if (dx == 0.0 && dy == 0.0)
					return;
				if ((button & Mouse_Left) && (button & Mouse_Right))
				{
					const auto directionEx = camera->Up()*dy + dx * camera->Right();
					camera->Movement(directionEx, 0.002);
				}
				else if (button == Mouse_Left)
				{
					camera->Ratation(dx, dy);
				}
				else if (button == Mouse_Right)
				{
					const auto directionEx = camera->Front()*dy;
					camera->Movement(directionEx, 0.01);
				}
				lastMousePos.x = xpos;
				lastMousePos.y = ypos;


				//std::cout << camera->ViewMatrix() << std::endl;

			}
		}

	}

}