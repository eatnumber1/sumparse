#include "sumparse/sumparse.h"

#include <string>
#include <string_view>

#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/strings/strip.h"

namespace sumparse {

using ::rhutil::StatusOr;
using ::rhutil::Status;
using ::rhutil::OkStatus;
using ::rhutil::InvalidArgumentError;

namespace {

Status ParseLine(std::string_view line, HashSum *hashsum) {
  std::pair<std::string_view, std::string_view> splits =
    absl::StrSplit(line, absl::MaxSplits(' ', 1));
  if (splits.first.empty() || splits.second.empty()) {
    return InvalidArgumentError(
        absl::StrFormat("Failed to parse line '%s'\n", line));
  }
  std::string_view hash = splits.first;
  char type = splits.second[0];
  std::string_view name = splits.second.substr(1);

  HashSum::File::Type file_type;
  switch (type) {
    case '*':
      file_type = HashSum::File::TYPE_BINARY;
      break;
    case ' ':
      file_type = HashSum::File::TYPE_TEXT;
      break;
    default:
      return InvalidArgumentError(
          absl::StrFormat("Unknown file type '%c' in line '%s'\n", type, line));
  }

  HashSum::File *file = hashsum->add_file();
  file->set_hash(std::string(hash));
  file->set_name(std::string(name));
  file->set_type(file_type);
  return OkStatus();
}

}  // namespace

StatusOr<HashSum> ParseHashSum(HashSum::Type type, std::istream *input) {
  HashSum hashsum;
  hashsum.set_type(type);

  int lineno = 1;
  for (std::string line; std::getline(*input, line); lineno++) {
    std::string_view clean_line(line);
    absl::ConsumeSuffix(&clean_line, "\r");
    Status st = ParseLine(clean_line, &hashsum);
    if (!st.ok()) {
      return Status(st.code(), absl::StrFormat("Error on line %d: %s", lineno,
                                               st.message()));
    }
  }

  return std::move(hashsum);
}

}  // namespace sumparse
