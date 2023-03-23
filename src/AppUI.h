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
    void DrawControlPanelView(AppState& AppState);
    void DrawFrameDebuggerView(AppState& AppState);

protected:
    void UpdateClusterDataUI(AppState& AppState);

protected:
    int CurrentFrameFilterIndex = -1;

    bool bShowControlPanelWindow = true;

    std::vector<float> SyncedFrames;
    std::vector<float> UnsyncedFrames;

    // key - Entry Name, value - Sync state
    std::map<std::string, bool> EntryData;

    std::vector<std::string> CategoryNames;
    std::vector<MsgEntry> FilteredMsgs;
};
