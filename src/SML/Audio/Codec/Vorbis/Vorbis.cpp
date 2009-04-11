/***************************************************************************
 *   Copyright (C) 2008 by Alex J. Ivasyuv                                 *
 *   alex@siegerstein.org.ua                                               *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "Vorbis.hpp"
#include <SML/Audio/Codec/DecodedData.hpp>

SilentMedia::Codec::Vorbis::Vorbis ( void ) : dspDev ( -1 ), vi ( NULL ), length ( 0 ), seekPos ( 0 ),
                                           seek ( 0 )
{
   this -> ddata = DecodedData::Instance();
}

SilentMedia::Codec::Vorbis::~Vorbis ( void ) {

}

bool SilentMedia::Codec::Vorbis::init ( std::string inputFile ) {
   this -> dspDev = this -> ddata -> getDSPDev();
   this -> fileName = inputFile;

   ov_fopen ( const_cast < char* > ( this -> fileName.c_str() ), &vf );

   vi = ov_info ( &vf, -1 );
   this -> ddata -> setMainParam ( Utils::Func::getFileSize ( this -> fileName.c_str() ), ov_time_total ( &vf, -1 ),
                                   vi -> channels, vi -> rate, vi -> bitrate_nominal, -1 );
   this -> readVorbisComment();

   return 0;
}

void SilentMedia::Codec::Vorbis::readVorbisComment ( void ) {
//    std::cout << ov_comment ( &vf, -1 ) -> vendor << std::endl;
   char **ptr = ov_comment ( &vf, -1 ) -> user_comments;

   std::vector < std::string > vorbisCommList;

   std::map < std::string, std::string > vorbisComm;
   vorbisComm . clear();

   while ( *ptr ) {
      std::string myStr = *ptr;
      int find = myStr . find_first_of ( "=" );
      if ( find) {
         vorbisComm [ Utils::Func::toUpper ( myStr . substr ( 0, find ) ) ] = myStr . substr ( find + 1 );
      }
      ++ptr;
   }
   this -> ddata -> setVorbisComment ( vorbisComm );
}

void SilentMedia::Codec::Vorbis::play ( ) {
   if ( seek == 0 ) {
      ov_fopen ( const_cast < char* > ( this -> fileName . c_str() ), &this -> pvf );
   }

   if ( ov_seekable ( &this -> pvf ) ) {
      this -> length = ov_time_total ( &this -> pvf, -1 );
   } else {
      std::cout << "Source is not seekable." << std::endl;
   }
   
   const int buf_size = 4096;
   char buf [ buf_size ];
   
   int eof = 0;
   int current_section = 0;

//    for(int i=0;i<ov_streams(&this -> pvf);i++){
// //       vorbis_info *vi=ov_info(&ov,i);
// //       printf("\tlogical bitstream section %d information:\n",i+1);
// //       printf("\t\t%ldHz %d channels bitrate %ldkbps serial number=%ld\n",
// //              vi->rate,vi->channels,ov_bitrate(&ov,i)/1000,
// //                                                ov_serialnumber(&ov,i));
// //       printf("\t\tcompressed length: %ld bytes ",(long)(ov_raw_total(&ov,i)));
// //       printf(" play time: %lds\n",(long)ov_time_total(&ov,i));
//       std::cout << "bitrate: " << ov_bitrate ( &this -> pvf, i ) << std::endl;
//    }
   
   this -> ddata -> setParam();
   while ( !eof ) {
      long ret = ov_read ( &this -> pvf, buf, buf_size, 0, 2, 1, &current_section );
      if ( ret == 0 ) {
         eof = 1;
      } else if ( ret < 0 ) {
         std::cout << "ERROR in ov_read()" << std::endl;
      } else {
         write ( this -> dspDev, buf, ret );
//          std::cout << "average bitrate: " << ov_bitrate ( &this -> pvf, -1 ) << std::endl;
//          std::cout << "bitrate: " << ov_bitrate_instant ( &this -> pvf ) << std::endl << std::flush;
      }
   }
   ov_clear ( &pvf );
}

void SilentMedia::Codec::Vorbis::setSeekPos ( double val ) {
   this -> seek = 1;

   int retCode = ov_time_seek ( &this -> pvf, ( ( this -> length ) * ( val / 100 ) ) );
   if ( retCode != 0 ) {
      if ( retCode == OV_ENOSEEK ) {
         std::cout << "Error in ov_time_seek(): Bitstream is not seekable" << std::endl;
      } else if ( retCode == OV_EINVAL ) {
         std::cout << "Error in ov_time_seek(): Invalid argument value; possibly called with an OggVorbis_File structure that isn't open" << std::endl;
      } else if ( retCode == OV_EREAD ) {
         std::cout << "Error in ov_time_seek(): A read from media returned an error" << std::endl;
      } else if ( retCode == OV_EFAULT ) {
         std::cout << "Error in ov_time_seek(): Internal logic fault; indicates a bug or heap/stack corruption." << std::endl;
      } else if ( retCode == OV_EBADLINK ) {
         std::cout << "Error in ov_time_seek(): Invalid stream section supplied to libvorbisfile, or the requested link is corrupt." << std::endl;
      }
   }
}

unsigned long int SilentMedia::Codec::Vorbis::getCurrSeekPos ( void ) const {
   if ( this -> length == 0 ) {
      return 0;
   } else {
      return ( 100 / ( this -> length / ov_time_tell ( const_cast < OggVorbis_File* > (&this -> pvf) ) ) );
   }
}

void SilentMedia::Codec::Vorbis::flush ( void ) {
   this -> seek = 0;
}
