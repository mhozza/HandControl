/*
 *    HandControl - Hand gesture recognition
 *    Copyright (C) 2010  Michal Hozza (mhozza@gmail.com)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>


namespace handcontrol
{
    class Exception : public std::exception
    {
    protected:
        const char* message;
    public:
        Exception(const char* message = "General exception.\0") : message(message){}
        inline const char* what() const throw(){return message;}

    };

    class CameraGetImageFailureException : public Exception
    {

    };

    class CameraStartStreamingFailureException : public Exception
    {
    protected:
        const char* message;
    public:
        CameraStartStreamingFailureException(const char* message = "Could not start streaming.\0") : message(message){}
    };

    class CameraOpenException : public Exception
    {
    protected:
        const char* message;
    public:
        CameraOpenException(const char* message = "Could not open device.\0") : message(message){}
    };
}

#endif // EXCEPTIONS_H
