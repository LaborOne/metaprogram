cc_library(
  name = "tuple_helper",
  hdrs = ["tuple_helper.hpp"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = "list_base",
  hdrs = ["list_base.hpp"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "tree_base",
  hdrs = ["tree_base.hpp"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "list",
  hdrs = ["list.hpp"],
  deps = ["//src:list_base", "//src:tuple_helper"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = "tree",
  hdrs = ["tree.hpp"],
  deps = ["//src:tree_base", "//src:list"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = "printer",
  hdrs = ["printer.hpp"],
  deps = ["list_base", "tree_base"],
  visibility = ["//visibility:public"],
)
