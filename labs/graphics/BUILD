load("@rules_pkg//pkg:tar.bzl", "pkg_tar")
load("@rules_qt//:qt.bzl", "qt_cc_binary")

qt_cc_binary(
    name = "graphics",
    srcs = ["main.cpp"],
    deps = [
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_widgets",
    ],
)

pkg_tar(
    name = "release",
    srcs = [
        ":graphics",
    ],
    include_runfiles = True,
    package_dir = "graphics",
)
