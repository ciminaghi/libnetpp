// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// linux/event-manager.h : header file for linux event-manager.h
#ifndef __linux_event_manager_h__
#define __linux_event_manager_h__

#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <map>

using namespace std;

#include "logger.h"
#include "event.h"
#include "timer.h"

namespace netpp {

  class linux_event_manager : public event_manager {
  private:
    map<int, event *>events;
    int max_fd;
    fd_set requested_fds;
    linux_event_manager(): event_manager(), max_fd(-1)
    { FD_ZERO(&requested_fds); };
  public:
    static linux_event_manager *get(void)
    {
      if (!instance)
	instance = new linux_event_manager();
      return static_cast<linux_event_manager *>(instance);
    }
    void add_fd(int fd, event *e)
    {
      if(fd > max_fd)
	max_fd = fd;
      events[fd] = e;
      FD_SET(fd, &requested_fds);
    }
    void del_fd(int fd)
    {
      event *e = events[fd];
      events.erase(fd);
      if (fd == max_fd) {
	map<int, event *>::iterator i;
	for(max_fd = -1, i = events.begin(); i != events.end(); ++i)
	  if (i->first > max_fd)
	    max_fd = i->first;
      }
      FD_CLR(fd, &requested_fds);
      e->mark_for_deletion();
    }
    virtual void idle(void)
    {
      fd_set s = requested_fds;
      unsigned long long next_timeout;
      struct timeval _tv, *tv;
      timer *t = timer::get();
      event *e;

      tv = NULL;
      if (t->get_next_evt_time(next_timeout)) {
	_tv.tv_usec = next_timeout * 1000;
	for ( ; _tv.tv_usec >= 1000000; _tv.tv_usec -= 1000000, _tv.tv_sec++);
	tv = &_tv;
      }
      int stat = select(max_fd + 1, &s, NULL, NULL, tv);
      switch (stat) {
      case 0:
	e = t->get_next_evt();
	if (!e)
	  break;
	e->arm();
	break;
      case -1:
	if (errno == EINTR)
	  break;
	log_fatal << __func__ << __LINE__ << "select error";
	break;
      default:
	map<int, event *>::iterator i;
	for (i = events.begin(); i != events.end(); ++i)
	  if (FD_ISSET(i->first, &s)) {
	    e = i->second;
	    e->arm();
	  }
      }
    }
    virtual ~linux_event_manager()
    {
      map<int, event *>::iterator i;
      for(i = events.begin(); i != events.end(); ++i)
	del_fd(i->first);
    };
  };

} // namespace netpp

#endif /* __linux_event_manager_h__ */
