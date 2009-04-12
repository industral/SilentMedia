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

#ifndef __SS_SILENTMEDIA_SML_HPP_
#define __SS_SILENTMEDIA_SML_HPP_

// standart C/C++ library
#include "slib.hpp"

const std::string SILENTMEDIA_VERSION = "0.0.1";
const std::string SILENTMEDIA_API_VERSION = "0.0.1";

const bool SILENTMEDIA_DEBUG = true;
const bool SILENTMEDIA_STRICT = true;

enum SoundSystemType { _OSS = 1, _ALSA };
enum AudioComponents { _DSP = 2, _MIXER = 4 };
enum EngineType { _Player, _Game };


#include <FLAC++/all.h>
#include <SML/Utils/Func/Func.hpp>
#include <SML/Utils/Base64/Base64.hpp>

#endif
