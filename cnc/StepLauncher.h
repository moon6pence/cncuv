#ifndef STEP_LAUNCHER_H_
#define STEP_LAUNCHER_H_

#include <functional>

namespace CnC {

// forward declarations
template <typename Tag, typename UserStep>
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
    StepLauncher(const StepCollection<Tag, UserStep> &stepCollection, Arg &arg) :
        _stepCollection(stepCollection), _arg(arg)
    {
    }

    void executeStep(const Tag &tag) override
    {
        _stepCollection.step.execute(tag, _arg);
    }

private:
    const StepCollection<Tag, UserStep> &_stepCollection;
    Arg &_arg;
};

template <typename Tag, typename UserStep, typename Arg>
class StepInstance
{
public:
    StepInstance(const StepCollection<Tag, UserStep> &stepCollection, const Tag &tag, Arg &arg) :
        _stepCollection(stepCollection), 
        _tag(tag), // copy tag here
        _arg(arg), 
        _retry(false)
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
        _this->run();
    }

    void run()
    {
        try
        {
            _stepCollection.step.execute(_tag, _arg);
        }
        catch (cannot_find_item &)
        {
            // std::cerr << "Retry step with tag: " << _tag << std::endl;

            // reinvoke step after callback
            _retry = true;
        }
        catch (std::exception &e)
        {
            std::cerr << "Exception from step: " << e.what() << std::endl;
        }
    }

    static void _finalize(uv_work_t *req, int status)
    {
        StepInstance *_this = (StepInstance *)req->data;

        if (_this->_retry)
        {
            // reinvoke step
            _this->_retry = false;
            _this->invoke();
            return;
        }

        delete _this;
    }

    const StepCollection<Tag, UserStep> &_stepCollection;
    Tag _tag;
    Arg &_arg;

    uv_work_t _request;
    bool _retry;
};

// StepLauncherAsync
template <typename Tag, typename UserStep, typename Arg>
class StepLauncherAsync : public StepLauncherBase<Tag>
{
public:
    StepLauncherAsync(const StepCollection<Tag, UserStep> &stepCollection, Arg &arg) :
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
    const StepCollection<Tag, UserStep> &_stepCollection;
    Arg &_arg;
};

} // namespace CnC

#endif // STEP_LAUNCHER_H_