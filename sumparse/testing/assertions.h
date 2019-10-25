#ifndef SUMPARSE_TESTING_ASSERTIONS_H_
#define SUMPARSE_TESTING_ASSERTIONS_H_

#include "gtest/gtest.h"
#include "google/protobuf/util/message_differencer.h"
#include "google/protobuf/message.h"

namespace sumparse {

testing::AssertionResult IsEqual(const google::protobuf::Message &a,
                                 const google::protobuf::Message &b);

}  // namespace sumparse

#endif  // SUMPARSE_TESTING_ASSERTIONS_H_
