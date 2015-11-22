#include "funcs.hpp"



int main () {
    Double x=700.5;
    StringArray a;
    a=split("10,base,showpoitn",',');
    for(int i=0;i<(int)a.size();i++)print a[i]+NEW_LINE;
    
    print "--------"+NEW_LINE;
    parseIndexFlags("showbase/showbase/showpoint/width=6/precision=2/filler=@/left/scientific");
    printIndexFlags();
    print "--------"+NEW_LINE;
    print toStringAccordingToIFL(x,0,16)+NEW_LINE;
    print "--------"+NEW_LINE;
    
    std::map<String,Double> ns_rules;
    ns_rules["i"]=1;
    ns_rules["-i"]=1;
    ns_rules["ir"]=15;
    ns_rules["-ir"]=-15;
    ns_rules["id"]=2;
    ns_rules["idr"]=3;
    ns_rules["-id"]=-2;
    ns_rules["-idr"]=-3;
    ns_rules["dc"]=directory_count;
    print parseTrueFalse(existsInMap(ns_rules,String("fn")));
    
    print "--------"+NEW_LINE;
    print processExtendedNameString("/i-b10//ir-b16//-ir-b3/ gsf/ir-s/ /idr-s/ /-ir-l/",ns_rules,0);
    
    return 0;
}
