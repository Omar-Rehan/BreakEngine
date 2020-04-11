#pragma once
#include "PoolAllocator.h"

typedef void EntryPoint(uintptr_t parameters);

class JobSystem {
public:
	enum class Priority { LOW, NORMAL, HIGH, CRITICAL };

	struct Counter { int count; };

	struct Job {
		EntryPoint* pEntryPoint;
		uintptr_t parameters;
		Priority priority;
		Counter* pCounter;
	};
	
	void Initialize();
	void Terminate();
	
	void KickJob(Job job);
	void WaitForCounter(Counter* pCounter);
	void KickJobAndWait(Job job);

	Counter* AllocateCounter();
	void FreeCounter(Counter* pCounter);

private:
	bool m_bRunning = false;

	PoolAllocator m_poolAllocator;

	std::deque<Job> m_jobQueue;
	boost::fibers::mutex m_queueMutex;
	boost::fibers::condition_variable m_queueConditionVariable;

	std::vector<std::thread> m_threads;

	Job GetNextJobFromQueue();
	static void WorkerThread(JobSystem* jobSystem);
};