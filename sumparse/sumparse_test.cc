#include "sumparse/sumparse.h"

#include <string>
#include <utility>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "rhutil/status.h"
#include "rhutil/testing/assertions.h"
#include "sumparse/testing/assertions.h"
#include "absl/strings/str_format.h"
#include "sumparse/hashsum.pb.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"

namespace sumparse {

using ::rhutil::IsOk;
using ::rhutil::StatusOr;
using ::rhutil::UnknownError;
using ::google::protobuf::io::IstreamInputStream;
using ::google::protobuf::TextFormat;

namespace {

HashSum HashSumFromProtoStringOrDie(std::string_view textproto) {
  HashSum hashsum;
  if (!TextFormat::ParseFromString(std::string(textproto), &hashsum)) {
    CHECK_OK(UnknownError(absl::StrFormat("Failed to parse %s", textproto)));
  }
  return hashsum;
}

StatusOr<HashSum> ParseHashSumFromString(HashSum::Type type,
                                          std::string_view s) {
  std::istringstream istrm{std::string(s)};
  return ParseHashSum(type, &istrm);
}

struct HashSumProtoSample {
  HashSumProtoSample(std::string_view textproto, std::string hashsum)
      : hashsum(std::move(hashsum)) {
    expected = HashSumFromProtoStringOrDie(textproto);
  }

  HashSum expected;
  std::string hashsum;
};

class HashSumEqualsProtoTest :
  public testing::TestWithParam<HashSumProtoSample> {};

TEST_P(HashSumEqualsProtoTest, MatchSample) {
  const auto &p = GetParam();
  const HashSum &expected = p.expected;
  const std::string &hashsum = p.hashsum;

  auto found_or = ParseHashSumFromString(expected.type(), hashsum);
  ASSERT_TRUE(IsOk(found_or));

  EXPECT_TRUE(IsEqual(expected, found_or.ValueOrDie()));
}

INSTANTIATE_TEST_SUITE_P(
    Simple,
    HashSumEqualsProtoTest,
    testing::Values(
      HashSumProtoSample(
        R"""(type: TYPE_SHA1
             file { name: "foo.bin" type: TYPE_BINARY
                    hash: "2e5a9235aaa4c04baefb8a5104fa2c2977869191" })""",
        "2e5a9235aaa4c04baefb8a5104fa2c2977869191 *foo.bin"
      ),
      HashSumProtoSample(
        R"""(type: TYPE_SHA1
             file { name: "bar.txt" type: TYPE_TEXT
                    hash: "d27d673244311650699c0fc2ed99473db8de38e1" })""",
        "d27d673244311650699c0fc2ed99473db8de38e1  bar.txt"
      ),
      HashSumProtoSample(
        R"""(type: TYPE_SHA1
             file { name: "foo.bin" type: TYPE_BINARY
                    hash: "2e5a9235aaa4c04baefb8a5104fa2c2977869191" }
             file { name: "bar.txt" type: TYPE_TEXT
                    hash: "d27d673244311650699c0fc2ed99473db8de38e1" })""",
        ("2e5a9235aaa4c04baefb8a5104fa2c2977869191 *foo.bin\n"
         "d27d673244311650699c0fc2ed99473db8de38e1  bar.txt")
      )
    )
);

}  // namespace
}  // namespace sumparse
