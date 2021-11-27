// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"strsafe.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"openssl-0.9.8l/out32/ssleay32.lib")
#pragma comment(lib,"openssl-0.9.8l/out32/libeay32.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <aclapi.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdint.h>
#include <new>
#include <iostream>
#include <cassert>
#include <queue>
#include <list>

#include "CSmtp.h"
#include "base64.h"
#include "md5.h"
#include "inifile.h"



// TODO: reference additional headers your program requires here
