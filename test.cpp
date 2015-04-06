#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Hello World!", "[hello]") 
{
    REQUIRE((1 + 1) == 2);
}

#include <cnc.h>

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
        // Consumer relations
        steps.consumes(fibs);
        // Producer relations
        steps.produces(fibs);
    }
};

TEST_CASE("Create instance", "[fib]") 
{
    FibContext context;
}
