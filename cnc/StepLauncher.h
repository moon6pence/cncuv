#ifndef STEP_LAUNCHER_H_
#define STEP_LAUNCHER_H_

#include <functional>

namespace CnC {

// forward declarations
template <typename UserStep>
class StepCollection;

// StepLauncherBase
template <typename Tag>
class StepLauncherBase
{
public:
    virtual void executeStep(const Tag &tag) = 0;
};

// StepLauncher
template <typename Tag, typename UserStep, typename Arg>
class StepLauncher : public StepLauncherBase<Tag>
{
public:
    StepLauncher(const StepCollection<UserStep> &stepCollection, Arg &arg) :
        _stepCollection(stepCollection), _arg(arg)
    {
    }

    void executeStep(const Tag &tag) override
    {
        _stepCollection.step.execute(tag, _arg);
    }

private:
    const StepCollection<UserStep> &_stepCollection;
    Arg &_arg;
};

class StepInstance
{
public:
    StepInstance(std::function<int(void)> &&stepFunc) : _stepFunc(stepFunc) 
    { 
        _request.data = this;
    }

    void invoke()
    {
        uv_queue_work(uv_default_loop(), &_request, _run, _finalize);
    }

private:
    static void _run(uv_work_t *req)
    {
        StepInstance *_this = (StepInstance *)req->data;
        _this->_stepFunc();
    }

    static void _finalize(uv_work_t *req, int status)
    {
        StepInstance *_this = (StepInstance *)req->data;
        delete _this;
    }

    uv_work_t _request;
    std::function<int(void)> _stepFunc;
};

// StepLauncherAsync
template <typename Tag, typename UserStep, typename Arg>
class StepLauncherAsync : public StepLauncherBase<Tag>
{
public:
    StepLauncherAsync(const StepCollection<UserStep> &stepCollection, Arg &arg) :
        _stepCollection(stepCollection), _arg(arg)
    {
    }

    void executeStep(const Tag &tag) override
    {
        StepInstance *instance = new StepInstance(std::bind(&UserStep::execute, &_stepCollection.step, std::cref(tag), std::ref(_arg)));

        // async call
        instance->invoke();
    }

private:
    const StepCollection<UserStep> &_stepCollection;
    Arg &_arg;
};

} // namespace CnC

#endif // STEP_LAUNCHER_H_