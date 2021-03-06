/*
  Copyright (c) 2016, Antonio SJ Musumeci <trapexit@spawn.link>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <grp.h>
#include <pwd.h>

#include <map>
#include <vector>

namespace mergerfs
{
  namespace ugid
  {
    extern __thread uid_t currentuid;
    extern __thread gid_t currentgid;
    extern __thread bool  initialized;

    struct Set
    {
      Set(const uid_t newuid,
          const gid_t newgid)
      {
        if(!initialized)
          {
            currentuid  = ::syscall(SYS_geteuid);
            currentgid  = ::syscall(SYS_getegid);
            initialized = true;
          }

        if(newuid == currentuid && newgid == currentgid)
          return;

        if(currentuid != 0)
          {
            ::syscall(SYS_setreuid,-1,0);
            ::syscall(SYS_setregid,-1,0);
          }

        if(newgid)
          {
            ::syscall(SYS_setregid,-1,newgid);
            initgroups(newuid,newgid);
          }

        if(newuid)
          ::syscall(SYS_setreuid,-1,newuid);

        currentuid = newuid;
        currentgid = newgid;
      }
    };

    struct SetRootGuard
    {
      SetRootGuard() :
        prevuid(currentuid),
        prevgid(currentgid)
      {
        Set(0,0);
      }

      ~SetRootGuard()
      {
        Set(prevuid,prevgid);
      }

      const uid_t prevuid;
      const gid_t prevgid;
    };
  }
}
