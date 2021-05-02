#include "StringUtils.hpp"

// Func to split a string with a given char
vector<string> StringUtils::split(string str, char delimiter) {
	vector<string> strVct;
	unsigned int relStart = 0;
	for (unsigned int i = 0; i < str.size(); i++) {
		if (str[i] == delimiter) {
			// Hacemos el substring y despues lo añadimos al vector str
			strVct.push_back(getSubstring(str, relStart, i));
			relStart = i+1;
			// Si el delimitador esta en el ultimo caracter sacamos el vector
			if (relStart >= str.size()) {
				return strVct;
			}
		}
	}
	strVct.push_back(getSubstring(str, relStart, str.size()));
	return strVct;
}

/* Example output: split(str, {'+', '-'}, true)
 * "1+2*2+3-4+5" -> [1, +2*2, +3, -4, +5]
 */
vector<string> StringUtils::split(string str, char delimiters[], unsigned int delimitersSize, bool addDelim) {
    vector<string> strVct;
    unsigned int relStart = 0;
    for (unsigned int i = 0; i < str.size(); i++) {
        for (unsigned int j = 0; j < delimitersSize; j++) {
            if (str[i] == delimiters[j]) {
                // Do substring
                strVct.push_back(getSubstring(str, relStart, i));
                relStart = addDelim ? i : i+1;
            }
        }
    }
    strVct.push_back(getSubstring(str, relStart, str.size()));
    return strVct;
}

//
// Substrings
//


// Func to do substring having an interval
string StringUtils::getSubstring(string str, unsigned int start, unsigned int end) {
	string result = "";
	for (unsigned int i = start; i < end; i++) {
        result += str[i];
	}
	return result;
}

// Func to remove last characters of a string (has to be a negative number)
string StringUtils::getSubstring(string str, int fromEnd) {
	string result = "";
	if (fromEnd < 0) {
		for (int i = 0; i < (fromEnd + (int)str.size()); i++) {
			result.push_back(str[i]);
		}
	}
	return result;
}

//
// Replace
//

// Func to replace all chars that matches with another
string StringUtils::replaceAll(string str, char regex, char replacement) {
    for (unsigned int i = 0; i < str.size(); i++) {
        if (str[i] == regex) str[i] = replacement;
    }
    return str;
}

// Func to replace all strs that matches with another
string StringUtils::replaceAll(string str, string regex, string replacement) {
    int relEnd = 0;
    for (unsigned int i = 0; i < str.size(); i++) {
        // we check if the first char of regex matches with current char
        if (str[i] == regex[0]) {
            relEnd = regex.size()+i;
            if (getSubstring(str, i, relEnd) == regex) {
                str = getSubstring(str, 0, i) + replacement
                    + getSubstring(str, relEnd, str.size());
                i--; // i dont know why, but mustnt remove it
            }
        }
    }
    return str;
}

string StringUtils::dropIndex(string str, unsigned int idx) {
    return getSubstring(str, 0, idx)+getSubstring(str, idx+1, str.size());
}

//
// Converters
//

string StringUtils::tostring(int value) {
    ostringstream strs;
    strs << value;
    return strs.str();
}

string StringUtils::tostring(float value) {
    ostringstream strs;
    strs << value;
    return strs.str();
}

string StringUtils::tostring(long double value) {
    ostringstream strs;
    strs << value;
    return strs.str();
}

string StringUtils::tostring(char value) {
    string str;
    str.push_back(value);
    return str;
}
