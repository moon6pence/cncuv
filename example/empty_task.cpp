#include <cnc/cnc.h>
#include <chrono>

// Forward declaration of the context class (also known as graph)
struct empty_context;

// The step classes
struct empty_step
{
	int execute(const int &tag, empty_context &context) const;
};

// The context class
struct empty_context : public CnC::Context<empty_context>
{
	// step collections
	CnC::StepCollection<int, empty_step> steps;

	// The context class constructor
	empty_context() : CnC::Context<empty_context>(),
		// Initialize each step collection
		steps(*this)
	{
	}
};

int empty_step::execute(const int &tag, empty_context &context) const
{
	return 0;
}

int main()
{
	const int n = 10000000;

	// set thread pool size
	CnC::setThreadPoolSize(1);

	// create context
	empty_context context;

	auto start = std::chrono::high_resolution_clock::now();

	// put tags to initiate evaluation
	for (int i = 0; i <= n; ++i)
		context.steps.put(i);

    // wait for completion
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << " sec" << std::endl;

	return 0;
}
