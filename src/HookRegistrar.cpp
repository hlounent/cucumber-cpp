#include <cucumber-cpp/internal/hook/HookRegistrar.hpp>

#include <cucumber-cpp/internal/CukeCommands.hpp>
#include <cucumber-cpp/internal/utils/ScopedList.hpp>

#include <boost/scoped_ptr.hpp>

namespace cucumber {
namespace internal {

Hook::~Hook()
{
}

void Hook::invokeHook(Scenario *scenario) {
    if (tagsMatch(scenario)) {
        body();
    } else {
        skipHook();
    }
}

void Hook::skipHook() {
}

void Hook::setTags(const std::string &csvTagNotation) {
    tagExpression = shared_ptr<TagExpression>(new AndTagExpression(csvTagNotation));
}

bool Hook::tagsMatch(Scenario *scenario) {
    return !scenario || tagExpression->matches(scenario->getTags());
}

void AroundStepHook::invokeHook(Scenario *scenario, CallableStep *step) {
    this->step = step;
    Hook::invokeHook(scenario);
}

void AroundStepHook::skipHook() {
    step->call();
}

void UnconditionalHook::invokeHook(Scenario*) {
    body();
}

HookRegistrar::~HookRegistrar() {
}

void HookRegistrar::addBeforeHook(BeforeHook *hook) {
    beforeHooks().push_back(hook);
    typedef ScopedList<Hook> Deleter;
    static boost::scoped_ptr<Deleter> deleter(new Deleter());
    deleter->add(hook);
}

HookRegistrar::hook_list_type& HookRegistrar::beforeHooks() {
    static boost::scoped_ptr<hook_list_type> beforeHooks(new hook_list_type());
    return *beforeHooks;
}

void HookRegistrar::execBeforeHooks(Scenario *scenario) {
    execHooks(beforeHooks(), scenario);
}


void HookRegistrar::addAroundStepHook(AroundStepHook *hook) {
    aroundStepHooks().push_front(hook);
    typedef ScopedList<Hook> Deleter;
    static boost::scoped_ptr<Deleter> deleter(new Deleter());
    deleter->add(hook);
}

HookRegistrar::aroundhook_list_type& HookRegistrar::aroundStepHooks() {
    static boost::scoped_ptr<aroundhook_list_type> aroundStepHooks(new aroundhook_list_type());
    return *aroundStepHooks;
}

InvokeResult HookRegistrar::execStepChain(Scenario *scenario, StepInfo *stepInfo, const InvokeArgs *pArgs) {
    StepCallChain scc(scenario, stepInfo, pArgs, aroundStepHooks());
    return scc.exec();
}


void HookRegistrar::addAfterStepHook(AfterStepHook *hook) {
    afterStepHooks().push_front(hook);
    typedef ScopedList<Hook> Deleter;
    static boost::scoped_ptr<Deleter> deleter(new Deleter());
    deleter->add(hook);
}

HookRegistrar::hook_list_type& HookRegistrar::afterStepHooks() {
    static boost::scoped_ptr<hook_list_type> afterStepHooks(new hook_list_type());
    return *afterStepHooks;
}

void HookRegistrar::execAfterStepHooks(Scenario *scenario) {
    execHooks(afterStepHooks(), scenario);
}


void HookRegistrar::addAfterHook(AfterHook *hook) {
    afterHooks().push_front(hook);
    typedef ScopedList<Hook> Deleter;
    static boost::scoped_ptr<Deleter> deleter(new Deleter());
    deleter->add(hook);
}

HookRegistrar::hook_list_type& HookRegistrar::afterHooks() {
    static boost::scoped_ptr<hook_list_type> afterHooks(new hook_list_type());
    return *afterHooks;
}

void HookRegistrar::execAfterHooks(Scenario *scenario) {
    execHooks(afterHooks(), scenario);
}


void HookRegistrar::execHooks(HookRegistrar::hook_list_type &hookList, Scenario *scenario) {
    for (HookRegistrar::hook_list_type::iterator hook = hookList.begin(); hook != hookList.end(); ++hook) {
        (*hook)->invokeHook(scenario);
    }
}

HookRegistrar::hook_list_type& HookRegistrar::beforeAllHooks() {
    static boost::scoped_ptr<hook_list_type> beforeAllHooks(new hook_list_type());
    return *beforeAllHooks;
}

void HookRegistrar::addBeforeAllHook(BeforeAllHook *hook) {
    beforeAllHooks().push_back(hook);
    typedef ScopedList<Hook> Deleter;
    static boost::scoped_ptr<Deleter> deleter(new Deleter());
    deleter->add(hook);
}

void HookRegistrar::execBeforeAllHooks() {
    execHooks(beforeAllHooks(), NULL);
}

HookRegistrar::hook_list_type& HookRegistrar::afterAllHooks() {
    static boost::scoped_ptr<hook_list_type> afterAllHooks(new hook_list_type());
    return *afterAllHooks;
}

void HookRegistrar::addAfterAllHook(AfterAllHook *hook) {
    afterAllHooks().push_back(hook);
    typedef ScopedList<Hook> Deleter;
    static boost::scoped_ptr<Deleter> deleter(new Deleter());
    deleter->add(hook);
}

void HookRegistrar::execAfterAllHooks() {
    execHooks(afterAllHooks(), NULL);
}


StepCallChain::StepCallChain(
    Scenario *scenario,
    StepInfo *stepInfo,
    const InvokeArgs *pStepArgs,
    HookRegistrar::aroundhook_list_type &aroundHooks
) :
    scenario(scenario),
    stepInfo(stepInfo),
    pStepArgs(pStepArgs)
{
    nextHook = aroundHooks.begin();
    hookEnd = aroundHooks.end();
}

InvokeResult StepCallChain::exec() {
    execNext();
    return result;
}

void StepCallChain::execNext() {
    if (nextHook == hookEnd) {
        execStep();
    } else {
        HookRegistrar::aroundhook_list_type::iterator currentHook = nextHook++;
        CallableStepChain callableStepChain(this);
        (*currentHook)->invokeHook(scenario, &callableStepChain);
    }
}

void StepCallChain::execStep() {
    if (stepInfo) {
        result = stepInfo->invokeStep(pStepArgs);
    }
}


CallableStepChain::CallableStepChain(StepCallChain *scc) : scc(scc) {};

void CallableStepChain::call() {
    scc->execNext();
}


}
}
