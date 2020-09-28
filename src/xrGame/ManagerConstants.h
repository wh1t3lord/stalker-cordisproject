#pragma once


class GlobalValues
{
private:
	GlobalValues(void) : m_time_factor(0.0f) {}

public:
	inline static GlobalValues& getInstance(void) noexcept { static GlobalValues instance; return instance; }

	~GlobalValues(void) {}
	GlobalValues(const GlobalValues&) = delete;
	GlobalValues& operator=(const GlobalValues&) = delete;
	GlobalValues(GlobalValues&&) = delete;
	GlobalValues& operator=(GlobalValues&&) = delete;

	inline float getTimeFactor(void) const noexcept { return this->m_time_factor; }
	inline void setTimeFactor(const float value) noexcept { this->m_time_factor = value; }

private:
	float m_time_factor;
};