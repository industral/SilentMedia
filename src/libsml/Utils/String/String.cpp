#include "String.hpp"

namespace SilentMedia {
  namespace Utils {
    string String::fromXML(const string &inString) {
      string outString = inString;

      boost::replace_all(outString, "%20", " ");
      boost::replace_all(outString, "&lt;", "<");
      boost::replace_all(outString, "&gt;", ">");
      boost::replace_all(outString, "&quot;", "\"");
      boost::replace_all(outString, "&apos;", "'");
      boost::replace_all(outString, "&amp;", "&");

      return outString;
    }

    string String::toXML(const string &inString) {
      string outString = inString;

      boost::replace_all(outString, " ", "%20");
      boost::replace_all(outString, "<", "&lt;");
      boost::replace_all(outString, ">", "&gt;");
      boost::replace_all(outString, "\"", "&quot;");
      boost::replace_all(outString, "'", "&apos;");
      boost::replace_all(outString, "&", "&amp;");

      return outString;
    }
  }
}
