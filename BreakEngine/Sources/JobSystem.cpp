#include "pch.h"
#include "JobSystem.h"

void JobSystem::Initialize() {
	m_bRunning = true;
	m_poolAllocator.Initialize(200, sizeof(Counter), sizeof(Counter));
	
	const unsigned int numOfCores = std::thread::hardware_concurrency();
	for (int i = 0; i < numOfCores - 1; i++) {
		m_threads.push_back(std::thread(WorkerThread, this));
	}
}
void JobSystem::Terminate() {
	m_bRunning = false;
	m_poolAllocator.Terminate();

	m_queueConditionVariable.notify_all();
	for (std::thread& thread : m_threads)
		if (thread.joinable()) thread.join();
}

void JobSystem::KickJob(Job job) {
	if (job.pCounter != nullptr) job.pCounter->count++;
	m_queueMutex.lock();
	
	m_jobQueue.push_back(job);
	
	m_queueMutex.unlock();
	m_queueConditionVariable.notify_all();
}
void JobSystem::KickJobAndWait(Job job) {
	if (job.pCounter == nullptr) {
		std::cout << "Can't wait for a job if it doesn't have your counter" << std::endl;
		return;
	}
	
	job.pCounter->count++;
	m_queueMutex.lock();

	m_jobQueue.push_back(job);

	m_queueMutex.unlock();
	m_queueConditionVariable.notify_all();

	while (job.pCounter->count) boost::this_fiber::yield();
}
void JobSystem::WaitForCounter(Counter* pCounter) {
	if (pCounter == nullptr) {
		std::cout << "Can't wait for a nullptr counter!" << std::endl;
		return;
	}

	while (pCounter->count) boost::this_fiber::yield();
}


JobSystem::Job JobSystem::GetNextJobFromQueue() {
	if (m_jobQueue.empty()) {
		std::cout << "Job queue is empty. What are you trying to pull?" << std::endl;
		return {NULL};
	}

	Job job = m_jobQueue.front();
	m_jobQueue.pop_front();
	return job;
}

void JobSystem::WorkerThread(JobSystem* jobSystem) {
	boost::fibers::mutex fibersQueueMutex;
	std::queue<std::pair<boost::fibers::fiber, Counter*>> fibers;
	
	boost::fibers::fiber loopFiber(
		[&] {
		while (jobSystem->m_bRunning) {
			/// Make sure the queue is not empty & that we lock the queue for exclusive access
			std::unique_lock<boost::fibers::mutex> uniqueLock(jobSystem->m_queueMutex);
			jobSystem->m_queueConditionVariable.wait(uniqueLock, [&] {return !jobSystem->m_bRunning || !jobSystem->m_jobQueue.empty();});

			if (!jobSystem->m_bRunning) break;
			/// Pull a job from the queue & create a fiber for it
			{
				Job job = jobSystem->GetNextJobFromQueue();
				fibersQueueMutex.lock();
				fibers.emplace(
					std::pair<boost::fibers::fiber, Counter*>(
						boost::fibers::fiber(job.pEntryPoint, job.parameters),
						job.pCounter
					)
				);
				fibersQueueMutex.unlock();
			}

			uniqueLock.unlock();
			boost::this_fiber::yield(); /// Give the chance to other fibers to execute
		}
		return;
		}
	);

	boost::fibers::fiber checkFiber(
		[&] {
		/// Check the queue for old fibers that finished execution
		while (jobSystem->m_bRunning) {
			fibersQueueMutex.lock();
			if (!fibers.empty()) {
				fibers.front().first.join();

				Counter* oldFiberCounter = fibers.front().second;
				if (oldFiberCounter != nullptr) oldFiberCounter->count--; /// decrement its count for if another fiber is waiting on it
				fibers.pop();
			}
			fibersQueueMutex.unlock();
			boost::this_fiber::yield();
		}

		/// Clear the fibers queue
		fibersQueueMutex.lock();
		while (!fibers.empty()) {
			if (fibers.front().first.joinable()) 
				fibers.front().first.join();
			fibers.pop();
		}
		fibersQueueMutex.unlock();

		return;
		}
	);

	if (loopFiber.joinable()) loopFiber.join();
	if (checkFiber.joinable()) checkFiber.join();
}

JobSystem::Counter* JobSystem::AllocateCounter() {
	Counter* counter = reinterpret_cast<Counter*>(m_poolAllocator.Allocate());
	counter->count = 0;
	return counter;
}
void JobSystem::FreeCounter(Counter* pCounter) {
	if (pCounter != nullptr) m_poolAllocator.Deallocate(pCounter);
}