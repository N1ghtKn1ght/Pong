#include "Ball.h"

void Object::BallObject::move(const glm::vec2& pWindowSize)
{
	m_position += m_velocity;
	if (m_position.x <= 0.f)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = 0.f /*pWindowSize.x - m_size.x*/;
	}
	else if (m_position.x + m_size.x >= pWindowSize.x)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = pWindowSize.x - m_size.x;
	}
	else if (m_position.y <= 0.f)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = 0;
	}
	else if (m_position.y + m_size.y >= pWindowSize.y)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = pWindowSize.y - m_size.y;
	}
}

void Object::BallObject::setVelocity(const glm::vec2& velocity)
{
	m_velocity = velocity;
}
