#include "sml.hpp"

SilentMediaLibrary::SML::SML ( void ) { }
SilentMediaLibrary::SML::~SML ( void ) { }

void SilentMediaLibrary::SML::debug ( std::string name ) {
      std::cout << std::endl << "[libsml v." << SILENTMEDIA_VERSION << "]: " << "Error in " << "`"
            << __FILE__ << "`" << " line: " << __LINE__ << std::endl;
      perror ( name . c_str() );
   }
}
