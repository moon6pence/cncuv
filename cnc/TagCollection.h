#ifndef TAG_COLLECTION_H_
#define TAG_COLLECTION_H_

namespace CnC {

template <typename Tag>
template <typename Derived>
TagCollection<Tag>::TagCollection(Context<Derived> &context)
{ 
}

template <typename Tag>
template <typename UserStep, typename Arg>
void TagCollection<Tag>::prescribes(const StepCollection<UserStep> &stepCollection, Arg &arg)
{
    // Initiate new step launcher
    StepLauncherBase<Tag> *stepLauncher = new StepLauncher<Tag, UserStep, Arg>(stepCollection, arg);
    _stepLauncher.reset(stepLauncher);
}

template <typename Tag>
void TagCollection<Tag>::put(const Tag &tag)
{
    if (!_stepLauncher) return;

    _stepLauncher->executeStep(tag);
}

} // namespace CnC

#endif // TAG_COLLECTION_H_