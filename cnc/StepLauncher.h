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

template <typename Tag, typename UserStep, typename Arg>
class StepInstance
{
public:
    StepInstance(const StepCollection<UserStep> &stepCollection, const Tag &tag, Arg &arg) :
        _stepCollection(stepCollection), 
        _tag(tag), // copy tag here
        _arg(arg)
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
        _this->_stepCollection.step.execute(_this->_tag, _this->_arg);
    }

    static void _finalize(uv_work_t *req, int status)
    {
        StepInstance *_this = (StepInstance *)req->data;
        delete _this;
    }

    const StepCollection<UserStep> &_stepCollection;
    Tag _tag;
    Arg &_arg;

    uv_work_t _request;
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
        auto *instance = new StepInstance<Tag, UserStep, Arg>(_stepCollection, tag, _arg);

        // async call
        instance->invoke();
    }

private:
    const StepCollection<UserStep> &_stepCollection;
    Arg &_arg;
};

} // namespace CnC

#endif // STEP_LAUNCHER_H_