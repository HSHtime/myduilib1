/*
 * log.c
 *
 * Copyright (C) 2009 Hector Martin <hector@marcansoft.com>
 * Copyright (C) 2009 Nikias Bassen <nikias@gmx.li>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <libcompat.h>
#else
#include <sys/time.h>
#endif
//#include <syslog.h>

#include "log.h"

unsigned int log_level = LL_WARNING;

int log_syslog = 0;

void log_enable_syslog()
{
	if (!log_syslog) {
		//openlog("usbmuxd", LOG_PID, 0);
		log_syslog = 1;
	}
}

void log_disable_syslog()
{
	if (log_syslog) {
		//closelog();
	}
}

static int level_to_syslog_level(int level)
{
	/*int result = level + LOG_CRIT;
	if (result > LOG_DEBUG) {
	result = LOG_DEBUG;
	}*/
	//return result;
	return level;
}

void usbmuxd_log(enum loglevel level, const char *fmt, ...)
{
	va_list ap;
	char *fs;
	struct timeval ts;
	struct tm *tp;

	if(level > log_level)
		return;

	gettimeofday(&ts, NULL);
#ifdef WIN32
	time_t tm = (time_t)ts.tv_sec;
	tp = localtime(&tm);
#else
	tp = localtime(&ts.tv_sec);
#endif

	fs = (char *)malloc(20 + strlen(fmt));

	if(log_syslog) {
		sprintf(fs, "[%d] %s\n", level, fmt);
	} else {
		strftime(fs, 10, "[%H:%M:%S", tp);
		sprintf(fs+9, ".%03d][%d] %s\n", (int)(ts.tv_usec / 1000), level, fmt);
	}

	va_start(ap, fmt);
	if (log_syslog) {
		//vsyslog(level_to_syslog_level(level), fs, ap);
	} else {
		vfprintf(stderr, fs, ap);
	}
	va_end(ap);

	free(fs);
}