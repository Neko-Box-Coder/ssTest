//=======================================================================
//termcolor license (at Master b3cb0f365f8435588df7a6b12a82b2ac5fc1fe95) (Not ssTest, see LICENSE)
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

//=======================================================================
//termcolor starts
//=======================================================================

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

//=======================================================================
//termcolor ends
//=======================================================================



#ifndef SSTEST_BASE_HPP
#define SSTEST_BASE_HPP

#include <exception>

//=======================================================================
//Macros for allowing overloadable Macro functions
//=======================================================================

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

#define INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS( ... ) \
    INTERNAL_ssTEST_COMPOSE \
    ( \
        INTERNAL_ssTEST_GET_COUNT, \
        ( \
            INTERNAL_ssTEST_EXPAND __VA_ARGS__ (), \
            INTERNAL_ssTEST_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, \
            INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, \
            INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY, INTERNAL_ssTEST_NOT_EMPTY \
        ) \
    )(__VA_ARGS__)

#define INTERNAL_ssTEST_EMPTY()

#define INTERNAL_ssTEST_NOT_EMPTY(...) , __VA_ARGS__

#define INTERNAL_ssTEST_DELAY_STRINGIFY(x) INTERNAL_ssTEST_COMPOSE(INTERNAL_ssTEST_STRINGIFY, (x))
#define INTERNAL_ssTEST_STRINGIFY(x) #x

#include <string>
#include <functional>
#include <iostream>
#include <vector>

#define ssCOUT std::cout << ssTest_Status.ssTest_Indent

#define INTERNAL_ssTEST_OUTPUT_FORMATTED_CODE(indentation, code) \
    Internal_ssTest::OutputFormattedCode(indentation, INTERNAL_ssTEST_DELAY_STRINGIFY(code), __LINE__)

#define INTERNAL_ssTEST_OUTPUT_OPTIONAL_TEXT() \
    if(ssTest_Status.ssTest_RunningOptional) std::cout << termcolor::yellow << " (Optional) " << termcolor::reset

#define INTERNAL_ssTEST_OUTPUT_ASSERT_STARTS(info) \
    OutputAssertStarts(ssTest_Status, info)

#define INTERNAL_ssTEST_OUTPUT_CATCH_ERROR(assertExpression) \
    catch(std::exception& ssTestExcept) \
    { \
        Internal_ssTest::OutputExceptionError(ssTest_Status, assertExpression, ssTestExcept); \
    } \
    catch(...) \
    { \
        Internal_ssTest::OutputUnknownError(ssTest_Status, assertExpression); \
    } \
    do {} while(0)

#define INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED_0() 

#define INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED_1(_1) \
    if(!ssTest_Status.ssTest_LastTestResult) \
    { \
        ssCOUT << termcolor::red << "|     Assertion Failed Values: " << std::endl; \
        ssCOUT <<   "|          " << #_1 << " = " << _1 << termcolor::reset << std::endl; \
        ssCOUT << "|" << std::endl; \
    }

#define INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED_2(_1, _2) \
    if(!ssTest_Status.ssTest_LastTestResult) \
    { \
        ssCOUT << termcolor::red << "|     Assertion Failed Values: " << std::endl; \
        ssCOUT <<   "|          " << #_1 << " = " << _1 << std::endl; \
        ssCOUT <<   "|          " << #_2 << " = " << _2 << termcolor::reset << std::endl; \
        ssCOUT << "|" << std::endl; \
    }

#define INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED_3(_1, _2, _3) \
    if(!ssTest_Status.ssTest_LastTestResult) \
    { \
        ssCOUT << termcolor::red << "|     Assertion Failed Values: " << std::endl; \
        ssCOUT <<   "|          " << #_1 << " = " << _1 << std::endl; \
        ssCOUT <<   "|          " << #_2 << " = " << _2 << std::endl; \
        ssCOUT <<   "|          " << #_3 << " = " << _3 << termcolor::reset << std::endl; \
        ssCOUT << "|" << std::endl; \
    }

#define INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED_4(_1, _2, _3, _4) \
    if(!ssTest_Status.ssTest_LastTestResult) \
    { \
        ssCOUT << termcolor::red << "|     Assertion Failed Values: " << std::endl; \
        ssCOUT <<   "|          " << #_1 << " = " << _1 << std::endl; \
        ssCOUT <<   "|          " << #_2 << " = " << _2 << std::endl; \
        ssCOUT <<   "|          " << #_3 << " = " << _3 << std::endl; \
        ssCOUT <<   "|          " << #_4 << " = " << _4 << termcolor::reset << std::endl; \
        ssCOUT << "|" << std::endl; \
    }

#define INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED_5(_1, _2, _3, _4, _5) \
    if(!ssTest_Status.ssTest_LastTestResult) \
    { \
        ssCOUT << termcolor::red << "|     Assertion Failed Values: " << std::endl; \
        ssCOUT <<   "|          " << #_1 << " = " << _1 << std::endl; \
        ssCOUT <<   "|          " << #_2 << " = " << _2 << std::endl; \
        ssCOUT <<   "|          " << #_3 << " = " << _3 << std::endl; \
        ssCOUT <<   "|          " << #_4 << " = " << _4 << std::endl; \
        ssCOUT <<   "|          " << #_5 << " = " << _5 << termcolor::reset << std::endl; \
        ssCOUT << "|" << std::endl; \
    }

#define INTERNAL_ssTEST_OUTPUT_ASSERT_2(info, assertExpression) \
    Internal_ssTest::OutputAssert(  ssTest_Status, \
                                    info, \
                                    __LINE__, \
                                    #assertExpression, \
                                    [&](){return assertExpression;});

#define INTERNAL_ssTEST_OUTPUT_ASSERT_1(assertExpression) \
    INTERNAL_ssTEST_OUTPUT_ASSERT_2("", assertExpression)

#define INTERNAL_ssTEST_OUTPUT_ASSERT_4(info, assertValue, expectedValue, operator) \
    { \
        auto ssTestAssertValue = assertValue; \
        auto ssTestExpectedValue = expectedValue; \
        \
        Internal_ssTest::OutputAssertWithOperator(  ssTest_Status, \
                                                    info, \
                                                    __LINE__, \
                                                    ssTestAssertValue, \
                                                    ssTestExpectedValue, \
                                                    #assertValue, \
                                                    #expectedValue, \
                                                    #operator, \
                                                    [&](){return ssTestAssertValue operator ssTestExpectedValue;}); \
    }

#define INTERNAL_ssTEST_OUTPUT_ASSERT_3(info, assertValue, expectedValue) \
    INTERNAL_ssTEST_OUTPUT_ASSERT_4(info, assertValue, expectedValue, ==)

#define INTERNAL_ssTEST_OUTPUT_SKIP_2(info, assert) \
    Internal_ssTest::OutputSkip(ssTest_Status, \
                                info, \
                                __LINE__, \
                                #assert);

#define INTERNAL_ssTEST_OUTPUT_SKIP_1(assert) \
    INTERNAL_ssTEST_OUTPUT_SKIP_2("", assert)

#define INTERNAL_ssTEST_OUTPUT_SKIP_3(info, assertValue, expectedValue) \
    INTERNAL_ssTEST_OUTPUT_SKIP_4(info, assertValue, expectedValue, ==)

#define INTERNAL_ssTEST_OUTPUT_SKIP_4(info, assertValue, expectedValue, operator) \
    Internal_ssTest::OutputSkip(ssTest_Status, \
                                info, \
                                __LINE__, \
                                std::string(#assertValue) + " " + std::string(#operator) + " " + \
                                std::string(#expectedValue));

namespace Internal_ssTest
{
    inline std::string GetFileName(std::string filepath)
    {
        std::string ssTestfileName = filepath;
        std::size_t ssTestfound = ssTestfileName.find_last_of("/\\");
        if(ssTestfound == std::string::npos)
        {
            ssTestfound = ssTestfileName.find_last_of("\\");
            if(ssTestfound == std::string::npos)
                ssTestfound = 0;
        }
        std::size_t ssTestExtfound = ssTestfileName.rfind(".c");
        if(ssTestExtfound == std::string::npos)
        {
            ssTestExtfound = ssTestfileName.rfind(".h");
            if(ssTestExtfound == std::string::npos)
                ssTestExtfound = ssTestfileName.size();
        }
        return ssTestfileName.substr(  ssTestfound == 0 ? 0 : ssTestfound + 1,
                                        ssTestfound == 0 ? ssTestExtfound - ssTestfound :
                                                            ssTestExtfound - ssTestfound - 1);
    }

    inline std::string GetFileExt(std::string filepath)
    {
        std::string ssTestfileName = filepath;
        std::size_t ssTestExtfound = ssTestfileName.rfind(".c");
        if(ssTestExtfound == std::string::npos)
        {
            ssTestExtfound = ssTestfileName.rfind(".h");
            if(ssTestExtfound == std::string::npos)
                ssTestExtfound = ssTestfileName.size();
        }
        return ssTestfileName.substr(ssTestExtfound, ssTestfileName.size() - ssTestExtfound);
    }
    
    inline void OutputFormattedCode(std::string indentation, std::string code, int line)
    {
        std::string ssTestCodeStr = code;
        ssTestCodeStr = ssTestCodeStr.substr(1, ssTestCodeStr.size() - 2);
        for(size_t i = 0; i < ssTestCodeStr.size(); ++i)
        {
            if(ssTestCodeStr[i] == ' ' || ssTestCodeStr[i] == '\t')
                ssTestCodeStr.erase(i--, 1);
            else
                break;
        }
        for(int i = static_cast<int>(ssTestCodeStr.size() - 1); i >= 0; --i)
        {
            if(ssTestCodeStr[i] == ' ' || ssTestCodeStr[i] == '\t')
                ssTestCodeStr.erase(i, 1);
            else
                break;
        }
        bool ssTestInsideCurlyBrackets = false;
        for(size_t i = 0; i < ssTestCodeStr.size() - 1; ++i)
        { \
            if(ssTestCodeStr[i] == '{')
                ssTestInsideCurlyBrackets = true;
            else if(ssTestCodeStr[i] == '}')
                ssTestInsideCurlyBrackets = false;

            if(!ssTestInsideCurlyBrackets && ssTestCodeStr[i] == ';')
                ssTestCodeStr.insert(i + 1, "\n");
        }
        bool ssTestLastNewline = false;
        for(size_t i = 0; i < ssTestCodeStr.size(); ++i)
        {
            if(ssTestCodeStr[i] == '\n')
                ssTestLastNewline = true;
            else if(ssTestCodeStr[i] == ' ' && ssTestLastNewline)
                ssTestCodeStr.erase(i--, 1);
            else
                ssTestLastNewline = false;
        }
        for(size_t i = 0; i < ssTestCodeStr.size() - 1; ++i)
        {
            if(ssTestCodeStr[i] == '\n')
            {
                ssTestCodeStr.insert(i, "\"");
                ssTestCodeStr.insert(i + 2, std::string(indentation) + "\"");
                ++i;
            }
        }
        std::cout <<   indentation << "\"" << ssTestCodeStr << "\" on line " << line <<
                    " in " << Internal_ssTest::GetFileName(__FILE__) << 
                    Internal_ssTest::GetFileExt(__FILE__) << std::endl;
    }
    
    struct TestStatus
    {
        std::string ssTest_Indent = "| ";
        int ssTest_TestSuccess = 0;
        int ssTest_TestSkipped = 0;
        int ssTest_AssertSuccess = 0;
        int ssTest_AssertFailed = 0;
        int ssTest_OptionalSuccess = 0;
        int ssTest_OptionalFailed = 0;
        bool ssTest_LastTestResult = true;
        std::string ssTest_Name;
        std::string ssTest_FileName;
        std::string ssTest_FileExt;
        std::vector<std::function<void()>> ssTest_Functions;
        std::vector<std::string> ssTest_FunctionsNames;
        std::vector<std::string> ssTest_FailedTestsNames;
        std::vector<bool> ssTest_FunctionsSkipFlags;
        bool ssTest_SetUpCalled = false;
        bool ssTest_ResetBetweenTests = true;
        bool ssTest_OutputSetups = true;
        bool ssTest_OutputExecutions = true;
        bool ssTest_OutputAsserts = true;
        bool ssTest_RunningOptional = false;
        std::function<void()> ssTest_SetUp = [](){};
        std::function<void()> ssTest_CleanUp = [](){};
        int ssTest_TestOnly = -1;
    };
    
    inline void OutputOptionalText(Internal_ssTest::TestStatus& ssTest_Status)
    {
        if(ssTest_Status.ssTest_RunningOptional) 
            std::cout << termcolor::yellow << " (Optional) " << termcolor::reset;
    }
    
    inline void OutputAssertStarts(Internal_ssTest::TestStatus& ssTest_Status, std::string info)
    {
        if(info.empty())
        {
            ssCOUT << termcolor::blue << "|---- Assertion Starts:" << termcolor::reset;
            OutputOptionalText(ssTest_Status);
            std::cout << std::endl;
        }
        else
        {
            ssCOUT <<   termcolor::blue << "|---- Assertion Starts (" << info << "):" <<
                        termcolor::reset;
        
            OutputOptionalText(ssTest_Status);
            std::cout << std::endl;
        }
    }
    
    inline void OutputAssertResult(Internal_ssTest::TestStatus& ssTest_Status, bool ssTestInternalResult)
    {
        if(ssTestInternalResult)
        {
            if(ssTest_Status.ssTest_OutputAsserts)
            {
                ssCOUT <<    termcolor::green << "|     Assertion Passed (" <<
                            (ssTest_Status.ssTest_RunningOptional ? "+" : "O") << ")" << termcolor::reset;

                INTERNAL_ssTEST_OUTPUT_OPTIONAL_TEXT();
                std::cout << std::endl;
                ssCOUT << "|" << std::endl;
            }
            
            ssTest_Status.ssTest_RunningOptional ? 
            ++ssTest_Status.ssTest_OptionalSuccess : 
            ++ssTest_Status.ssTest_AssertSuccess;
        }
        else
        {
            if(true)
            {
                ssCOUT <<   termcolor::red << "|     Assertion Failed (" << \
                            (ssTest_Status.ssTest_RunningOptional ? "-" : "X") << ")" << termcolor::reset; \

                INTERNAL_ssTEST_OUTPUT_OPTIONAL_TEXT();
                std::cout << std::endl;
                ssCOUT << "|" << std::endl;
            }
            
            ssTest_Status.ssTest_RunningOptional ? 
            ++ssTest_Status.ssTest_OptionalFailed : 
            ++ssTest_Status.ssTest_AssertFailed;
        }
    }
    
    inline void OutputExceptionError(   Internal_ssTest::TestStatus& ssTest_Status, 
                                        std::string assertExpression,
                                        std::exception& ssTestExcept)
    {
        ssCOUT <<   termcolor::red << "|     Trying to run " << assertExpression <<
                    termcolor::reset << std::endl;
        
        ssCOUT <<   termcolor::red << "|     Error Caught (" << 
                    (ssTest_Status.ssTest_RunningOptional ? "-" : "X") << 
                    "):" << ssTestExcept.what() << termcolor::reset;
        
        INTERNAL_ssTEST_OUTPUT_OPTIONAL_TEXT(); 
        ssCOUT << std::endl; 
        ssCOUT << "|" << std::endl; 
        
        ssTest_Status.ssTest_AssertFailed++;
    }
    
    inline void OutputUnknownError( Internal_ssTest::TestStatus& ssTest_Status, 
                                    std::string assertExpression)
    {
        ssCOUT <<   termcolor::red << "|     Trying to run " << assertExpression <<
                    termcolor::reset << std::endl;

        ssCOUT <<   termcolor::red << "|     Error Caught (" << 
                    (ssTest_Status.ssTest_RunningOptional ? "-" : "X") <<
                    ")" << termcolor::reset;

        INTERNAL_ssTEST_OUTPUT_OPTIONAL_TEXT(); 

        if(ssTest_Status.ssTest_RunningOptional)
        {
            ssCOUT <<   termcolor::yellow <<
                        "|     Continuing even with unknown error for optional asserts" <<
                        termcolor::reset;

            ssTest_Status.ssTest_OptionalFailed++;
        }
        else
        {
            ssCOUT << termcolor::red << "|     Re-throwing unknown error..." << termcolor::reset;
            throw;
        }
        ssCOUT << std::endl;
        ssCOUT << "|" << std::endl;
    }
    
    inline void OutputAssert(   Internal_ssTest::TestStatus& ssTest_Status,
                                std::string info,
                                int line,
                                std::string assertExpression,
                                std::function<bool()> func)
    {
        try
        {
            if(ssTest_Status.ssTest_OutputAsserts)
            {
                INTERNAL_ssTEST_OUTPUT_ASSERT_STARTS(info);
                ssCOUT <<   "|     Asserting: \"" << assertExpression << "\" on line " <<
                            line << " in " << ssTest_Status.ssTest_FileName << 
                            ssTest_Status.ssTest_FileExt << std::endl;
            }

            bool ssTestInternalResult = false;
            ssTestInternalResult = func();
            ssTest_Status.ssTest_LastTestResult = ssTestInternalResult;

            if(!ssTestInternalResult)
            {
                ssCOUT <<   termcolor::red << "|     Assertion Failed For Expression: \"" << assertExpression;
                if(ssTest_Status.ssTest_OutputAsserts)
                    std::cout << "\"" << termcolor::reset << std::endl;
                else
                {
                    std::cout <<    "\" on line " << line << " in " << ssTest_Status.ssTest_FileName <<
                                    ssTest_Status.ssTest_FileExt << termcolor::reset << std::endl;
                }
            }

            OutputAssertResult(ssTest_Status, ssTestInternalResult);
        }
        INTERNAL_ssTEST_OUTPUT_CATCH_ERROR(assertExpression);
    }
    
    template<typename T, typename U> 
    inline void OutputAssertWithOperator(   Internal_ssTest::TestStatus& ssTest_Status,
                                            std::string info,
                                            int line,
                                            const T& assertValue,
                                            const U& expectedValue,
                                            std::string assertValuePrint,
                                            std::string expectedValuePrint,
                                            std::string operatorValuePrint,
                                            std::function<bool()> func)
    {
        try
        {
            if(ssTest_Status.ssTest_OutputAsserts)
            {
                INTERNAL_ssTEST_OUTPUT_ASSERT_STARTS(info);
                ssCOUT <<   "|     Asserting: \"" << assertValuePrint << " " << operatorValuePrint << " " <<
                            expectedValuePrint << "\" on line " << line << " in " << ssTest_Status.ssTest_FileName <<
                            ssTest_Status.ssTest_FileExt << std::endl;
            }

            bool ssTestInternalResult = false;
            {
                ssTestInternalResult = func();
                ssTest_Status.ssTest_LastTestResult = ssTestInternalResult;

                if(!ssTestInternalResult)
                {
                    ssCOUT <<   termcolor::red << "|     Assertion Failed For Expression: \"" <<
                                assertValuePrint << " " << operatorValuePrint << " " << expectedValuePrint;

                    if(ssTest_Status.ssTest_OutputAsserts)
                        std::cout << "\"" << termcolor::reset << std::endl;
                    else
                    {
                        std::cout << "\" on line " << line << " in " << ssTest_Status.ssTest_FileName <<
                        ssTest_Status.ssTest_FileExt << termcolor::reset << std::endl;
                    }

                    if(!ssTestInternalResult)
                    {
                        ssCOUT << termcolor::red << "|     Assertion Failed Values: " << std::endl;
                        ssCOUT <<   "|          " << assertValuePrint << " = " << assertValue << std::endl;
                        ssCOUT <<   "|          " << expectedValuePrint << " = " << expectedValue << termcolor::reset <<
                                    std::endl;
                    }
                }
            }

            OutputAssertResult(ssTest_Status, ssTestInternalResult);
        }
        INTERNAL_ssTEST_OUTPUT_CATCH_ERROR( assertValuePrint + " " + operatorValuePrint + " " + 
                                            expectedValuePrint);
    }
    
    inline void OutputSkip( Internal_ssTest::TestStatus& ssTest_Status,
                            std::string info,
                            int line,
                            std::string assertExpression)
    {
        if(std::string(info).empty())
            ssCOUT << termcolor::blue << "|---- Assertion Starts: " << termcolor::reset << std::endl;
        else
        {
            ssCOUT <<   termcolor::blue << "|---- Assertion Starts (" << info << "):" <<
                        termcolor::reset << std::endl;
        }

        ssCOUT <<   "|     Skipping: \"" << assertExpression << "\" on line " << line << " in " <<
                    ssTest_Status.ssTest_FileName << ssTest_Status.ssTest_FileExt << std::endl; \

        ssCOUT <<   "|     " << termcolor::yellow << "Assertion Skipped (/)" << termcolor::reset <<
                    std::endl;

        ssCOUT << "|" << std::endl;
    }
    
    #ifdef _WIN32
        #include <windows.h>
        //#include <ConsoleApi2.h>
    #endif
    
    inline int OutputAndRunAllTests(Internal_ssTest::TestStatus& ssTest_Status)
    {
        #ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
        #endif
        
        ssCOUT << "=====================================================================" << std::endl;
        ssCOUT << "\\ " << std::endl;
        ssCOUT << " > " << ssTest_Status.ssTest_Name.c_str() << std::endl;
        ssCOUT << "/ " << std::endl;
        ssCOUT << "=====================================================================" << std::endl;
        ssCOUT << std::endl;

        ssCOUT << "List of all " << ssTest_Status.ssTest_Functions.size() << " tests:" << std::endl;
        for(size_t i = 0; i < ssTest_Status.ssTest_Functions.size(); i++)
        {
            if(ssTest_Status.ssTest_FunctionsSkipFlags.at(i))
            {
                ssCOUT << "- (Skipped) \"" << ssTest_Status.ssTest_FunctionsNames.at(i) << "\"" <<
                std::endl;
                ++ssTest_Status.ssTest_TestSkipped;
            }
            else
                ssCOUT << "- \"" << ssTest_Status.ssTest_FunctionsNames.at(i) << "\"" << std::endl;
        }

        if(ssTest_Status.ssTest_TestOnly != -1)
        {
            ssCOUT << std::endl;
            ssCOUT << "But only running: \"" <<
                ssTest_Status.ssTest_FunctionsNames.at(ssTest_Status.ssTest_TestOnly) << "\"" <<
                std::endl;
        }

        if(!ssTest_Status.ssTest_ResetBetweenTests)
            ssTest_Status.ssTest_SetUp();
        for(size_t i = 0; i < ssTest_Status.ssTest_Functions.size(); i++)
        {
            if(ssTest_Status.ssTest_TestOnly != -1 && i != ssTest_Status.ssTest_TestOnly)
                continue;

            ssCOUT << std::endl;
            ssCOUT << "+-------------------------------------------------------" << std::endl;
            ssCOUT << "+ Running \"" << ssTest_Status.ssTest_FunctionsNames.at(i) << "\":" << std::endl;
            ssCOUT << "V-------------------------------------------------------" << std::endl;

            if(ssTest_Status.ssTest_FunctionsSkipFlags.at(i))
            {
                ssCOUT <<   termcolor::yellow << "|---> Test Skipped <---" <<
                            termcolor::reset << std::endl;

                ssCOUT << "L" << std::endl;
                continue;
            }
            else
            {
                if(ssTest_Status.ssTest_ResetBetweenTests)
                    ssTest_Status.ssTest_SetUp();

                int ssTestLastFailedCount = ssTest_Status.ssTest_AssertFailed;
                ssTest_Status.ssTest_Functions.at(i)();
                ssCOUT << "L" << std::endl;

                if(ssTestLastFailedCount == ssTest_Status.ssTest_AssertFailed)
                    ++ssTest_Status.ssTest_TestSuccess;
                else
                {
                    ssTest_Status   .ssTest_FailedTestsNames
                                    .push_back(ssTest_Status.ssTest_FunctionsNames.at(i));
                }

                if(ssTest_Status.ssTest_ResetBetweenTests)
                    ssTest_Status.ssTest_CleanUp();
            }
        }
        if(!ssTest_Status.ssTest_ResetBetweenTests)
            ssTest_Status.ssTest_CleanUp();

        int ssTestAssertTotal = ssTest_Status.ssTest_AssertSuccess + ssTest_Status.ssTest_AssertFailed;
        int ssTestOptionalAssertTotal = ssTest_Status.ssTest_OptionalSuccess + ssTest_Status.ssTest_OptionalFailed;
        ssCOUT << std::endl;
        ssCOUT << "All tests results:" << std::endl;
        ssCOUT <<   ssTest_Status.ssTest_TestSuccess << "/" << (ssTest_Status.ssTest_TestOnly != -1 ?
                    1 :
                    ssTest_Status.ssTest_Functions.size() - ssTest_Status.ssTest_TestSkipped) << " tests passed" << std::endl;

        ssCOUT << ssTest_Status.ssTest_AssertSuccess << "/" << ssTestAssertTotal << " assertions passed" << std::endl;

        if(ssTestOptionalAssertTotal > 0)
        {
            ssCOUT <<   termcolor::yellow << ssTest_Status.ssTest_OptionalSuccess << "/" <<
                        ssTestOptionalAssertTotal << " optional assertions passed" <<
                        termcolor::reset << std::endl;
        }

        if(ssTest_Status.ssTest_AssertFailed > 0)
        {
            ssCOUT <<   termcolor::red << ssTest_Status.ssTest_Name << " group has failed some tests (XXX)" <<
                        termcolor::reset << std::endl;

            for(size_t i = 0; i < ssTest_Status.ssTest_FailedTestsNames.size(); i++)
            {
                ssCOUT <<   termcolor::red << "- \"" << ssTest_Status.ssTest_FailedTestsNames.at(i) <<
                            "\" failed" << termcolor::reset << std::endl;
            }

            ssCOUT << std::endl;
            return EXIT_FAILURE;
        }
        else
        {\
            ssCOUT <<   termcolor::green << ssTest_Status.ssTest_Name << " group has passed all tests (OOO)" <<
                        termcolor::reset << std::endl;

            ssCOUT << std::endl;
            return EXIT_SUCCESS;
        }
    }
}

//ssTEST_COMMON_SETUP and ssTEST_COMMON_CLEANUP replaces ssTEST_COMMON_SET_UP and ssTEST_COMMON_CLEAN_UP
#define ssTEST_COMMON_SETUP ssTest_Status.ssTest_SetUp = [&]()
#define ssTEST_COMMON_CLEANUP ssTest_Status.ssTest_CleanUp = [&]()

#define ssTEST_COMMON_SET_UP \
    _Pragma("message( \"ssTEST_COMMON_SET_UP is deprecated, use ssTEST_COMMON_SETUP instead\" )") ssTEST_COMMON_SETUP
#define ssTEST_COMMON_CLEAN_UP \
    _Pragma("message( \"ssTEST_COMMON_CLEAN_UP is deprecated, use ssTEST_COMMON_CLEANUP instead\" )") ssTEST_COMMON_CLEANUP

#define ssTEST_CALL_COMMON_SETUP() ssTest_Status.ssTest_SetUp()
#define ssTEST_CALL_COMMON_CLEANUP() ssTest_Status.ssTest_CleanUp()

#define ssTEST_DISABLE_COMMON_SETUP_CLEANUP_BETWEEN_TESTS() \
    ssTest_Status.ssTest_ResetBetweenTests = false

#define ssTEST(name) \
    ssTest_Status.ssTest_Functions.resize(ssTest_Status.ssTest_Functions.size() + 1); \
    ssTest_Status.ssTest_FunctionsNames.push_back(name); \
    ssTest_Status.ssTest_FunctionsSkipFlags.push_back(false); \
    ssTest_Status.ssTest_Functions[ssTest_Status.ssTest_Functions.size() - 1] = [&]()

#define ssTEST_SKIP(name) \
    ssTest_Status.ssTest_Functions.resize(ssTest_Status.ssTest_Functions.size() + 1); \
    ssTest_Status.ssTest_FunctionsNames.push_back(name); \
    ssTest_Status.ssTest_FunctionsSkipFlags.push_back(true); \
    ssTest_Status.ssTest_Functions[ssTest_Status.ssTest_Functions.size() - 1] = [&]()

#define ssTEST_ONLY_THIS(name)\
    ssTest_Status.ssTest_TestOnly = ssTest_Status.ssTest_Functions.size(); \
    ssTEST(name)

#define ssTEST_DISABLE_OUTPUT_SETUP() ssTest_Status.ssTest_OutputSetups = false

#define ssTEST_OUTPUT_SETUP( setup, ... ) \
    if(ssTest_Status.ssTest_OutputSetups) \
    { \
        ssCOUT << termcolor::blue << "|---- Setting up: " << termcolor::reset << std::endl; \
        INTERNAL_ssTEST_OUTPUT_FORMATTED_CODE(ssTest_Status.ssTest_Indent + "|     ", (setup INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS(__VA_ARGS__))); \
    } \
    setup INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS(__VA_ARGS__) \
    if(ssTest_Status.ssTest_OutputSetups) \
        ssCOUT << "|" << std::endl

#define ssTEST_OUTPUT_SKIP_SETUP( setup, ... ) \
    if(ssTest_Status.ssTest_OutputSetups) \
    { \
        ssCOUT << termcolor::yellow << "|---- Skip setting up: " << termcolor::reset << std::endl; \
        INTERNAL_ssTEST_OUTPUT_FORMATTED_CODE(ssTest_Status.ssTest_Indent + "|     ", (setup INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS(__VA_ARGS__))); \
    } \
    if(ssTest_Status.ssTest_OutputSetups) \
        ssCOUT << "|" << std::endl

#define ssTEST_DISABLE_OUTPUT_EXECUTION() ssTest_Status.ssTest_OutputExecutions = false;

#define ssTEST_OUTPUT_EXECUTION( execution, ... ) \
    if(ssTest_Status.ssTest_OutputExecutions) \
    { \
        ssCOUT << termcolor::blue << "|---- Executing: " << termcolor::reset << std::endl; \
        INTERNAL_ssTEST_OUTPUT_FORMATTED_CODE(ssTest_Status.ssTest_Indent + "|     ", (execution INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS(__VA_ARGS__))); \
    } \
    execution INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS(__VA_ARGS__) \
    if(ssTest_Status.ssTest_OutputExecutions) \
        ssCOUT << "|" << std::endl

#define ssTEST_OUTPUT_SKIP_EXECUTION( execution, ... ) \
    if(ssTest_Status.ssTest_OutputExecutions) \
    { \
        ssCOUT << termcolor::yellow << "|---- Skip executing: " << termcolor::reset << std::endl; \
        INTERNAL_ssTEST_OUTPUT_FORMATTED_CODE(ssTest_Status.ssTest_Indent + "|     ", (execution INTERNAL_ssTEST_EXPAND_VA_IF_EXISTS(__VA_ARGS__))); \
    } \
    if(ssTest_Status.ssTest_OutputExecutions) \
        ssCOUT << "|" << std::endl

#define ssTEST_DISABLE_OUTPUT_ASSERT() ssTest_Status.ssTest_OutputAsserts = false;

#define ssTEST_OUTPUT_VALUES_WHEN_FAILED( ... ) \
    do \
    { \
        INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_VALUES_WHEN_FAILED, __VA_ARGS__ ) \
    } while(0)

#define ssTEST_OUTPUT_ASSERT( ... ) \
    do \
    { \
        INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_ASSERT, __VA_ARGS__ ) \
    } while(0)

#define ssTEST_OUTPUT_OPTIONAL_ASSERT( ... ) \
    do \
    { \
        ssTest_Status.ssTest_RunningOptional = true; \
        INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_ASSERT, __VA_ARGS__ ) \
        ssTest_Status.ssTest_RunningOptional = false; \
    } while(0)

#define ssTEST_OUTPUT_SKIP_ASSERT( ... ) \
    do \
    { \
        if(ssTest_Status.ssTest_OutputAsserts) \
        { \
            INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_SKIP, __VA_ARGS__ ) \
        } \
    } while(0)

#define ssTEST_GET_NESTED_TEST_GROUP_INDENT() ssTest_Status.ssTest_Indent + "|     "

#define ssTEST_SET_TEST_GROUP_INDENT(indent) \
    ssTest_Status.ssTest_Indent = indent + ssTest_Status.ssTest_Indent;

#define ssTEST_INIT_TEST_GROUP( ... ) \
    INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_INIT, __VA_ARGS__ ) \

#define INTERNAL_ssTEST_INIT_0() INTERNAL_ssTEST_INIT_1( Internal_ssTest::GetFileName(__FILE__) )

#define INTERNAL_ssTEST_INIT_1(testGroupName) \
{ \
    Internal_ssTest::TestStatus ssTest_Status; \
    ssTest_Status.ssTest_Name = testGroupName; \
    ssTest_Status.ssTest_FileName = Internal_ssTest::GetFileName(__FILE__); \
    ssTest_Status.ssTest_FileExt = Internal_ssTest::GetFileExt(__FILE__); \
    ssTest_Status.ssTest_Name = testGroupName; \
    \
    try \
    {

#define ssTEST_END_TEST_GROUP() \
        return OutputAndRunAllTests(ssTest_Status); \
    } \
    catch(const std::exception& e) \
    { \
        ssCOUT << "Error Caught (XXX): " << e.what() << std::endl; \
        throw; \
    } \
    catch(...) \
    { \
        ssCOUT << "Unknown Error Caught (XXX)" << std::endl; \
        throw; \
    } \
}

#endif
