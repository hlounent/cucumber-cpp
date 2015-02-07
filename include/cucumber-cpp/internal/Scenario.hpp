#ifndef CUKE_SCENARIO_HPP_
#define CUKE_SCENARIO_HPP_

#include "hook/Tag.hpp"

#include <boost/scoped_ptr.hpp>

namespace cucumber {
namespace internal {

class Scenario {
public:
    Scenario(const TagExpression::tag_list *pTags);

    const TagExpression::tag_list & getTags();
private:
    const TagExpression::tag_list* pTags;
    boost::scoped_ptr<const TagExpression::tag_list> pOwnedTags;
};

}
}

#endif /* CUKE_SCENARIO_HPP_ */
