#ifndef JSONFUNCS_H
#define JSONFUNCS_H

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>

#include <Windows.h>
#include <fstream>
#include <array>

#include <lua.hpp>
#include "LuaCore.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "XMLData.h"
using namespace std;

template <typename pseudomap>
inline void ArrayToJson(rapidjson::Document* doc, const char* name, const pseudomap& array) {
	rapidjson::Value mapjson(rapidjson::kObjectType);
	for (const auto& entry : array) {
		rapidjson::Value key(entry.first.c_str(), doc->GetAllocator());
		mapjson.AddMember(key, entry.second, doc->GetAllocator());
	}
	doc->AddMember(rapidjson::StringRef(name), mapjson, doc->GetAllocator());
}

template <typename pseudomap>
inline void JsonToArray(const rapidjson::Value& jsonValue, pseudomap& array) {
	for (auto itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); ++itr) {
		array[itr->name.GetString()] = itr->value.GetInt();
	}
}



template <typename pseudomap2>
inline void JsonToArrayOfArray(const rapidjson::Value& jsonValue, pseudomap2& array) {
	for (auto itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); ++itr) {
		string key = itr->name.GetString();
		typename pseudomap2::value_type::second_type innerArray;
		const rapidjson::Value& jsonArray = itr->value;
		for (rapidjson::SizeType i = 0; i < jsonArray.Size(); ++i) {
			if (jsonArray[i].IsInt()) {
				innerArray[i] = jsonArray[i].GetInt();
			}
		}
		array[key] = innerArray;
	}
}

template <typename pseudomap2>
inline void ArrayOfArrayToJson(rapidjson::Document* doc, const char* name, const pseudomap2& array) {
	rapidjson::Value mapjson(rapidjson::kObjectType);
	for (const auto& entry : array) {
		rapidjson::Value key(entry.first.c_str(), doc->GetAllocator());

		rapidjson::Value innerArrayValue(rapidjson::kArrayType);
		for (const auto& element : entry.second) {
			innerArrayValue.PushBack(element, doc->GetAllocator());
		}
		mapjson.AddMember(key, innerArrayValue, doc->GetAllocator());
	}
	doc->AddMember(rapidjson::StringRef(name), mapjson, doc->GetAllocator());
}



inline rapidjson::Document GetJsonDoc(string* dir) {
	ifstream ifs(*dir);
	string directory = dir->substr(0, dir->find_last_of("\\/"));
	if (!CreateDirectory(directory.c_str(), NULL)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			return nullptr;
		}
	}
	string content((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));
	rapidjson::Document doc;
	doc.Parse(content.c_str());
	return doc;
}


#endif