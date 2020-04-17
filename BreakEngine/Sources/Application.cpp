#include "pch.h"
#include "Application.h"

void Application::Initialize() {
	m_camera = Camera();
	m_pGLFW = new GLFW();
	m_pFileManager = new FileManager();
	m_pResourceManager = new ResourceManager();

	m_pGLFW->Initialize();
	m_pFileManager->Initialize();
	m_pResourceManager->Initialize(m_pFileManager);
}
void Application::Terminate() {
	m_pResourceManager->Terminate();
	m_pFileManager->Terminate();
	m_pGLFW->Terminate();
}

void Application::GameLoop() {
	Timer timer;
	m_pGLFW->AttachCamera(&m_camera);


	/// Color stuff
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/// Depth stuff
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	m_pFileManager->Mount("Assets/Nanosuit Model.zip");
	Model m = m_pResourceManager->GetModel("Nanosuit Model/nanosuit.obj");
	ShaderProgram s = ShaderProgram("Shaders/SimpleNanosuitVShader.glsl", "Shaders/SimpleNanosuitFShader.glsl");

	JobSystem jobSystem;
	jobSystem.Initialize();
	//TestJobSystem(&jobSystem);

	TestButtonSequence();

	while (!m_pGLFW->WindowShouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TestJobSystem2(&jobSystem, m, s);

		timer.Mark();
		m_pGLFW->SwapBuffers();
		m_pGLFW->PollEvents(timer.GetDeltaTimeAverage());
	}

	jobSystem.Terminate();
}


void Application::TestStackAllocator() {
	StackAllocator stk(222);
	stk.PrintAddresses();

	void* p1 = stk.Allocate(4, 4);
	stk.PrintAddresses();

	void* p2 = stk.Allocate(16, 8);
	stk.PrintAddresses();

	stk.Deallocate(p2);
	stk.PrintAddresses();

	stk.Deallocate(p1);
	stk.PrintAddresses();
}
void Application::TestPoolAllocator() {
	PoolAllocator p;
	p.Initialize(4, 4, 2);
	p.PrintAddresses();

	void* p1 = p.Allocate();
	p.PrintAddresses();

	void* p2 = p.Allocate();
	p.PrintAddresses();

	p.Deallocate(p1);
	p.PrintAddresses();

	p.Deallocate(p2);
	p.PrintAddresses();

	p.Terminate();
}

void Application::TestJobSystem2(JobSystem* jobSystem, Model& m, ShaderProgram& s) {
	struct ParametersStruct {
		Application* pApp;
		glm::mat4* modelMatrix;
		glm::mat4* viewMatrix;
		glm::mat4* projectionMatrix;
	};

	glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
	ParametersStruct parameters;
	{
		parameters.pApp = this;
		parameters.modelMatrix = &modelMatrix;
		parameters.viewMatrix = &viewMatrix;
		parameters.projectionMatrix = &projectionMatrix;
	}

	JobSystem::Job CalcMatrices {
		CalculateTransformationMatrices,
		reinterpret_cast<uintptr_t>(&parameters),
		JobSystem::Priority::NORMAL,
		nullptr
	};
	CalcMatrices.pCounter = jobSystem->AllocateCounter();
	jobSystem->KickJobAndWait(CalcMatrices);
	jobSystem->FreeCounter(CalcMatrices.pCounter);

	s.SetFloatMatrix4Uniform("modelMat", modelMatrix);
	s.SetFloatMatrix4Uniform("viewMat", viewMatrix);
	s.SetFloatMatrix4Uniform("projMat", projectionMatrix);
	m.Render(s);
}
void Application::CalculateTransformationMatrices(uintptr_t parameters) {
	struct ParametersStruct {
		Application* pApp;
		glm::mat4* modelMatrix;
		glm::mat4* viewMatrix;
		glm::mat4* projectionMatrix;
	};
	ParametersStruct* params = reinterpret_cast<ParametersStruct*>(parameters);

	/// Model matrix
	*params->modelMatrix = glm::mat4(1.0f);
	*params->modelMatrix = glm::translate(*params->modelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));

	/// View matrix
	*params->viewMatrix = params->pApp->m_camera.GetViewMatrix();

	/// Projection matrix
	*params->projectionMatrix = glm::perspective(params->pApp->m_camera.GetFieldOfView(), 4.0f / 3.0f, 0.1f, 100.0f);
}

void Application::TestJobSystem(JobSystem* jobSystem) {
	Timer timer;
	const int maxNumOfJobs = 110;
	uint64_t start = 0, end = 200000, sum = 0;
	uint64_t starts[maxNumOfJobs], ends[maxNumOfJobs], sums[maxNumOfJobs];
	struct Interval {uint64_t *start, *end, *sum;} intervals[maxNumOfJobs];

	for (int numOfJobs = 100; numOfJobs < maxNumOfJobs; numOfJobs++) {
		std::cout << "Num of jobs = " << numOfJobs << std::endl;

		/// Test with mutlithreaded job system
		{
			timer.Mark();

			JobSystem::Counter* counter = jobSystem->AllocateCounter();
			for (int i = 0; i < numOfJobs; i++) {
				/// Initialize the arrays
				{
					starts[i] = start;
					ends[i] = end;
					sums[i] = sum;
				}

				/// Initialize the intervals
				{
					intervals[i].start = &starts[i];
					intervals[i].end = &ends[i];
					intervals[i].sum = &sums[i];
				}

				/// Create/Kick the jobs
				{
					JobSystem::Job oddsSumJob {
						GetOddsSum,
						reinterpret_cast<uintptr_t>(&intervals[i]),
						JobSystem::Priority::NORMAL,
						counter
					};
					jobSystem->KickJob(oddsSumJob);
				}
			}
			jobSystem->WaitForCounter(counter);
			jobSystem->FreeCounter(counter);

			timer.Mark();
			std::cout << "With job system: " << timer.GetDeltaTime() * 1000.0f << " ms" << std::endl;
		}

		/// Test single thread
		{
			timer.Mark();

			for (int i = 0; i < numOfJobs; i++) {
				/// Initialize the arrays
				{
					starts[i] = start;
					ends[i] = end;
					sums[i] = sum;
				}

				/// Initialize the intervals
				{
					intervals[i].start = &starts[i];
					intervals[i].end = &ends[i];
					intervals[i].sum = &sums[i];
				}

				/// Do calculations
				{
					GetOddsSum(reinterpret_cast<uintptr_t>(&intervals[i]));
				}
			}

			timer.Mark();
			std::cout << "Without job system: " << timer.GetDeltaTime() * 1000.0f << " ms" << std::endl;
		}


		std::cout << std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
}
void Application::GetOddsSum(uintptr_t parameters) {
	struct Interval {
		uint64_t* start;
		uint64_t* end;
		uint64_t* sum;
	};
	Interval* interval = reinterpret_cast<Interval*>(parameters);

	uint64_t i = *(interval->start);
	if (!(i & 1)) i++;

	while (i <= *(interval->end)) {
		*interval->sum += i;
		i += 2;
	}
}

void Application::TestButtonSequence() {
	std::vector<int> buttons {
		(int)Joystick::XBOX_BUTTONS::A,
		(int)Joystick::XBOX_BUTTONS::B,
		(int)Joystick::XBOX_BUTTONS::X,
		(int)Joystick::XBOX_BUTTONS::Y
	};
	std::function<void()> sequenceCompletedCallbackFunction = std::bind(&Application::PrintSequenceDetected, this);
	m_pGLFW->RegisterButtonSequence(GLFW::HID::Joystick, buttons, 2.0f, sequenceCompletedCallbackFunction);
}
void Application::PrintSequenceDetected() {
	std::cout << "SEQUENCE DETECTED!" << std::endl;
}