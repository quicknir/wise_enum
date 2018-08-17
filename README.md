# wise_enum

> Because reflection makes you wise, not smart

`wise_enum` is a standalone smart enum library for C++14 (or later). It supports all of the standard functionality that you would expect from a smart enum class in C++:
 - Tells you the number of enumerators
 - Converts string to enum, and enum to string
 - Lets you iterate over all enum values
 
 - Does everything in an idiomatic C++ way (friendly to generic programming, compile time programming, etc)

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
the value for an enumeration, or allow it to be determined implicitly.

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

More documentation on the API is coming soon; for now consult the example code.
The API is quite small and simple.

There are some known limitations:
 - It can't be used inside a class, unfortunately. This probably cannot ever
 change. Suggested workaround is to declare it in a detail namespace, and then
 use an alias inside your class
 - By default, you are limited to 99 enumerations. If you need more, simply run
   the `create_generated` script to create a file with as many as you need, and
   replace `wise_enum_generated` with that file. The default limit may be raised
   or lowered based on feedback.
 - Because macros are macros, if/when feeding the value for an enumeration, it
   cannot have unparenthesized commas. This is actually relatively rare, but for
   example `f<3,4>()` is an example of something that could be a legal C++
   expression and contains an unparenthesized comma. For now you could simply
   assign that to a variable, and use the variable, as a workaround. This will
   be investigated for improvement (pull requests welcome)

 Why didn't I use `BOOST_PP`? I started with it, but the limit on sequences was
 very disappointing (64) and there dind't seem to be any easy way to change it.
 So then I started down the codegen route, and once there, I wasn't using very
 much. I know there are always people who prefer to avoid the dependency, so I
 decided to drop it.

 What about C++11? I'm not likely to add support myself, but I welcome pull
 requests. This code will require quite a bit of attention to detail though,
 around the matter of `constexpr`.

The library does require an `optional` like type. If compiled with 17 it will
automatically use `std::optional` but if you are compiling with 14, you'll need
to supply it an optional-like type by defining a macro. I may package an
optional implementation, or provide a non-optional API (it's used for the string
-> enum conversion), in the near future (depending as well on feedback)
