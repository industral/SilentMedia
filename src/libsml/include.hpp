#ifndef _SILENTMEDIA_INCLUDE_HPP_
#define _SILENTMEDIA_INCLUDE_HPP_

// C++ header
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <fstream>

// boost string
#include <boost/algorithm/string.hpp>

// boost filesystem
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

// boost thread
#include <boost/thread/thread.hpp>

// boost foreach
#include <boost/foreach.hpp>

// boost random
#include <boost/random.hpp>

/* libconfig
 * http://www.hyperrealm.com/libconfig/
 */
#include <libconfig.h++>

// C header
#include <cstdio>
#include <ctime> // time
#include <climits> // limits

// include log4cxx
#include <log4cxx/logger.h>

// include Utils
#include <libsml/Utils/include.hpp>

using namespace log4cxx;

enum PlayStatus {
  Stop, Play, Pause
};

#endif
