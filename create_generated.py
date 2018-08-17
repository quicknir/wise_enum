#!/usr/bin/env python

from itertools import groupby
import argparse


def _print_macro_line(f, s, terminal=False, cols=90):
    if len(s) >= cols - 2:
        raise RuntimeError("Too long line!")
    if terminal:
        f.write(s + "\n")
        return

    f.write(s + " " * (cols - 2 - len(s)) + "\\\n")


def _print_comma_list(f, args, indent, grouping_factor=10):
    args = list(args)
    args_comma = [x + "," for x in args[:-1]] + args[-1:]
    for _, line in groupby(
            enumerate(args_comma), lambda x: x[0] / grouping_factor):
        _print_macro_line(f, " " * indent + " ".join(x[1] for x in line))


def _print_macro_func(f, func_name, args):

    _print_macro_line(f, "#define " + func_name + "(")
    _print_comma_list(f, args, 4)
    _print_macro_line(f, "  )")


def main(num, filename):

    with open(filename, 'w') as f:

        _print_macro_func(f, "WISE_ENUM_IMPL_ARG_N",
                          ["_{}".format(i)
                           for i in range(1, num)] + ["N", "..."])
        _print_macro_line(f, "  N")
        f.write('\n')

        _print_macro_line(f, "#define WISE_ENUM_IMPL_RSEQ_N()")
        _print_comma_list(f, reversed(["{}".format(i) for i in range(num)]), 2)
        f.write('\n')

        for i in range(1, num):
            _print_macro_func(f, "WISE_ENUM_IMPL_{}".format(i),
                              ["type", "name", "storage"
                               ] + ["x{}".format(j) for j in range(i)])

            _print_macro_line(f, "  type name storage {")
            _print_comma_list(f, [
                "WISE_ENUM_IMPL_ENUM_INIT(x{})".format(j) for j in range(i)
            ], 4, 1)

            _print_macro_line(f, "  };")

            _print_macro_line(
                f,
                "  constexpr auto wise_enum_descriptor_pair_array(::wise_enum::detail::Tag<name>) {"
            )
            _print_macro_line(
                f, "    return std::array<std::pair<name, const char *>, " + str(i) + ">{{")

            _print_comma_list(f, [
                "WISE_ENUM_IMPL_DESC_PAIR(x{})".format(j) for j in range(i)
            ], 6, 1)

            _print_macro_line(f, "    }};")
            _print_macro_line(f, "  }")

            _print_macro_line(
                f, "  constexpr const char *wise_enum_to_string(name e) {")
            _print_macro_line(f, "    switch (e) {")
            for j in range(i):
                _print_macro_line(
                    f,
                    "      case name::WISE_ENUM_IMPL_ENUM(x{}): return WISE_ENUM_IMPL_ENUM_STR(x{});".
                    format(j, j))

            _print_macro_line(f, "    }")
            _print_macro_line(f, "    return nullptr;")
            _print_macro_line(f, "  }")
            f.write("\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=
        "Generate file needed for smart enums, up to some maximum number of enumerations"
    )
    parser.add_argument('num', type=int)
    parser.add_argument('output_filename')
    args = parser.parse_args()
    main(args.num, args.output_filename)
