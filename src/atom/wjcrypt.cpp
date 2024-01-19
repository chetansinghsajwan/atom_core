module;

extern "C"
{
#include "WjCryptLib_Sha1.h"
#include "WjCryptLib_Md5.h"
}

export module atom.core:wjcrypt;

using ::SHA1_HASH;
using ::Sha1Context;
using ::Sha1Finalise;
using ::Sha1Initialise;
using ::Sha1Update;

using ::MD5_HASH;
using ::Md5Context;
using ::Md5Finalise;
using ::Md5Initialise;
using ::Md5Update;
