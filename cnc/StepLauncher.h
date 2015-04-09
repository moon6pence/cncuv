#ifndef STEP_LAUNCHER_H_
#define STEP_LAUNCHER_H_

namespace CnC {

// forward declarations
template <typename UserStep>
class StepCollection;

// StepLauncherBase
class StepLauncherBase
{
};

// StepLauncher
template <typename UserStep>
class StepLauncher : public StepLauncherBase
{
public:
    StepLauncher(const StepCollection<UserStep> &stepCollection);

private:
    const StepCollection<UserStep> &_stepCollection;
};

// Implementation

template <typename UserStep>
inline StepLauncher<UserStep>::StepLauncher(const StepCollection<UserStep> &stepCollection) :
    _stepCollection(stepCollection)
{
}

} // namespace CnC

#endif // STEP_LAUNCHER_H_