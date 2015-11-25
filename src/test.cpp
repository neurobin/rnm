#include "funcs.hpp"


String processPdNameString(String ns,String file){
    String name=ns;
    int subm[]={0,1,2,3,4};
    Regex multi_re (""+path_delim+"pd(\\d+)-?(\\d*)-?([^/]*)("+path_delim+")");
    RegexTokenIterator end; ///default constructor=end of sequence
    RegexTokenIterator toit (ns.begin(), ns.end(), multi_re,subm);
    while (toit != end){
        String tot,si, ei,delim;
        tot=*toit++;si=*toit++;ei=*toit++;delim=*toit++;toit++;
        print tot+NEW_LINE;print si+NEW_LINE;print ei+NEW_LINE;print delim+NEW_LINE;
        
        //~ if(basenp=="b"){
            //~ if(isPositiveInt(basep)){
                //~ base=stringTo<int>(basep);
                //~ if(base>=NUM_BASE_MIN&&base<=NUM_BASE_MAX){
                    //~ if(existsInMap(ns_rules,String(rulep))){
                        //~ name=replaceString(name,tot,toStringAccordingToIFL(ns_rules[rulep],index_field_length,base));
                    //~ }
                //~ }
            //~ }
        //~ }
    }
return name;
}

StringArray cpdn;

int main(){
    String f="/media/jahid/softs/iso images/windows/kfljsl.iso";
    String d="/home/jahidul/Downloads/sofs/instant/more-instant";
    processPdNameString("/pd012/ /pd1278/ /pd120-0/ /pd9098-032-4324/ /pd90-fsdf/ /pd09fsd/ /pd-324/",f);
    }
