package(default_visibility = ["//sumparse:internal"])

package_group(
    name = "internal",
    packages = ["//sumparse/..."],
)

cc_proto_library(
    name = "hashsum_cc_proto",
    deps = [":hashsum_proto"],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "hashsum_proto",
    srcs = ["hashsum.proto"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "sumparse",
    visibility = ["//visibility:public"],
    srcs = ["sumparse.cc"],
    hdrs = ["sumparse.h"],
    deps = [
        ":hashsum_cc_proto",
        "@rhutil//rhutil:status",
        "@abseil//absl/strings",
        "@abseil//absl/strings:str_format",
    ],
)

cc_test(
    name = "sumparse_test",
    srcs = ["sumparse_test.cc"],
    deps = [
        ":sumparse",
        ":hashsum_cc_proto",
        "@rhutil//rhutil/testing:protobuf_assertions",
        "@rhutil//rhutil/testing:assertions",
        "@com_google_protobuf//:protobuf",
        "@googletest//:gtest_main",
        "@abseil//absl/strings:str_format",
    ],
)
