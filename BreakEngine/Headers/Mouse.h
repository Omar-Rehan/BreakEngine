#pragma once

class Mouse {
public:
    Mouse();
    void Update(float nextX, float nextY);
    float GetDeltaX();
    float GetDeltaY();

private:
    bool m_bFirstFrame;
    float m_sensitivity;
    float m_crntX, m_crntY, m_deltaX, m_deltaY;
};