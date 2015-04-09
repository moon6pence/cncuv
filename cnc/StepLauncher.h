#ifndef STEP_LAUNCHER_H_
#define STEP_LAUNCHER_H_

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

} // namespace CnC

#endif // STEP_LAUNCHER_H_