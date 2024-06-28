cc_library(
  name = "tuple_helper",
  hdrs = ["tuple_helper.hpp"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = "function_operator",
  hdrs = ["function_operator.hpp"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = "list_base",
  hdrs = ["list_base.hpp"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "rbtree_base",
  hdrs = ["rbtree_base.hpp"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "list",
  hdrs = ["list.hpp"],
  deps = ["//src:list_base", "//src:tuple_helper"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "algorithm",
  hdrs = ["algorithm.hpp"],
  deps = ["//src:list_base", "//src:list", "//src:rbtree_base", "//src:function_operator"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "rbtree",
  hdrs = ["rbtree.hpp"],
  deps = ["//src:rbtree_base", "//src:list"],
  visibility = ["//visibility:public"],
)
cc_library(
  name = "printer",
  hdrs = ["printer.hpp"],
  deps = ["list_base", "rbtree_base", "algorithm"],
  visibility = ["//visibility:public"],
)
