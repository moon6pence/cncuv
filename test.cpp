#include <uv.h>
#include <atomic>

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
    CnC::StepCollection<int, SimpleStep> steps;

    int result;
    std::atomic<int> sum;

    SimpleContext() : CnC::Context<SimpleContext>(), 
        // steps(*this), 
        steps(*this, *this), // test: pass context as argument explicitly
        result(0), 
        sum(0)
    {
    }
};

int SimpleStep::execute(const int &tag, SimpleContext &context) const
{
    // std::cout << "start " << tag << std::endl;
    context.result = tag + 1;
    context.sum += tag; // this line have to be thread safe
    // std::cout << "end " << tag << std::endl;
    return 0;
}

TEST_CASE("Simple step execution", "[StepCollection]")
{
    SimpleContext context;
    context.steps.put(7);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    REQUIRE(context.result == 8);
}

TEST_CASE("Iterative step execution", "[StepCollection]")
{
    SimpleContext context;
    for (int i = 1; i <= 100; i++)
        context.steps.put(i);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    REQUIRE(context.sum == 5050);
}
