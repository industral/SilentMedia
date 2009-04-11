#include <fstream>
#include <iostream>
#include <string>
#include <vector>

   std::string allow = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

   std::string line, s;
   size_t begin = -1;
   size_t end = -1;
   size_t start = -1;
   size_t stop = -1;
      
   size_t start_param = -1;
   size_t stop_param = -1;
   size_t end_param = -1;
   size_t begin_param = -1;
   size_t find;
      
   std::string action, method, param, symbol;
      
   std::vector < std::string > list;

   std::string readParam ( size_t pos ) {
      begin_param = s.find ( "\"", pos );
      end_param = s.find ( "\"", ++begin_param);

      param = s.substr ( begin_param, end_param - begin_param );

      find = s.find_first_not_of ( allow, ++end_param );
      symbol = s.substr ( find, 1 );

      return param;
   }


   int main ( void ) {

   
      std::ifstream in ( "sound.map" );

      if ( in.is_open() ) {

      
         int start_action, end_action, start_method, end_method;
      
         while ( getline ( in, line ) ) {
            s += line;
         
         
         
         
//          list . push_back ( line );
//          std::cout << "Line: " << line << std::endl;
            //          
//          begin = line.find ( "@" );
//          end = line.find_first_of ( " ", begin );
//          start = line.find ( "{" );
            //         
//          std::cout << "begin: " << begin << std::endl;
//          std::cout << "end: " << end << std::endl;
            // //          
//          if ( begin != std::string::npos && end != std::string::npos && start != std::string::npos ) {
//             action = line.substr ( begin, end );
//             std::cout << "Action: " << action << std::endl;
//          }
            //          

//          }
            //          
            //          
//          std::cout << std::endl;
         }

         begin = s.find ( "@" );
         end = s.find_first_of ( " ", begin );

         action = s.substr ( begin, end - begin );
         std::cout << "Action: " << action << std::endl;
      
      // method
         begin = s.find ( "#" );
         end = s.find_first_of ( " ", begin );
         start = s.find_first_of ( "{", end );

         if ( begin != std::string::npos && end != std::string::npos && start != std::string::npos ) {
            method = s.substr ( begin, end - begin );
            std::cout << "Method: " << method << std::endl;

            std::cout << "Param: " << readParam ( start ) << std::endl;

            while ( symbol == "," ) {
               std::cout << "Param: " << readParam ( find ) << std::endl;
            }
            
         }
      
      
      
//       std::cout << s << std::endl;
//       for ( int i = 0; i < list.size(); ++i ) {
// //          std::cout << "Line: " << list [ i ]  << std::endl;
         //          
//          // find action
//          begin = list [ i ].find ( "@" );
//          end = list [ i ].find_first_of ( " ", begin );
//          start = list [ i ].find ( "{" );
         //                  
// //          std::cout << "begin: " << begin << std::endl;
// //          std::cout << "end: " << end << std::endl;
         //          //          
//          if ( begin != std::string::npos && end != std::string::npos && start != std::string::npos ) {
//             action = list [ i ].substr ( begin, end );
//             std::cout << "Action: " << action << std::endl;
//             start_action = i;
//          }
//          // end find action
         //          
//          // find method
//          begin = list [ i ].find ( "#" );
//          end = list [ i ].find_first_of ( " ", begin );
//          start = list [ i ].find ( "{" );
         //                   
// //          std::cout << "begin: " << begin << std::endl;
// //          std::cout << "end: " << end << std::endl;
         //                   
//          if ( begin != std::string::npos && end != std::string::npos && start != std::string::npos ) {
//             method = list [ i ].substr ( begin, end );
//             std::cout << "Method: " << method << std::endl;
//             start_method = i;
//          }
         //          
//          int stuff = i;
//          if ( start_method == --stuff ) {
         //             
//          // end find method
//       }
      
      

//       begin = s.find ( "@shortGun" );
//       std::cout << begin << std::endl;
         //       
//       end = s.find ( " " );
//       std::cout << end << std::endl;
         //       
//       std::string str2 = s.substr ( begin, end - 3 );
//       std::cout << str2 << std::endl;
         //       
//       begin = s.find ( "#" );
//       std::cout << begin << std::endl;
         //       
//       end = s.find ( " " );
//       std::cout << end << std::endl;
      
//          if ( i != std::string::npos && line[i] == '#' )
//             continue;
      
      
      }
      return 0;
   }
