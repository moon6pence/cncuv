#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Hello World!", "[hello]") 
{
    REQUIRE((1 + 1) == 2);
}

#include <cnc/cnc.h>

struct FibContext;

// declare data type for fibonacci
typedef unsigned long long fib_type;

struct FibStep
{
    int execute(const int &tag, FibContext &context) const;
};

struct FibContext : public CnC::Context<FibContext>
{
    CnC::StepCollection<FibStep> steps;
    CnC::ItemCollection<int, fib_type> fibs;
    CnC::TagCollection<int> tags;

    FibContext() : CnC::Context<FibContext>(), 
        steps(*this), 
        fibs(*this),
        tags(*this)
    {
        // Prescriptive relations
        tags.prescribes(steps);

        // MEMO: actually, current CnC model doesn't need producer and consumer relationships. 
        // Consumer relations
        // steps.consumes(fibs);
        // Producer relations
        // steps.produces(fibs);
    }
};

int FibStep::execute(const int &tag, FibContext &context) const
{
    switch (tag) 
    {
        case 0: context.fibs.put(tag, 0); break;
        case 1: context.fibs.put(tag, 1); break;
        default:
            // get previous 2 results
            fib_type f_1; context.fibs.get(tag - 1, f_1);
            fib_type f_2; context.fibs.get(tag - 2, f_2);
            // put our result
            context.fibs.put(tag, f_1 + f_2);
    }

    return 0;
}

TEST_CASE("Create instance", "[fib]")
{
    int n = 42;

    // create context
    FibContext context;

    // put tags to initiate evaluation
    for (int i = 0; i <= n; ++i) 
        context.tags.put(i);

    // wait for completion
    // context.wait();

    // get result
    fib_type result;
    context.fibs.get(n, result);

    // check result
    REQUIRE(result == 267914296);
}