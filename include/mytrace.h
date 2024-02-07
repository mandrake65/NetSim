#ifndef _MYTRACE_H_
#define _MYTRACE_H_


#include <iostream>
#include <mutex>

#define TRACE_CLASS()  {  std::lock_guard<std::mutex> guard(trace_mutex); std::cout<<typeid(*this).name() <<"::"<<__func__ <<std::endl;}
#define TRACE_CLASS_MSG(msg)  {std::lock_guard<std::mutex> guard(trace_mutex); std::cout<<typeid(*this).name() <<"::"<<__func__ << " "<< msg<<std::endl;}
#define TRACE_MSG(msg)  {std::lock_guard<std::mutex> guard(trace_mutex); std::cout<<"::"<<__func__ << " "<< msg<<std::endl;}
extern std::mutex trace_mutex;

#endif // !_MYTRACE_H_
