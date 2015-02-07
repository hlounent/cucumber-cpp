#include <cucumber-cpp/internal/Scenario.hpp>

namespace cucumber {
namespace internal {

Scenario::Scenario(const TagExpression::tag_list *pTags) :
    pTags(pTags),
    pOwnedTags() {
    if (!pTags) {
        pOwnedTags.reset(new TagExpression::tag_list);
    }
};

const TagExpression::tag_list & Scenario::getTags() {
    return pTags != NULL ? *pTags : *(pOwnedTags.get());
}

}
}
