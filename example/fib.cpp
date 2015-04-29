#include <cnc/cnc.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

struct FibContext;

// declare data type for fibonacci
typedef unsigned long long fib_type;

struct FibStep
{
    int execute(const int &tag, FibContext &context) const;
};

struct FibContext : public CnC::Context<FibContext>
{
    CnC::StepCollection<int, FibStep> steps;
    CnC::ItemCollection<int, fib_type> fibs;

    FibContext() : CnC::Context<FibContext>(), 
        steps(*this), 
        fibs()
    {
    }
};

int FibStep::execute(const int &tag, FibContext &context) const
{
    // std::cout << "start " << tag << std::endl;

    // get previous 2 results
    fib_type f_1; context.fibs.get(tag - 1, f_1);
    fib_type f_2; context.fibs.get(tag - 2, f_2);

    // std::cout << f_1 << " + " << f_2 << " = " << (f_1 + f_2) << std::endl;

    // put our result
    context.fibs.put(tag, f_1 + f_2);

    // std::cout << "end " << tag << std::endl;

    return 0;
}

TEST_CASE("Get fibonacci number", "[fib]")
{
    int n = 42;

    // create context
    FibContext context;

    // seed
    context.fibs.put(0, 0);
    context.fibs.put(1, 1);

    // put tags to initiate evaluation
    for (int i = 2; i <= n; ++i)
        context.steps.put(i);

    // wait for completion
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    // get result
    fib_type result;
    context.fibs.get(n, result);

    // check result
    REQUIRE(result == 267914296);
}