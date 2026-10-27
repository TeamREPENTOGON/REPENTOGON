#include <cstring>

#include "SymbolNameReader.h"

static bool ParseVisibility(char** text, SymbolName& name) {
  char* result = strchr(*text, ' ');
  /* There must be at least a space somewhere because we expect the return type. */
  if (!result) {
    fprintf(stderr, "[visibility] Malformed mangled name '%s': no space\n",
	    name._raw.c_str());
    return false;
  }

  *result = 0;

  if (result[-1] == ':') {
    name._visibility = std::string(*text, result - 1);
    *text = result + 1;
  } else {
    *result = ' ';
  }

  return true;
}

static bool ParseQualifier(char** text, SymbolName& name) {
  char* result = strchr(*text, ' ');
  /* There must be at least a space somewhere because we expect the return type. */
  if (!result) {
    fprintf(stderr, "[qualifier] Malformed mangled name '%s': no space\n",
	    name._raw.c_str());
    return false;
  }

  *result = 0;

  if (!strcmp(*text, "static") || !strcmp(*text, "virtual")) {
    name._qualifier = std::string(*text);
    *text = result + 1;
  } else {
    *result = ' ';
  }

  return true;
}

bool IsCallingConvention(const char* s) {
  return !strcmp(s, "__cdecl") || !strcmp(s, "__thiscall") ||
    !strcmp(s, "__stdcall") || !strcmp(s, "__fastcall");
}

static bool ParseReturnType(char** text, SymbolName& name) {
  /* The return type is harder to parse because it can be split into multiple
   * tokens and we need to look ahead to determine if we've reached either the
   * calling convention or the name.
   *
   * Immediately look for the opening parenthesis of the arguments. This gives
   * us a boundary we are not allowed to cross. As for the calling convention,
   * we need to scan the entire word to determine whether it is a calling
   * convention or not, as __ is a valid lead for a type name.
   */
  char* startArgs = strchr(*text, '(');
  if (!startArgs) {
    fprintf(stderr, "[return] Malformed mangled name '%s': no args\n",
	    name._raw.c_str());
    return false;
  }

  /* Zero the beginning of the arguments so strchr will not try to read into
   * the args (or beyond).
   */
  *startArgs = 0;

  char* limit = NULL;
  std::string type;
  while ((limit = strchr(*text, ' '))) {
    *limit = 0;

    /* We need special treatment for "operator" due to it not being part of the
     * type but still being hidden after a space.
     */
    if (IsCallingConvention(*text) || !strcmp(*text, "operator")) {
      *limit = ' ';
      break;
    }

    if (!type.empty())
      type.append(" ");

    type.append(*text);
    *text = limit + 1;
  }

  name._returnType = type;
  /* We are guaranteed to encounter a space before the name of the function.
   * Once we are here, we may, at worst, have seen the calling convention, and
   * there's nothing to do. parse_convention will read it.
   */
  *startArgs = '(';
  return true;
}

static bool ParseCallingConvention(char** text, SymbolName& name) {
  /* Set the beginning of args to NUL. This way, we can look for the next space
   * without risking missing it or accidentally reading args while looking for
   * the convention.
   */
  char* startArgs = strchr(*text, '(');
  if (!startArgs) {
    fprintf(stderr, "[convention] Malformed mangled name '%s': no args\n",
	    name._raw.c_str());
    return false;
  }

  *startArgs = 0;
  char* limit = strchr(*text, ' ');
  /* No calling convention. */
  if (!limit) {
    *startArgs = '(';
    return true;
  }

  *limit = 0;

  if (!IsCallingConvention(*text)) {
    fprintf(stderr, "[convention] Malformed mangled name '%s': '%s' is not a convention\n", 
	    name._raw.c_str(), *text);
    return false;
  }

  name._callingConvention = *text;

  *text = limit + 1;
  *startArgs = '(';

  return true;
}

static bool ParseName(char** text, SymbolName& name) {
  char* startArgs = strchr(*text, '(');
  if (!startArgs) {
    fprintf(stderr, "[name] Malformed mangled name '%s': no args\n",
	    name._raw.c_str());
    return false;
  }

  *startArgs = 0;
  name._name = *text;
  *text = startArgs + 1;

  return true;
}

static bool ParseArgs(char** text, SymbolName& name) {
  /* We need to split on comma. However, because commas can appear inside
   * template parameters, we cannot rely on strchr alone. Read char by char,
   * stacking template brackets as we encounter them, and drop once we're done.
   */
  char* s = *text;
  int count = 0;
  while (*s) {
    if (*s == ',' && count == 0) {
      *s = 0;
      name._args.push_back(*text);
      *text = s + 1;
    }

    if (*s == '<') {
      ++count;
    } else if (*s == '>') {
      if (count == 0) {
	fprintf(stderr, "[args] Malformed mangled name '%s': unbalanced template\n",
		name._raw.c_str());
	return false;
      }

      --count;
    }

    ++s;
  }

  if (**text != ')') {
    *(s - 1) = 0;
    name._args.push_back(*text);
  }

  return true;
}

bool SymbolNameReader::ReadSymbolName(const char* mangledName, const char* text,
				      SymbolName& symbol) {
    symbol._mangledName = mangledName;
    symbol._raw = text;

    char* raw = (char*)malloc(strlen(text) + 1);
    char* orig = raw;
    strcpy(raw, text);

    if (!ParseVisibility(&raw, symbol)) {
	return false;
    }

    if (!ParseQualifier(&raw, symbol)) {
	return false;
    }

    if (!ParseReturnType(&raw, symbol)) {
	return false;
    }

    if (!ParseCallingConvention(&raw, symbol)) {
	return false;
    }

    if (!ParseName(&raw, symbol)) {
	return false;
    }

    if (!ParseArgs(&raw, symbol)) {
	return false;
    }

    free(orig);
    return true;
}
