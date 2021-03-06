#ifndef _TIMING_WIN64_HPP_
#define _TIMING_WIN64_HPP_

#include  <stdio.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include <windows.h>

struct Measurement{
  __int64 totalTime;
};

class ProfilerDp{

public:
  ProfilerDp( std::string& aTitle );
  void start ( std::string& aName );
  void end ( );
  void dumpToFile( const char* aFileName );

private:
  std::string               mTitle;
  std::vector<Measurement>  mTimings;
  std::vector<std::string>  mBlockNames;
  __int64 mStart;
  __int64 mEnd;
  
  

private:
  friend class Game;
  friend class Physics;

private:
  __int64 time();

  __int64 ReadTSC() {
     int res[2];                              // store 64 bit result here

     #if defined(__GNUC__) && !defined(__INTEL_COMPILER)
     // Inline assembly in AT&T syntax

     #if defined (_LP64)                      // 64 bit mode
        __asm__ __volatile__  (               // serialize (save rbx)
        "xorl %%eax,%%eax \n push %%rbx \n cpuid \n"
         ::: "%rax", "%rcx", "%rdx");
        __asm__ __volatile__  (               // read TSC, store edx:eax in res
        "rdtsc\n"
         : "=a" (res[0]), "=d" (res[1]) );
        __asm__ __volatile__  (               // serialize again
        "xorl %%eax,%%eax \n cpuid \n pop %%rbx \n"
         ::: "%rax", "%rcx", "%rdx");
     #else                                    // 32 bit mode
        __asm__ __volatile__  (               // serialize (save ebx)
        "xorl %%eax,%%eax \n pushl %%ebx \n cpuid \n"
         ::: "%eax", "%ecx", "%edx");
        __asm__ __volatile__  (               // read TSC, store edx:eax in res
        "rdtsc\n"
         : "=a" (res[0]), "=d" (res[1]) );
        __asm__ __volatile__  (               // serialize again
        "xorl %%eax,%%eax \n cpuid \n popl %%ebx \n"
         ::: "%eax", "%ecx", "%edx");
     #endif
     #else
     // Inline assembly in MASM syntax
        __asm {
           xor eax, eax
           cpuid                              // serialize
           rdtsc                              // read TSC
           mov dword ptr res, eax             // store low dword in res[0]
           mov dword ptr res+4, edx           // store high dword in res[1]
           xor eax, eax
           cpuid                              // serialize again
        };
     #endif   // __GNUC__

     return *(__int64*)res;                   // return result
  }
};

class ProfilerLoopDp{

public:
  ProfilerLoopDp( std::string& aTitle );
  void start ( std::string& aName );
  void end ( );
  void dumpToFile( const char* aFileName, const char* aFileNameExtended );

private:
  std::string               mTitle;
  std::vector< std::vector<Measurement> >  mTimings;
  std::vector< std::string >  mBlockNames;
  std::map< std::string, int> mIds;
  int mCurrentId;
  __int64 mStart;
  __int64 mEnd;
  

private:
  __int64 time();

  __int64 ReadTSC() {
     int res[2];                              // store 64 bit result here

     #if defined(__GNUC__) && !defined(__INTEL_COMPILER)
     // Inline assembly in AT&T syntax

     #if defined (_LP64)                      // 64 bit mode
        __asm__ __volatile__  (               // serialize (save rbx)
        "xorl %%eax,%%eax \n push %%rbx \n cpuid \n"
         ::: "%rax", "%rcx", "%rdx");
        __asm__ __volatile__  (               // read TSC, store edx:eax in res
        "rdtsc\n"
         : "=a" (res[0]), "=d" (res[1]) );
        __asm__ __volatile__  (               // serialize again
        "xorl %%eax,%%eax \n cpuid \n pop %%rbx \n"
         ::: "%rax", "%rcx", "%rdx");
     #else                                    // 32 bit mode
        __asm__ __volatile__  (               // serialize (save ebx)
        "xorl %%eax,%%eax \n pushl %%ebx \n cpuid \n"
         ::: "%eax", "%ecx", "%edx");
        __asm__ __volatile__  (               // read TSC, store edx:eax in res
        "rdtsc\n"
         : "=a" (res[0]), "=d" (res[1]) );
        __asm__ __volatile__  (               // serialize again
        "xorl %%eax,%%eax \n cpuid \n popl %%ebx \n"
         ::: "%eax", "%ecx", "%edx");
     #endif
     #else
     // Inline assembly in MASM syntax
        __asm {
           xor eax, eax
           cpuid                              // serialize
           rdtsc                              // read TSC
           mov dword ptr res, eax             // store low dword in res[0]
           mov dword ptr res+4, edx           // store high dword in res[1]
           xor eax, eax
           cpuid                              // serialize again
        };
     #endif   // __GNUC__

     return *(__int64*)res;                   // return result
  }
};


#endif
