#include <cppunit/Portability.h>

#if defined(__GNUC__)
# include <cxxabi.h>
# include <memory>
#endif // __GNUC__


CPPUNIT_NS_BEGIN


std::string demangle(const std::string& name)
{
#if defined(__GNUC__)
  // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
  // https://github.com/boostorg/core/blob/develop/include/boost/core/demangle.hpp
  int status = 0;

  const std::unique_ptr<char, decltype( ( std::free ) )> demangled =
  {
    abi::__cxa_demangle( name.c_str(),
                         NULL,
                         NULL,
                         &status ),
    std::free
  };

  return ( ( status == 0 && demangled.get() != nullptr )
               ? demangled.get() : name );
#else
  // clang-7 works fine without demangling
  // TODO: test on MSVC
  return name;
#endif // __GNUC__
}


CPPUNIT_NS_END
