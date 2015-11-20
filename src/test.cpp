#include "classes.hpp"
template<typename T>
String toString(T a){
    Stream ss;
    ss <<a;
    return ss.str();
}

char * normalizePath(char* pwd, const char * src, char* res) {
	size_t res_len;
	size_t src_len = strlen(src);

	const char * ptr = src;
	const char * end = &src[src_len];
	const char * next;

	if (src_len == 0 || src[0] != '/') {
		// relative path
		size_t pwd_len;

		pwd_len = strlen(pwd);
		memcpy(res, pwd, pwd_len);
		res_len = pwd_len;
	} else {
		res_len = 0;
	}

	for (ptr = src; ptr < end; ptr=next+1) {
		size_t len;
		next = (char*)memchr(ptr, '/', end-ptr);
		if (next == NULL) {
			next = end;
		}
		len = next-ptr;
		switch(len) {
		case 2:
			if (ptr[0] == '.' && ptr[1] == '.') {
				const char * slash = (char*)memrchr(res, '/', res_len);
				if (slash != NULL) {
					res_len = slash - res;
				}
				continue;
			}
			break;
		case 1:
			if (ptr[0] == '.') {
				continue;
			}
			break;
		case 0:
			continue;
		}

		if (res_len != 1)
			res[res_len++] = '/';
		
		memcpy(&res[res_len], ptr, len);
		res_len += len;
	}

	if (res_len == 0) {
		res[res_len++] = '/';
	}
	res[res_len] = '\0';
	return res;
}


String getAbsolutePath(String x){
    char path[FILENAME_MAX+1];
    if(follow_symlink){
        realpath(x.c_str(),path);
        return toString(path);
    }
    else {
        return toString(normalizePath((char*)CWDN.c_str(),(const char*)x.c_str(),path));
    }
    
}

int main(){
    char s[FILENAME_MAX];
    getCurrentDir(s);
    CWDN=toString(s);
    print normalizePath(s,"test.txt.link",s)+NEW_LINE;
    print getAbsolutePath("test.txt.link");
    
    return 0;}
