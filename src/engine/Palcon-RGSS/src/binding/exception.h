/*
** exception.h
**
** Exception handling for Palcon-RGSS
** Based on mkxp exception.h
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <stdexcept>

struct Exception : public std::runtime_error
{
	Exception(const std::string &msg)
	    : std::runtime_error(msg)
	{}
};

#endif // EXCEPTION_H
