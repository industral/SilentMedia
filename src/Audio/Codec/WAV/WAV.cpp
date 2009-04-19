#include <SML/config.hpp>
#include "WAV.hpp"
#include <SML/Audio/Codec/DecodedData.hpp>

SilentMedia::Codec::WAV::WAV ( void ) :
/*
   offsetPos в 0 а не в -1, ибо он только увиличивается инкрементом а не инициализируется. Соответственно нужно инициализировать в ноль.
   Тем более что как раз прочитаное количество семплов в начале будет равно нулю, что и есть верно.
*/
wavinfo ( new WavInfo ), input_fd ( -1 ), offsetPos ( 0 ), fileSize ( -1 ), totalTime ( -1 ),
          totalSamples ( -1 ), bitRate ( -1 ), playCheck ( false ), seekCheck ( false )
{
   this -> ddata = DecodedData::Instance();
}

SilentMedia::Codec::WAV::~WAV ( void ) {
//    std::cout << "WAV Destructor.. " << std::endl;
   delete wavinfo; wavinfo = NULL;
   this -> flush();
}

bool SilentMedia::Codec::WAV::init ( std::string inputFile, std::string idObj ) {
   this -> inputFile = inputFile;
   this -> idObj = idObj;
//    std::cout << "file: " << this -> inputFile << std::endl;

   // открываем файловый дискриптор
   this -> pFile = fopen ( this -> inputFile . c_str(), "rb" );
// 
   // Считываем информацию в класс WAV
   // http://ccrma.stanford.edu/CCRMA/Courses/422/projects/WaveFormat/
//    std::cout << "wavinfo: " << this -> wavinfo << std::endl;
//    std::cout << "pFile: " << this -> pFile << std::endl;
   fread ( this -> wavinfo, sizeof ( WavInfo ), 1, this -> pFile );
   // закрываем файловый дискриптор
   if ( fclose ( this -> pFile ) != 0 ) {
      std::cout << "Error in WavDecode::init(): error in fclose()" << std::endl;
   }
//    if ( SILENTMEDIA_DEBUG == true ) {
//       std::cout << "ChunkID: " << wavinfo -> getChunkID() << std::endl;
//       std::cout << "ChunkSize: " << wavinfo -> getChunkSize() << std::endl;
//       std::cout << "Format: " << wavinfo -> getFormat() << std::endl;
//       std::cout << "Subchunk1ID: " << wavinfo -> getSubchunk1ID() << std::endl;
//       std::cout << "Subchunk1Size: " << wavinfo -> getSubchunk1Size() << std::endl;
//       std::cout << "AudioFormat: " << wavinfo -> getAudioFormat() << std::endl;
//       std::cout << "NumChannels: " << wavinfo -> getNumChannels() << std::endl;
//       std::cout << "SampleRate: " << wavinfo -> getSampleRate() << std::endl;
//       std::cout << "ByteRate: " << wavinfo -> getByteRate() << std::endl;
//       std::cout << "BlockAlign: " << wavinfo -> getBlockAlign() << std::endl;
//       std::cout << "BitsPerSample: " << wavinfo -> getBitsPerSample() << std::endl;
//       std::cout << "Subchunk2ID: " << wavinfo -> getSubchunk2ID() << std::endl;
//       std::cout << "Subchunk2Size: " << wavinfo -> getSubchunk2Size() << std::endl;
//    }

   // Общее время звучания файла получаем с делением размера полезных аудио-данных ( pcm-поток ) на байтрейт ( размер занимаемых данных за секунду )
   // По идеи это общее количество семплов разделено на количество семплов за секунду ( sampleRate ). Но тут нету общего количество семплов.
   // По этому приходится считать так извращенно :)
   this -> totalTime = ( static_cast < double > ( wavinfo -> getChunkSize() ) / ( wavinfo -> getByteRate() ) );

   // Общее количество семплов = количество семплов за секунду * общее время звучания песни
   this -> totalSamples = ( ( wavinfo -> getSampleRate() ) * ( this -> totalTime ) );
   this -> bitRate = ( ( wavinfo -> getByteRate() ) * 8 );

//    std::cout << "WAV: " << ::_ddata << std::endl;
   this -> ddata -> setAudioParams ( this -> inputFile, wavinfo -> getChunkSize(), this -> totalTime,
                                   wavinfo -> getNumChannels(), wavinfo -> getSampleRate(),
                                         this -> bitRate, wavinfo -> getBitsPerSample()
                                 );
   return true;
}

bool SilentMedia::Codec::WAV::write ( std::string id ) {
//    std::cout << "WAV::write(): id = " << id << std::endl;
   // Записываем размер файла в переменную, так как при каждой инициализации другого файла это значение будет менятся
   this -> fileSize = wavinfo -> getChunkSize(); // ?
   // Не открываем файл повторно если он в режиме воспроизведения или делался seek
   if ( this -> playCheck == false || this -> seekCheck == false ) {
      if ( ( this -> input_fd = open ( this -> inputFile . c_str(), O_RDONLY ) ) == -1 ) {
//          std::cout << "WAV: " << this -> inputFile << std::endl;
         return ( _debug ( this -> inputFile ) );
      }
   }

   this -> playCheck = true;

   const int buf_size = 4096;
   int k = 2 * wavinfo -> getNumChannels();
   int bufSize = k * buf_size;
   int actlen = 0;
   char buf [ bufSize ];

   this -> ddata -> begin ( this -> idObj );
   while ( ( actlen = read ( this -> input_fd, buf, bufSize ) ) ) {
      this -> offsetPos += actlen;
      this -> ddata -> write ( &buf, this -> idObj );
//       write ( dspDev, buf, actlen );
//       fwrite(buf,1,actlen,stdout);
//       std::cout << offsetPos << std::endl;
   }
   std::cout << "End stream!" << std::endl;
   this -> ddata -> end ( this -> idObj );
}

// Получаем текущее значение временной метки путем деления полного размера файла в байтах на количество уже считаных байт
unsigned long int SilentMedia::Codec::WAV::getSeek ( void ) const {
   return ( 100 / ( this -> fileSize / ( this -> offsetPos ) ) ) ;
}

void SilentMedia::Codec::WAV::setSeek ( double val ) {
   this -> seekCheck = true;
   unsigned long int resultPos = ( this -> fileSize * ( val / 100 ) );
   /*
   Если число resultPos получится не парное, то вместо музыки мы услышим шум...
   Поэтому проверяем парное ли число, если нет то прибавляем единицу.
   */
   if ( resultPos % 2 != 0 ) { ++resultPos; }
   this -> offsetPos = lseek ( this -> input_fd, resultPos, SEEK_SET );
}

void SilentMedia::Codec::WAV::flush ( void ) {
//    std::cout << "in Codec::WAV::flush()" << std::endl;
   // обнуляем показатели
   this -> playCheck = false;
   this -> seekCheck = false;
   this -> offsetPos = 0;

   // закрываем файловый дискриптор
   if ( this -> input_fd != -1 ) {
      if ( close ( this -> input_fd ) != 0 ) {
         std::cout << "Error in close()" << std::endl;
      }
   }
   this -> input_fd = -1;
}

