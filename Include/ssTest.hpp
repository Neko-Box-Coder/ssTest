//=======================================================================
// termcolor license (at Master b3cb0f365f8435588df7a6b12a82b2ac5fc1fe95) (Not ssTest, see LICENSE)
//=======================================================================

//Copyright (c) 2013, Ihor Kalnytskyi.
//All rights reserved.

//Redistribution and use in source and binary forms of the software as well
//as documentation, with or without modification, are permitted provided
//that the following conditions are met:

//* Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.

//* Redistributions in binary form must reproduce the above
//  copyright notice, this list of conditions and the following
//  disclaimer in the documentation and/or other materials provided
//  with the distribution.

//* The names of the contributors may not be used to endorse or
//  promote products derived from this software without specific
//  prior written permission.

//THIS SOFTWARE AND DOCUMENTATION IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
//NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
//OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE AND DOCUMENTATION, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//DAMAGE.

// =======================================================================
//termcolor starts
// =======================================================================

    //!
    //! termcolor
    //! ~~~~~~~~~
    //!
    //! termcolor is a header-only c++ library for printing colored messages
    //! to the terminal. Written just for fun with a help of the Force.
    //!
    //! :copyright: (c) 2013 by Ihor Kalnytskyi
    //! :license: BSD, see LICENSE for details
    //!

    #ifndef TERMCOLOR_HPP_
    #define TERMCOLOR_HPP_

    #include <cstdint>
    #include <iostream>

    // Detect target's platform and set some macros in order to wrap platform
    // specific code this library depends on.
    #if defined(_WIN32) || defined(_WIN64)
    #   define TERMCOLOR_TARGET_WINDOWS
    #elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    #   define TERMCOLOR_TARGET_POSIX
    #endif

    // If implementation has not been explicitly set, try to choose one based on
    // target platform.
    #if !defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES) && !defined(TERMCOLOR_USE_WINDOWS_API) && !defined(TERMCOLOR_USE_NOOP)
    #   if defined(TERMCOLOR_TARGET_POSIX)
    #       define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
    #       define TERMCOLOR_AUTODETECTED_IMPLEMENTATION
    #   elif defined(TERMCOLOR_TARGET_WINDOWS)
    #       define TERMCOLOR_USE_WINDOWS_API
    #       define TERMCOLOR_AUTODETECTED_IMPLEMENTATION
    #   endif
    #endif

    // These headers provide isatty()/fileno() functions, which are used for
    // testing whether a standard stream refers to the terminal.
    #if defined(TERMCOLOR_TARGET_POSIX)
    #   include <unistd.h>
    #elif defined(TERMCOLOR_TARGET_WINDOWS)
    #   include <io.h>
    #   include <windows.h>
    #endif


    namespace termcolor
    {
        // Forward declaration of the `_internal` namespace.
        // All comments are below.
        namespace _internal
        {
            inline int colorize_index();
            inline FILE* get_standard_stream(const std::ostream& stream);
            inline FILE* get_standard_stream(const std::wostream& stream);
            template <typename CharT>
            bool is_colorized(std::basic_ostream<CharT>& stream);
            template <typename CharT>
            bool is_atty(const std::basic_ostream<CharT>& stream);

        #if defined(TERMCOLOR_TARGET_WINDOWS)
            template <typename CharT>
            void win_change_attributes(std::basic_ostream<CharT>& stream, int foreground, int background = -1);
        #endif
        }

        template <typename CharT>
        std::basic_ostream<CharT>& colorize(std::basic_ostream<CharT>& stream)
        {
            stream.iword(_internal::colorize_index()) = 1L;
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& nocolorize(std::basic_ostream<CharT>& stream)
        {
            stream.iword(_internal::colorize_index()) = 0L;
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& reset(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[00m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1, -1);
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bold(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[1m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& dark(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[2m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& italic(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[3m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& underline(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[4m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1, COMMON_LVB_UNDERSCORE);
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& blink(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[5m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& reverse(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[7m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& concealed(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[8m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& crossed(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[9m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }
        
        template <uint8_t code, typename CharT>
        std::basic_ostream<CharT>& color(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[38;5;" << +code << "m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <uint8_t code, typename CharT>
        std::basic_ostream<CharT>& on_color(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[48;5;" << +code << "m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <uint8_t r, uint8_t g, uint8_t b, typename CharT>
        std::basic_ostream<CharT>& color(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[38;2;" << +r << ";" << +g << ";" << +b << "m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <uint8_t r, uint8_t g, uint8_t b, typename CharT>
        std::basic_ostream<CharT>& on_color(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[48;2;" << +r << ";" << +g << ";" << +b << "m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& grey(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[30m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    0   // grey (black)
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& red(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[31m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_RED
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& green(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[32m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_GREEN
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& yellow(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[33m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_GREEN | FOREGROUND_RED
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& blue(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[34m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& magenta(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[35m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_RED
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& cyan(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[36m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_GREEN
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& white(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[37m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
                );
            #endif
            }
            return stream;
        }


        template <typename CharT>
        std::basic_ostream<CharT>& bright_grey(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[90m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    0 | FOREGROUND_INTENSITY   // grey (black)
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_red(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[91m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_RED | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_green(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[92m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_GREEN | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_yellow(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[93m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_blue(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[94m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_magenta(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[95m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_cyan(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[96m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& bright_white(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[97m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream,
                    FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }


        template <typename CharT>
        std::basic_ostream<CharT>& on_grey(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[40m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    0   // grey (black)
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_red(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[41m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_RED
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_green(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[42m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_yellow(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[43m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_RED
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_blue(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[44m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_BLUE
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_magenta(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[45m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_BLUE | BACKGROUND_RED
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_cyan(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[46m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_BLUE
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_white(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[47m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED
                );
            #endif
            }

            return stream;
        }


        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_grey(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[100m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    0 | BACKGROUND_INTENSITY   // grey (black)
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_red(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[101m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_RED | BACKGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_green(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[102m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_yellow(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[103m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_blue(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[104m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_BLUE | BACKGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_magenta(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[105m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_cyan(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[106m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
                );
            #endif
            }
            return stream;
        }

        template <typename CharT>
        std::basic_ostream<CharT>& on_bright_white(std::basic_ostream<CharT>& stream)
        {
            if (_internal::is_colorized(stream))
            {
            #if defined(TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES)
                stream << "\033[107m";
            #elif defined(TERMCOLOR_USE_WINDOWS_API)
                _internal::win_change_attributes(stream, -1,
                    BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY
                );
            #endif
            }

            return stream;
        }



        //! Since C++ hasn't a way to hide something in the header from
        //! the outer access, I have to introduce this namespace which
        //! is used for internal purpose and should't be access from
        //! the user code.
        namespace _internal
        {
            // An index to be used to access a private storage of I/O streams. See
            // colorize / nocolorize I/O manipulators for details. Due to the fact
            // that static variables ain't shared between translation units, inline
            // function with local static variable is used to do the trick and share
            // the variable value between translation units.
            inline int colorize_index()
            {
                static int colorize_index = std::ios_base::xalloc();
                return colorize_index;
            }

            //! Since C++ hasn't a true way to extract stream handler
            //! from the a given `std::ostream` object, I have to write
            //! this kind of hack.
            inline
            FILE* get_standard_stream(const std::ostream& stream)
            {
                if (&stream == &std::cout)
                    return stdout;
                else if (&stream == &std::cerr || &stream == &std::clog)
                    return stderr;

                return nullptr;
            }

            //! Since C++ hasn't a true way to extract stream handler
            //! from the a given `std::wostream` object, I have to write
            //! this kind of hack.
            inline
            FILE* get_standard_stream(const std::wostream& stream)
            {
                if (&stream == &std::wcout)
                    return stdout;
                else if (&stream == &std::wcerr || &stream == &std::wclog)
                    return stderr;

                return nullptr;
            }

            // Say whether a given stream should be colorized or not. It's always
            // true for ATTY streams and may be true for streams marked with
            // colorize flag.
            template <typename CharT>
            bool is_colorized(std::basic_ostream<CharT>& stream)
            {
                return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index()));
            }

            //! Test whether a given `std::ostream` object refers to
            //! a terminal.
            template <typename CharT>
            bool is_atty(const std::basic_ostream<CharT>& stream)
            {
                FILE* std_stream = get_standard_stream(stream);

                // Unfortunately, fileno() ends with segmentation fault
                // if invalid file descriptor is passed. So we need to
                // handle this case gracefully and assume it's not a tty
                // if standard stream is not detected, and 0 is returned.
                if (!std_stream)
                    return false;

            #if defined(TERMCOLOR_TARGET_POSIX)
                return ::isatty(fileno(std_stream));
            #elif defined(TERMCOLOR_TARGET_WINDOWS)
                return ::_isatty(_fileno(std_stream));
            #else
                return false;
            #endif
            }

        #if defined(TERMCOLOR_TARGET_WINDOWS)
        
            //! same hack as used in get_standard_stream function, but for Windows with `std::ostream`
            inline HANDLE get_terminal_handle(std::ostream& stream)
            {
                if (&stream == &std::cout)
                    return GetStdHandle(STD_OUTPUT_HANDLE);
                else if (&stream == &std::cerr || &stream == &std::clog)
                    return GetStdHandle(STD_ERROR_HANDLE);
                return nullptr;
            }
            
            //! same hack as used in get_standard_stream function, but for Windows with `std::wostream`
            inline HANDLE get_terminal_handle(std::wostream& stream)
            {
                if (&stream == &std::wcout)
                    return GetStdHandle(STD_OUTPUT_HANDLE);
                else if (&stream == &std::wcerr || &stream == &std::wclog)
                    return GetStdHandle(STD_ERROR_HANDLE);
                return nullptr;
            }
            
            //! Change Windows Terminal colors attribute. If some
            //! parameter is `-1` then attribute won't changed.
            template <typename CharT>
            void win_change_attributes(std::basic_ostream<CharT>& stream, int foreground, int background)
            {
                // yeah, i know.. it's ugly, it's windows.
                static WORD defaultAttributes = 0;

                // Windows doesn't have ANSI escape sequences and so we use special
                // API to change Terminal output color. That means we can't
                // manipulate colors by means of "std::stringstream" and hence
                // should do nothing in this case.
                if (!_internal::is_atty(stream))
                    return;

                // get terminal handle
                HANDLE hTerminal = INVALID_HANDLE_VALUE;
                hTerminal = get_terminal_handle(stream);

                // save default terminal attributes if it unsaved
                if (!defaultAttributes)
                {
                    CONSOLE_SCREEN_BUFFER_INFO info;
                    if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                        return;
                    defaultAttributes = info.wAttributes;
                }

                // restore all default settings
                if (foreground == -1 && background == -1)
                {
                    SetConsoleTextAttribute(hTerminal, defaultAttributes);
                    return;
                }

                // get current settings
                CONSOLE_SCREEN_BUFFER_INFO info;
                if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                    return;

                if (foreground != -1)
                {
                    info.wAttributes &= ~(info.wAttributes & 0x0F);
                    info.wAttributes |= static_cast<WORD>(foreground);
                }

                if (background != -1)
                {
                    info.wAttributes &= ~(info.wAttributes & 0xF0);
                    info.wAttributes |= static_cast<WORD>(background);
                }

                SetConsoleTextAttribute(hTerminal, info.wAttributes);
            }
        #endif // TERMCOLOR_TARGET_WINDOWS

        } // namespace _internal

    } // namespace termcolor


    #undef TERMCOLOR_TARGET_POSIX
    #undef TERMCOLOR_TARGET_WINDOWS

    #if defined(TERMCOLOR_AUTODETECTED_IMPLEMENTATION)
    #   undef TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
    #   undef TERMCOLOR_USE_WINDOWS_API
    #endif

    #endif // TERMCOLOR_HPP_

// =======================================================================
//termcolor ends
// =======================================================================



#ifndef SSTEST_BASE_H
#define SSTEST_BASE_H

#include <exception>

// =======================================================================
// Macros for allowing overloadable Macro functions
// =======================================================================

// https://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments
#define INTERNAL_ssTEST_CAT( A, B ) A ## B
#define INTERNAL_ssTEST_SELECT( NAME, NUM ) INTERNAL_ssTEST_CAT( NAME ## _, NUM )
#define INTERNAL_ssTEST_COMPOSE( NAME, ARGS ) NAME ARGS

#define INTERNAL_ssTEST_GET_COUNT( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11 /* ad nauseam */, COUNT, ... ) COUNT
#define INTERNAL_ssTEST_EXPAND() ,,,,,,,,,,, // 6 commas (or 7 empty tokens)
#define INTERNAL_ssTEST_VA_SIZE( ... ) INTERNAL_ssTEST_COMPOSE( INTERNAL_ssTEST_GET_COUNT, (INTERNAL_ssTEST_EXPAND __VA_ARGS__ (), 0, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1) )

#ifndef _MSC_VER
#define INTERNAL_ssTEST_VA_SELECT( NAME, ... ) INTERNAL_ssTEST_SELECT( NAME, INTERNAL_ssTEST_VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
#else

//MSVC workaround: https://stackoverflow.com/questions/48710758/how-to-fix-variadic-macro-related-issues-with-macro-overloading-in-msvc-mic
//This is honestly retarded.
#define INTERNAL_ssTEST_VA_ARGS_FIX( macro, args ) macro args
#define INTERNAL_ssTEST_VA_SELECT( NAME, ... ) INTERNAL_ssTEST_VA_ARGS_FIX(INTERNAL_ssTEST_SELECT, ( NAME, INTERNAL_ssTEST_VA_SIZE( __VA_ARGS__ ) )) (__VA_ARGS__)
#endif


#include <string>
#include <functional>
#include <iostream>
#include <vector>


#define INTERNAL_ssTEST_FILE_NAME()\
[]()\
{\
    std::string ssTestfileName = __FILE__;\
    std::size_t ssTestfound = ssTestfileName.find_last_of("/\\");\
    std::size_t ssTestExtfound = ssTestfileName.rfind(".c");\
    return ssTestfileName.substr(ssTestfound+1, ssTestExtfound - ssTestfound - 1);\
}()

#ifdef _WIN32
    #include <windows.h>
    #define INTERNAL_ssTEST_TITLE(title)\
    SetConsoleOutputCP(CP_UTF8);\
    ssTestName = title;\
    std::cout << "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" << std::endl;\
    std::cout << "⏷︎ " << title.c_str() << std::endl;\
    std::cout << "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" << std::endl << std::endl;
#else
    #define INTERNAL_ssTEST_TITLE(title)\
    ssTestName = title;\
    std::cout << "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" << std::endl;\
    std::cout << "⏷︎ " << title.c_str() << std::endl;\
    std::cout << "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" << std::endl << std::endl;
#endif

#define ssTEST_SET_UP ssTestSetUp = [&]()
#define ssTEST_CLEAN_UP ssTestCleanUp = [&]()

#define ssTEST_CALL_SET_UP() ssTestSetUp()
#define ssTEST_CALL_CLEAN_UP() ssTestCleanUp()

#define ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS()\
;ssTestResetBetweenTests = false;

//#define ssTEST( ... ) INTERNAL_ssTEST_VA_SELECT(ssTEST, __VA_ARGS__)

#define ssTEST(name)\
ssTestFunctions.resize(ssTestFunctions.size()+1);\
ssTestFunctionsNames.push_back(name);\
ssTestFunctionsSkipFlags.push_back(false);\
ssTestFunctions[ssTestFunctions.size() - 1] = [&]()

#define ssTEST_SKIP(name)\
ssTestFunctions.resize(ssTestFunctions.size()+1);\
ssTestFunctionsNames.push_back(name);\
ssTestFunctionsSkipFlags.push_back(true);\
ssTestFunctions[ssTestFunctions.size() - 1] = [&]()

#define ssTEST_ONLY_THIS(name)\
ssTestFunctions.clear(); \
ssTestFunctionsNames.clear(); \
ssTestFunctionsSkipFlags.clear(); \
ssTestFunctions.resize(ssTestFunctions.size()+1);\
ssTestFunctionsNames.push_back(name);\
ssTestFunctionsSkipFlags.push_back(false);\
ssTestExcludeOthers = true; \
ssTestFunctions[ssTestFunctions.size() - 1] = [&]()

#define ssTEST_OUTPUT_SETUP( setup ) \
    std::cout << "    - Setting up: " << std::endl;\
    std::cout << "      \"" << #setup << "\"" << std::endl;\
    setup \
    std::cout << std::endl

#define ssTEST_OUTPUT_EXECUTION( execution ) \
    std::cout << "    - Executing: " << std::endl;\
    std::cout << "      \"" << #execution << "\"" << std::endl;\
    execution \
    std::cout << std::endl

#define ssTEST_OUTPUT_ASSERT( ... ) do{ INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_ASSERT, __VA_ARGS__ ) } while(0)

#define INTERNAL_ssTEST_OUTPUT_ASSERT_1(assert) INTERNAL_ssTEST_OUTPUT_ASSERT_2("", assert)

#define INTERNAL_ssTEST_OUTPUT_ASSERT_2(info, assert)\
{\
    try\
    {\
        if(std::string(info).empty())\
            std::cout << "    - Assertion Starts: " << std::endl;\
        else\
            std::cout << "    - Assertion Starts (" << info << "):" << std::endl;\
        \
        std::cout << "      Asserting: \"" << #assert << "\"" << std::endl;\
        bool ssTest_Internal_result = false;\
        {\
            ssTest_Internal_result = assert;\
        }\
        \
        if(ssTest_Internal_result)\
        {\
            std::cout << "      " << termcolor::green << "Assertion Passed ✓" << termcolor::reset << std::endl << std::endl;\
            ssTestSuccess++;\
        }\
        else\
        {\
            std::cout << "      " << termcolor::red << "Assertion Failed ✕" << termcolor::reset << std::endl << std::endl;\
            ssTestFailed++;\
        }\
    }\
    catch(std::exception& ssTest_except)\
    {\
        std::cout << "      " << termcolor::red << "Error Catched: " << ssTest_except.what() << std::endl << std::endl;\
        ssTestFailed++;\
    }\
    catch(...)\
    {\
        std::cout << "      " << termcolor::red << "Error occured" << std::endl << std::endl;\
        ssTestFailed++;\
        throw;\
    }\
}

#define ssTEST_OUTPUT_SKIP( ... ) do { INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_SKIP, __VA_ARGS__ ) } while(0)

#define INTERNAL_ssTEST_OUTPUT_SKIP_0() INTERNAL_ssTEST_OUTPUT_SKIP_1("")

#define INTERNAL_ssTEST_OUTPUT_SKIP_1(assert)\
{\
    std::cout << "    - Assertion Starts: " << std::endl;\
    std::cout << "      Skipping: \"" << #assert << "\"" << std::endl;\
    std::cout << "      " << termcolor::yellow << "Assertion Skipped ⏸︎" << termcolor::reset << std::endl << std::endl;\
}

#define INTERNAL_ssTEST_OUTPUT_SKIP_2(info, assert)\
{\
    if(std::string(info).empty())\
        std::cout << "    - Assertion Starts: " << std::endl;\
    else\
        std::cout << "    - Assertion Starts (" << info << "):" << std::endl;\
    \
    std::cout << "      Skipping: \"" << #assert << "\"" << std::endl;\
    std::cout << "      " << termcolor::yellow << "Assertion Skipped ⏸︎" << termcolor::reset << std::endl << std::endl;\
}

#define ssTEST_INIT()\
{\
    int ssTestSuccess = 0;\
    int ssTestFailed = 0;\
    std::string ssTestName = "";\
    std::vector<std::function<void()>> ssTestFunctions;\
    std::vector<std::string> ssTestFunctionsNames;\
    std::vector<bool> ssTestFunctionsSkipFlags;\
    int ssTestCurrentTestIndex = 0;\
    bool ssTestSetUpCalled = false;\
    bool ssTestResetBetweenTests = true;\
    std::function<void()> ssTestSetUp = [](){};\
    std::function<void()> ssTestCleanUp = [](){};\
    bool ssTestExcludeOthers = false; \
    try\
    {

#define ssTEST_END()\
        INTERNAL_ssTEST_TITLE(INTERNAL_ssTEST_FILE_NAME());\
        if(!ssTestResetBetweenTests)\
            ssTestSetUp();\
        for(int i = 0; i < ssTestFunctions.size(); i++)\
        {\
            ssTestCurrentTestIndex = i;\
            std::cout << "⏵︎ Running \"" << ssTestFunctionsNames[ssTestCurrentTestIndex] << "\":" << std::endl;\
            if(ssTestFunctionsSkipFlags[i])\
            {\
                std::cout << termcolor::yellow << "      --> Test Skipped ⏸︎  <--" << termcolor::reset << std::endl << std::endl;\
                continue;\
            }\
            else\
            {\
                if(ssTestResetBetweenTests)\
                    ssTestSetUp();\
                \
                ssTestFunctions[i]();\
                \
                if(ssTestResetBetweenTests)\
                    ssTestCleanUp();\
            }\
            if(ssTestExcludeOthers) \
                break; \
        }\
        if(!ssTestResetBetweenTests)\
            ssTestCleanUp();\
        int ssTestTotal = ssTestSuccess + ssTestFailed;\
        std::cout << std::endl << "Results:" << std::endl;\
        std::cout << ssTestSuccess << "/" << ssTestTotal << " assertions passed" << std::endl;\
        if(ssTestFailed > 0)\
        {\
            std::cout << termcolor::red << ssTestName << " has failed some assertions ✕" << termcolor::reset << std::endl << std::endl;\
            return EXIT_FAILURE;\
        }\
        else\
        {\
            std::cout << termcolor::green << ssTestName << " has passed all assertions ✓" << termcolor::reset << std::endl << std::endl;\
            return EXIT_SUCCESS;\
        }\
    }\
    catch(const std::exception& e)\
    {\
        std::cout << "Exception Caught: " << e.what() << std::endl;\
        throw;\
    }\
    catch(...)\
    {\
        std::cout << "Unknown Exception Caught" << std::endl;\
        throw;\
    }\
}

#endif