#include <uv.h>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Hello World!", "[hello]") 
{
    REQUIRE((1 + 1) == 2);
}

static void callback(uv_write_t *req, int status)
{
    printf("Hello from Callback.\n");
}

TEST_CASE("Hello libuv", "[libuv]")
{
    uv_tty_t tty;
    uv_write_t req;
    int ret = uv_tty_init(uv_default_loop(), &tty, 1, 0);

    uv_buf_t bufs[] = { uv_buf_init("Hello UV!\n", 10) };
    ret = uv_write(&req, (uv_stream_t*)&tty, bufs, 1, callback);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void work(uv_work_t *req)
{
    int *p = (int *)req->data;
    *p = *p + 1;
}

TEST_CASE("Test work queue", "[libuv]")
{
    int n = 3;
    uv_work_t req;
    req.data = &n;
    uv_queue_work(uv_default_loop(), &req, work, nullptr);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    REQUIRE(n == (3 + 1));
}

#include <functional>

void add_one(int &n)
{
    n = n + 1;
    std::cout << n << std::endl;
}

void run_function(uv_work_t *req)
{
    std::function<void()> &func = *(std::function<void()> *)req->data;
    func();
}

TEST_CASE("Test work queue with std::function", "[libuv]")
{
    int n = 4;
    std::function<void()> func = std::bind(add_one, std::ref(n));
    func(); // direct call binded function
    REQUIRE(n == 5);

    // async call
    uv_work_t req;
    req.data = &func;
    uv_queue_work(uv_default_loop(), &req, run_function, nullptr);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    REQUIRE(n == 6);
}

#include <cnc/cnc.h>

TEST_CASE("Put and get item", "[ItemCollection]")
{
    CnC::ItemCollection<int, std::string> numbers;

    REQUIRE(numbers.empty() == true);
    REQUIRE(numbers.size() == 0);

    numbers.put(1, "one");
    numbers.put(2, "two");
    numbers.put(3, "three");

    REQUIRE(numbers.empty() == false);
    REQUIRE(numbers.size() == 3);

    std::string item;
    numbers.get(1, item);
    
    REQUIRE(item == "one");
}

struct SimpleContext;

struct SimpleStep
{
    int execute(const int &tag, SimpleContext &context) const;
};

struct SimpleContext : public CnC::Context<SimpleContext>
{
    CnC::TagCollection<int> tags;
    CnC::StepCollection<SimpleStep> steps;

    int result, sum;

    SimpleContext() : CnC::Context<SimpleContext>(), 
        tags(*this), 
        steps(*this), 
        result(0), 
        sum(0)
    {
        tags.prescribes(steps, *this);
    }
};

int SimpleStep::execute(const int &tag, SimpleContext &context) const
{
    //std::cout << "start " << tag << std::endl;
    context.result = tag + 1;
    context.sum += tag; // MEMO: this line is not thread safe
    //std::cout << "end " << tag << std::endl;
    return 0;
}

TEST_CASE("Simple step execution", "[StepCollection]")
{
    SimpleContext context;
    context.tags.put(7);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    REQUIRE(context.result == 8);
}

TEST_CASE("Iterative step execution", "[StepCollection]")
{
    SimpleContext context;
    for (int i = 1; i <= 100; i++)
        context.tags.put(i);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    REQUIRE(context.sum == 5050);
}

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
        fibs(),
        tags(*this)
    {
        // Prescriptive relations
        tags.prescribes(steps, *this);

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

            // std::cout << f_1 << " + " << f_2 << " = " << (f_1 + f_2) << std::endl;

            // put our result
            context.fibs.put(tag, f_1 + f_2);
    }

    return 0;
}

TEST_CASE("Get fibonacci number", "[fib]")
{
    int n = 42;

    // create context
    FibContext context;

    // put tags to initiate evaluation
    for (int i = 0; i <= n; ++i)
        context.tags.put(i);

    // wait for completion
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    // get result
    fib_type result;
    context.fibs.get(n, result);

    // check result
    REQUIRE(result == 267914296);
}