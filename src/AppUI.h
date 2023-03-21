#pragma once

#include <vector>
#include <map>

#include <string>

class AppState;
struct MsgEntry;
enum class MsgType;

class AppUI
{
public:
    void DrawUI(AppState& AppState);

protected:
    void DrawMainMenu(AppState& AppState);
    void DrawEntryView();
    void DrawCategoryView();
    void DrawLogView(AppState& AppState);

protected:
    // key - Entry Name, value - Sync state
    std::map<std::string, bool> EntryData;

    std::vector<std::string> LogStrings;
    std::vector<std::string> CategoryNames;

    std::vector<MsgEntry> FilteredMsgs;
};
