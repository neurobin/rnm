#include "funcs.hpp"
#include "times.hpp"
#include <functional>





int main(){
    print fileExtension(".config")<<NEW_LINE;
    print fileNameWithoutExtension("fdsfsd..fsdaf..fsd.jp.config")<<NEW_LINE;
    print "------------------------"<<NEW_LINE;
    print funcTime(countCharInString,"precision=10","=")<<NEW_LINE;
    print "------------------------"<<NEW_LINE;
    print funcTime(countMatchInString,"I am a simple and very simple string", "simple")<<NEW_LINE;
    print "------------------------"<<NEW_LINE;
    print funcTime(countCharWithAlgorithm,"precision=10",'=')<<NEW_LINE;
    print "------------------------"<<NEW_LINE;
    print "parse1: "<<funcTime(parseTwoStepIndexFlag,"precision=12")<<NEW_LINE;
    print "------------------------"<<NEW_LINE;
    print "parse2: "<<funcTime(parseTwoStepIndexFlag,"precision=12")<<NEW_LINE;
    std::cout<<"norm: "<<funcTime(countCharInString,"precision=10","=")<<"\n";
    std::cout<<"algo: "<<funcTime(countCharWithAlgorithm,"precision=10",'=');
    String s;
    getLine(s);
    main();
	return 0;
}
