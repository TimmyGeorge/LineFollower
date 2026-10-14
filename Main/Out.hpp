#ifndef OUT_HPP
#define OUT_HPP

#if SERIAL_PRINTS_ENABLED
  #define OUT(code) code;
#else
  #define OUT(code) 
#endif

#endif