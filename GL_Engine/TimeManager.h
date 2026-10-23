#pragma once

#define TM TimeManager::GetInstance()


class TimeManager
{
public:
	static TimeManager* GetInstance()
	{
		static TimeManager instance;
		return &instance;
	}
	
	void Init();

	void Update();

	float GetGlobalTime() const { return globalTime; }
	float GetDeltaTime() const { return deltaTime; }
	float GetScaleTime() const { return scaleTime; }
	void SetScaleTime(float newScale) { scaleTime = newScale; }
private:
	TimeManager() = default;
	TimeManager(TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;
	~TimeManager() {}

	float globalTime = 0.0f;
	float scaleTime = 1.0f;
	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
};