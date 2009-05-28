/*******************************************************************************
 * Copyright (c) 2009, Alex Ivasyuv                                            *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 * 1. Redistributions of source code must retain the above copyright           *
 *    notice, this list of conditions and the following disclaimer.            *
 * 2. Redistributions in binary form must reproduce the above copyright        *
 *    notice, this list of conditions and the following disclaimer in the      *
 *    documentation and/or other materials provided with the distribution.     *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY Alex Ivasyuv ''AS IS'' AND ANY                 *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL Alex Ivasyuv BE LIABLE FOR ANY DIRECT,        *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES          *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;*
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF    *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           *
 ******************************************************************************/

#include <string>

#include <Media/PlayList/XSPF/XSPF.hpp>

//#include <boost/test/minimal.hpp>

using namespace std;
using namespace SilentMedia::Media::PlayList::XSPF;

//int test_main(int, char *[]) {
int main() {
//  const string playListFile = "../src/Test/PlayLists/XSPF/playlist.xspf";
  const string playListFile = "../src/Test/PlayLists/XSPF/sample.xspf";

  XSPF * playlist = new XSPF();
//
  bool ret = playlist -> open(playListFile);
  cout << ret << endl;

//  list < map < string, string > > tackMap = playlist -> getPlayListMap();
  playlist -> close();
//
  delete playlist;
  playlist = NULL;

//    // six ways to detect and report the same error:
//    BOOST_CHECK( add( 2,2 ) == 4 ); // #1 continues on error
//    BOOST_REQUIRE( add( 2,2 ) == 4 ); // #2 throws on error
//    if (add(2, 2) != 4)
//      BOOST_ERROR( "Ouch..." ); // #3 continues on error
//    if (add(2, 2) != 4)
//      BOOST_FAIL( "Ouch..." ); // #4 throws on error
//    if (add(2, 2) != 4)
//      throw "Oops..."; // #5 throws on error
//
//    return add(2, 2) == 4 ? 0 : 1; // #6 returns error code
  return 0;
}
