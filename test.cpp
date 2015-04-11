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

long fib_(long t) 
{
    if (t == 0 || t == 1)
        return 1;
    else
        return fib_(t - 1) + fib_(t - 2);
}

void fib(uv_work_t *req)
{
    int n = *(int *)req->data;
    //if (random() % 2)
    //    sleep(1);
    //else
    //    sleep(3);

    long fib = fib_(n);
    fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

TEST_CASE("Test work queue", "[libuv]")
{
    const int FIB_UNTIL = 42;
    uv_loop_t *loop = uv_default_loop();

    int data[FIB_UNTIL];
    uv_work_t req[FIB_UNTIL];
    int i;
    for (i = 0; i < FIB_UNTIL; i++) 
    {
        data[i] = i;
        req[i].data = (void *)&data[i];
        uv_queue_work(loop, &req[i], fib, nullptr);
    }

    uv_run(loop, UV_RUN_DEFAULT);
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
    // context.wait();

    // get result
    fib_type result;
    context.fibs.get(n, result);

    // check result
    REQUIRE(result == 267914296);
}