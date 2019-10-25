#ifndef SUMPARSE_SUMPARSE_H_
#define SUMPARSE_SUMPARSE_H_

#include <istream>

#include "rhutil/status.h"
#include "sumparse/hashsum.pb.h"

namespace sumparse {

rhutil::StatusOr<HashSum> ParseHashSum(HashSum::Type type, std::istream *input);

}  // namespace sumparse

#endif  // SUMPARSE_SUMPARSE_H_
