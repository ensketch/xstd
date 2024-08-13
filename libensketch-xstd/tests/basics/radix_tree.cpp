#include <ensketch/xstd/meta/radix_tree.hpp>

using ensketch::xstd::meta::radix_tree;
using ensketch::xstd::meta::radix_tree_from;

namespace detail = ensketch::xstd::meta::detail;
using detail::radix_tree::leaf;
using detail::radix_tree::node;
using detail::radix_tree::node_list;

static_assert(radix_tree<>{} == radix_tree<node<"">>{});
static_assert(radix_tree_from<>() == radix_tree<>{});

static_assert(radix_tree_from<"">() == radix_tree<leaf<"">>{});

static_assert(radix_tree_from<"help">() == radix_tree<  //
                                               node<"",
                                                    node_list<  //
                                                        leaf<"help">>>>{});

static_assert(radix_tree_from<"help", "help">() ==
              radix_tree<  //
                  node<"",
                       node_list<  //
                           leaf<"help">>>>{});

static_assert(radix_tree_from<"help", "">() == radix_tree<  //
                                                   leaf<"",
                                                        node_list<  //
                                                            leaf<"help">>>>{});

static_assert(radix_tree_from<"help", "helo">() ==  //
              radix_tree<                           //
                  node<"",
                       node_list<  //
                           node<"hel",
                                node_list<      //
                                    leaf<"o">,  //
                                    leaf<"p">>>>>>{});

static_assert(radix_tree_from<"help", "version">() ==  //
              radix_tree<                              //
                  node<"",
                       node_list<         //
                           leaf<"help">,  //
                           leaf<"version">>>>{});

static_assert(radix_tree_from<"help", "hel">() ==  //
              radix_tree<                          //
                  node<"",
                       node_list<  //
                           leaf<"hel",
                                node_list<  //
                                    leaf<"p">>>>>>{});

static_assert(radix_tree_from<"help",
                              "hello",
                              "version",
                              "verbose",
                              "very",
                              "in",
                              "input",
                              "out",
                              "output">() ==  //
              radix_tree<node<"",
                              node_list<  //
                                  node<"hel",
                                       node_list<        //
                                           leaf<"lo">,   //
                                           leaf<"p">>>,  //
                                  leaf<"in",
                                       node_list<          //
                                           leaf<"put">>>,  //
                                  leaf<"out",
                                       node_list<          //
                                           leaf<"put">>>,  //
                                  node<"ver",
                                       node_list<         //
                                           leaf<"bose">,  //
                                           leaf<"sion">,  //
                                           leaf<"y">>>    //
                                  >>>{});
