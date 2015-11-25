
#ifndef __TIMES_HPP
#define __TIMES_HPP

#include <chrono>

typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()

#define timeNow() std::chrono::high_resolution_clock::now()


template<typename Func, typename... Args>
void process(Func func, Args&&... args) {
    func(std::forward<Args>(args)...);
}

template<typename F, typename... Args>
double funcTime(F func, Args&&... args){
    TimeVar t1=timeNow();
    func(std::forward<Args>(args)...);
    return duration(timeNow()-t1);
}






#endif
