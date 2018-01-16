#define _GNU_SOURCE

#include <dlfcn.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#ifdef HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif
#include <netinet/in.h>
#include <sys/stat.h>
#include <assert.h>


static int (*next_socket)(int domain, int type, int protocol);

int socket(int domain, int type, int protocol)
{
  if(!next_socket) {
    next_socket = dlsym(RTLD_NEXT, "socket");
  }

  int fd = (*next_socket)(domain, type, protocol);
  if(fd < 0)
    return fd;

//  if(domain == PF_INET || domain == PF_INET6) {
//    int one = 1;
//    setsockopt(fd, SOL_IP, IP_FREEBIND, &one, sizeof(one));
//  }

#ifdef IP_FREEBIND
	int one = 1;
        setsockopt(fd, SOL_IP, IP_FREEBIND, &one, sizeof(one));
#endif

#ifdef IP_BINDANY
	if (domain == AF_INET) {
		int one = 1;
		setsockopt(fd, IPPROTO_IP, IP_BINDANY, &one, sizeof(one));
	}
#endif
#ifdef IPV6_BINDANY
	if (domain == AF_INET6) {
		int one = 1;
		setsockopt(fd, IPPROTO_IPV6, IPV6_BINDANY, &one, sizeof(one));
	}
#endif

  return fd;
}
