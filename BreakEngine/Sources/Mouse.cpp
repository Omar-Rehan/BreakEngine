#include "pch.h"
#include "Mouse.h"

Mouse::Mouse() { 
    m_bFirstFrame = true; 
    m_sensitivity = 0.8f; 
    
    m_crntX = m_crntY = 0.0f;
    m_deltaX = m_deltaY = 0.0f;
}

void Mouse::Update(float nextX, float nextY) {
    if (m_bFirstFrame) {
        m_crntX = nextX;
        m_crntY = nextY;
        m_bFirstFrame = false;
    }

    m_deltaX = (nextX - m_crntX) * m_sensitivity;
    m_deltaY = (nextY - m_crntY) * m_sensitivity;

    m_crntX = nextX;
    m_crntY = nextY;
}

float Mouse::GetDeltaX() { return m_deltaX; }
float Mouse::GetDeltaY() { return m_deltaY; }