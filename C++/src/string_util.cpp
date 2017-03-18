/* 
 * string_util - String utilities available in Java that we want (or
 *               we want an easier interface to) in C++
 *
 * Contact: Jeff Maddalon (j.m.maddalon@nasa.gov), Cesar Munoz, George Hagen
 *
 * Copyright (c) 2011-2017 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#include "string_util.h"
#include "format.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string.h>
#if defined(_MSC_VER)
#include <regex>
#else
#include <regex.h>
#endif


namespace larcfm {
  
  using std::string;
  using std::vector;

    vector<string> split(const string& str,const string& delimiters) {
      vector<string> tokens;
      string::size_type lastPos = str.find_first_not_of(delimiters, 0);
      string::size_type pos     = str.find_first_of(delimiters, lastPos);
      while (string::npos != pos || string::npos != lastPos) {
    	tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
      }
      return tokens;
    }
  
    vector<string> split_empty(const string& str,const string& delimiters) {
      vector<string> tokens;
      string::size_type lastPos = 0; //str.find_first_not_of(delimiters, 0);
      string::size_type pos     = str.find_first_of(delimiters, lastPos);
      while (string::npos != pos) {
    	  //fpln(" www "+str.substr(lastPos, pos - lastPos));
    	  if (pos == lastPos) {
    		  tokens.push_back("");
    	  } else {
    	      tokens.push_back(str.substr(lastPos, pos - lastPos));
    	  }
        lastPos = pos+1;
        pos = str.find_first_of(delimiters, lastPos);
      }
      if (lastPos != str.size()) {
    	  tokens.push_back(str.substr(lastPos, str.size() - lastPos));
      }
      if (lastPos == str.size()) {
    	  tokens.push_back("");
      }
      return tokens;
    }


#if defined(_MSC_VER)
    vector<string> split_regex(const string& s, const string& rgx_str) {
        std::vector<std::string> elems;

        std::regex rgx(rgx_str);
        std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
        std::sregex_token_iterator end;

        while (iter != end)  {
            elems.push_back(*iter);
            ++iter;
        }

        return elems;

    }
#else
    vector<string> split_regex(const string& s, const string& rgx_str) {
    	std::vector<std::string> elems;

    	int reti;
    	regex_t rgx;
    	reti = regcomp(&rgx, rgx_str.c_str(), REG_EXTENDED);
    	if (reti != 0) {
    		fdln("Could not compile regex\n");
    	}

    	/* Execute regular expression */
		char between[100];
		int numchars;
    	regmatch_t matchptr[1];
    	int count = 0;
    	const int length = s.size();
		const char* cstr = s.c_str();
		//fpln("regex str="+rgx_str);
		//fpln("split_regex s.size()="+Fm2(length));
    	while (count < length) {
    		string m2(cstr+count);
    		//fpln("str = "+m2);
    		//fpln("count = "+Fm2(count));
        	reti = regexec(&rgx, cstr+count, 1, matchptr, 0);
    		if (!reti) {
    			numchars = matchptr[0].rm_so;
            	//fpln("l2 numchars="+Fm2(numchars));
    			strncpy(between,cstr+count,numchars);
    			between[numchars] = '\0';
    			string m1(between);
    			elems.push_back(m1);
    			//fpln("match1="+m1);

    			count = count + matchptr[0].rm_eo;
    		} else if (reti == REG_NOMATCH) {
    			numchars = length - count;
    			strncpy(between,cstr+count,numchars);
    			between[numchars] = '\0';
    			string m1(between);
    			elems.push_back(m1);
    			//fpln("match2="+m1);

    			count = s.size();
    		} else {
    		    regerror(reti, &rgx, between, sizeof(between));
    		    string m1(between);
    		    //fdln("Regex match failed: "+m1);
    		}

    	}

		//fpln("split_regex done!");
    	regfree(&rgx);
    	return elems;

    }

#endif

    string substring(const string& s, int begin, int end){
    	end = (end < (int) s.length()) ? end : s.length();
    	if (begin < end || begin < 0) {
        	return s.substr(begin,end-begin);
    	} else {
    		return "";
    	}
    }

    string substring(const string& s, int begin){
    	return substring(s, begin, s.length());
    }

    void trim(string& s,const string& drop){
      s.erase(s.find_last_not_of(drop)+1);
      s.erase(0,s.find_first_not_of(drop));
    }
  
    std::string trimCopy(const string& s,const string& drop){
    	std::string tmp(s);
    	trim(tmp, drop);
    	return tmp;
    }

  string toLowerCase(const string& strToConvert) {//change each element of the string to lower case
    string nstr;
    nstr.resize(strToConvert.size());

    for(unsigned int i=0;i<strToConvert.size();i++)  {
      nstr[i] = tolower(strToConvert[i]);
    }
    return nstr;  //return the converted string
    
  }
  
  string toUpperCase(const string& strToConvert) {//change each element of the string to upper case
    string nstr;
    nstr.resize(strToConvert.size());

    for(unsigned int i=0;i<strToConvert.size();i++)  {
      nstr[i] = toupper(strToConvert[i]);
    }
    return nstr;  //return the converted string

  }

  bool equals(const string& s1, const string& s2) {
    return (s1.compare(s2) == 0);
  }
  
  bool equalsIgnoreCase(const string& s1, const string& s2) {
    return (toLowerCase(s1).compare(toLowerCase(s2)) == 0);
  }
  
  void replace(std::string& s, char c1, char c2) {
	  std::replace(s.begin(), s.end(), c1, c2);
  }

  bool contains(const std::vector<std::string>& v, const std::string& s) {
	  for(int i = 0; i < (int) v.size(); i++) {
		  if (equals(v[i],s)) return true;
	  }
	  return false;
  }

  bool contains(const std::string& str, const std::string& sub) {
	  return str.find(sub) != string::npos;
  }

  bool startsWith(const std::string& str, const std::string& prefix) {
	  return str.substr(0,prefix.length()).compare(prefix) == 0;
  }

  bool endsWith(const std::string& str, const std::string& suffix) {
	  return suffix.length() <= str.length() && str.substr(str.length() - suffix.length()).compare(suffix) == 0;
  }


  int parseInt(const std::string& str) {
	  //the C++11 version: return std::stoi(str);
	  std::istringstream buffer(str);
	  int value;
	  buffer >> value;
	  return value;
  }

}
