#ifndef GETTEXT_H
#define GETTEXT_H

#include <string>
#include <libintl.h>
#include <irrlicht.h>

#ifndef _WIN32
#  include <iconv.h>
#else
#  include <Windows.h>
#endif

using namespace irr;

#define _(string) gettext(string)
#define _w(string) utf8_to_irrwide(gettext(string))
#define _wp(string) utf8_to_wide(gettext(string)).c_str()

std::size_t convert(const char *to, const char *from,
                    char *outbuf, std::size_t outbuf_size,
                    char *inbuf, std::size_t inbuf_size);
std::wstring utf8_to_wide(const std::string &input);
core::stringw utf8_to_irrwide(const std::string &input);
std::string wide_to_utf8(const std::wstring &input);

core::stringw keyCodeName(const EKEY_CODE &keyCode);
void setLanguage(const std::string &language, bool replace);

#endif // GETTEXT_H