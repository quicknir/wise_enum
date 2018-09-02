# wise_enum

> Because reflection makes you wise, not smart

`wise_enum` is a standalone smart enum library for C++14 (or later). It supports
all of the standard functionality that you would expect from a smart enum class
in C++:
 - Tells you the number of enumerators
 - Lets you iterate over all enum values
 - Converts string to enum, and enum to string
 - Does everything in an idiomatic C++ way (friendly to generic programming,
   compile time programming, etc)

### Examples

Let's look at a bit of code. You can declare an enum like this:

```
// Equivalent to enum Color {GREEN = 2, RED};
WISE_ENUM(Color, (GREEN, 2), RED)
```

You can also declare an enum class instead of an enum, specify the storage
explicitly, declare an enum nested inside a class, or even adapt an already
declared enum:

```
// Equivalent to enum class MoreColor : int64_t {BLUE, BLACK = 1};
WISE_ENUM_CLASS((MoreColor, int64_t), BLUE, (BLACK, 1))

// Inside a class, must use a different macro, but still works
struct Bar {
    WISE_ENUM_MEMBER(Foo, BUZ)
};

// Adapt an existing enum you don't control so it works with generic code
namespace another_lib {
enum class SomebodyElse { FIRST, SECOND };
}
WISE_ENUM_ADAPT(another_lib::SomebodyElse, FIRST, SECOND)
```

You can ask the enum how many enumerators it has:

```
static_assert(wise_enum::size<Color> == 2, "");
```

Iterate over the enumerators:

```
std::cerr << "Enum values and names:\n";
for (auto e : wise_enum::range<Color>) {
  std::cerr << static_cast<int>(e.value) << " " << e.name << "\n";
}
```

Convert between strings and enums:

```
// Convert any enum to a string
std::cerr << wise_enum::to_string(Color::RED) << "\n";

// Convert any string to an optional<enum>
auto x1 = wise_enum::from_string<Color>("GREEN");
auto x2 = wise_enum::from_string<Color>("Greeeeeeen");

assert(x1.value() == Color::GREEN);
assert(!x2);
```

Check whether something is a wise enum at compile time:
```
static_assert(wise_enum::is_wise_enum_v<Color>, "");
static_assert(!wise_enum::is_wise_enum_v<int>, "");
enum flub { blub, glub };
static_assert(!wise_enum::is_wise_enum_v<flub>, "");
```

### Design

It has a few notable design choices.

First, when you use one of the macros to declare your enum, what gets declared
(among other things) is exactly the vanilla enum (or enum class) that you would
expect. Not an enum like class, or anything like that. That means that
`wise_enum`s can be used exactly like regular enums in non-reflective contexts,
because they are regular enums. They can be used as non-type template
parameters, and they can be used in switch case statements, unlike any user
defined type. This also means that upgrading a regular enum already widely used
(non-reflectively) in your codebase to a wise enum is never a breaking change.
No strange behavior, or edge cases, when used with other third party libraries
(e.g. serialization), or standard library type traits.

This first point is especially notable since [Better
Enums](https://github.com/aantron/better-enums) seems like the best maintained,
most polished, and most suggested smart enum solution I've seen. Last I checked,
it uses enum like classes, which carry all the disadvantages mentioned above.
This being a deal breaker for me was one of the major motivations for writing
this library. More generally, I recommend that if you are considering a smart
enum library, if this point isn't discussed in the docs, simply try to define an
enum and switch case on it, or define it as a non-type template parameter. If it
doesn't work, I'd reconsider.

Second, all the functionality in defining enums is preserved. You can define
`enum` or `enum class`es, set storage explicitly or let it be implicit, define
the value for an enumeration, or allow it to be determined implicitly. You can
also define enums nested in classes, which isn't supported in some smart enum
libraries.

Third, it's quite compile-time programming friendly. Everything is `constexpr`,
and a type trait is provided. This makes it easy to handle wise enums in a
specific way in generic code. For example, if you have a logger, it can't
intelligently log a vanilla enum without more information, but it can log a wise
enum, so you can use the type trait to handle them differently (with no
performance cost of course).

Fourth, it's careful with regards to performance and generated assembly. It
makes zero heap allocations and does zero dynamic initialization, and does not
use exceptions. The enum -> string is an optimal switch-case. String -> enum is
currently a linear search; this may be changed in the future (most alternatives
are not trivial to implement without doing heap allocations or dynamic
initialization).

### Requirements

wise_enum does require an `optional` like type. If compiled with 17 it will
automatically use `std::optional` but if you are compiling with 14, you'll need
to supply it an optional-like type by defining a macro. I may package an
optional implementation, or provide a non-optional API (it's used for the string
-> enum conversion), in the near future (depending as well on feedback).

### Limitations

There are some known limitations:

 - If there are enumerators with the same value, then `to_string` will not work.
   You can declare the enum and use all the other API. This is both because it
   doesn't jive at all with the implementation, and even conceptually it's not
   clear how you would handle a conversion to string since multiple strings
   would be associated with the same value.
 - By default, you are limited to 256 enumerations. If you need more, simply run
   the `create_generated` script to create a file with as many as you need, and
   replace `wise_enum_generated` with that file. The default limit may be raised
   or lowered based on feedback. An alternative solution here would be to create
   headers in advance that raise this number, but leave the onus on the user to
   include them (so users who don't need a large number aren't slowed down)

### Q&A

##### Why didn't I use `BOOST_PP`?
I started with it, but the limit on sequences was very disappointing (64) and
there didn't seem to be any easy way to change it. So then I started down the
codegen route, and once there, I wasn't using very much. I know there are always
people who prefer to avoid the dependency, so I decided to drop it.

##### What about C++11?
I'm not likely to add support myself, but I welcome pull requests. This code
will require quite a bit of attention to detail though, around the matter of
`constexpr`.
