#include <ensketch/xstd/static_radix_tree.hpp>

using ensketch::xstd::static_radix_tree;
using ensketch::xstd::static_radix_tree_from;

namespace detail = ensketch::xstd::detail;
using detail::static_radix_tree::leaf;
using detail::static_radix_tree::node;
using detail::static_radix_tree::node_list;

static_assert(static_radix_tree<>{} == static_radix_tree<node<"">>{});
static_assert(static_radix_tree_from<>() == static_radix_tree<>{});

static_assert(static_radix_tree_from<"">() == static_radix_tree<leaf<"">>{});

static_assert(static_radix_tree_from<"help">() ==
              static_radix_tree<       //
                  node<"", node_list<  //
                               leaf<"help">>>>{});

static_assert(static_radix_tree_from<"help", "help">() ==
              static_radix_tree<       //
                  node<"", node_list<  //
                               leaf<"help">>>>{});

static_assert(static_radix_tree_from<"help", "">() ==
              static_radix_tree<       //
                  leaf<"", node_list<  //
                               leaf<"help">>>>{});

static_assert(static_radix_tree_from<"help", "helo">() ==  //
              static_radix_tree<                           //
                  node<"", node_list<                      //
                               node<"hel", node_list<      //
                                               leaf<"o">,  //
                                               leaf<"p">>>>>>{});

static_assert(static_radix_tree_from<"help", "version">() ==  //
              static_radix_tree<                              //
                  node<"", node_list<                         //
                               leaf<"help">,                  //
                               leaf<"version">>>>{});

static_assert(static_radix_tree_from<"help", "hel">() ==  //
              static_radix_tree<                          //
                  node<"", node_list<                     //
                               leaf<"hel", node_list<     //
                                               leaf<"p">>>>>>{});

static_assert(
    static_radix_tree_from<"help", "hello", "version", "verbose", "very", "in",
                           "input", "out", "output">() ==          //
    static_radix_tree<node<"", node_list<                          //
                                   node<"hel", node_list<          //
                                                   leaf<"lo">,     //
                                                   leaf<"p">>>,    //
                                   leaf<"in", node_list<           //
                                                  leaf<"put">>>,   //
                                   leaf<"out", node_list<          //
                                                   leaf<"put">>>,  //
                                   node<"ver", node_list<          //
                                                   leaf<"bose">,   //
                                                   leaf<"sion">,   //
                                                   leaf<"y">>>     //
                                   >>>{});
