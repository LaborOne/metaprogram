cc_binary(
  name = "test_list",
  srcs = ["test_list.cc"],
  deps = ["//src:printer", "//src:list"],
  copts = ["-Werror", "-O3", "-ftemplate-depth=1000000"]
)
cc_binary(
  name = "test_tree",
  srcs = ["test_tree.cc"],
  deps = ["//src:printer", "//src:tree", "//src:list"],
  copts = ["-Werror", "-O3", "-ftemplate-depth=1000000"]
)
cc_binary(
  name = "test_sort",
  srcs = ["test_sort.cc"],
  deps = ["//src:printer", "//src:list", "//src:algorithm"],
  copts = ["-Werror", "-O3", "-ftemplate-depth=1000000"]
)
cc_binary(
  name = "test_functor",
  srcs = ["test_functor.cc"],
  deps = ["//src:printer", "//src:function_operator", "//src:algorithm"],
  copts = ["-Werror", "-O3", "-ftemplate-depth=1000000"]
)
