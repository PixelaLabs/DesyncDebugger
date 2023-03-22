#include "AppUI.h"

#include "AppState.h"

#include "imgui.h"

static ImGuiTextFilter LogFilter;
static ImGuiTextFilter EntriesFilter;
static ImGuiTextFilter CategoryFilter;

void AppUI::DrawUI(AppState& AppState)
{
    DrawMainMenu(AppState);
    DrawLogView(AppState);

    DrawEntryView();
    DrawCategoryView();

    if (bShowControlPanelWindow)
    {
        DrawControlPanelView(AppState);
    }
}

void AppUI::DrawMainMenu(AppState& AppState)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Control Panel"))
            {
                bShowControlPanelWindow = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void AppUI::DrawEntryView()
{
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Entries");

    if (ImGui::Button("X"))
    {
        EntriesFilter.Clear();
    }
    ImGui::SameLine();
    EntriesFilter.Draw("Filter", -100.0f);
    
    ImGui::Separator();

    if (ImGui::BeginChild("EntryViewScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        for (auto const& Data : EntryData)
        {
            auto const& EntryName = Data.first;
            bool SyncState = Data.second;

            if (!EntriesFilter.PassFilter(EntryName.c_str()))
            {
                continue;
            }

            if (SyncState)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.06f, 0.6f, 0.06f, 0.94f));
                ImGui::Text("V");
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.06f, 0.06f, 0.94f));
                ImGui::Text("X");
            }

            ImGui::PopStyleColor();

            ImGui::SameLine();
            if (ImGui::Selectable(EntryName.c_str(), false))
            {
                LogFilter.Clear();

                std::strcpy(LogFilter.InputBuf, EntryName.c_str());

                LogFilter.Build();
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

void AppUI::DrawCategoryView()
{
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Categories");

    if (ImGui::Button("X"))
    {
        CategoryFilter.Clear();
    }
    ImGui::SameLine();
    CategoryFilter.Draw("Filter", -100.0f);

    ImGui::Separator();

    if (ImGui::BeginChild("CategoryViewScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        for (auto const& CategoryName : CategoryNames)
        {
            if (!CategoryFilter.PassFilter(CategoryName.c_str()))
            {
                continue;
            }

            if (ImGui::Selectable(CategoryName.c_str(), false))
            {
                LogFilter.Clear();

                std::strcpy(LogFilter.InputBuf, CategoryName.c_str());

                LogFilter.Build();
            }
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

void AppUI::DrawLogView(AppState& AppState)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Compare Log");

    ImGui::End();

    static bool AutoScroll = true;

    if (!ImGui::Begin("Compare Log"))
    {
        ImGui::End();
        return;
    }
    
    static MsgType MsgCurrentState = MsgType::All;

    if (ImGui::RadioButton("[All]", MsgCurrentState == MsgType::All))
    {
        auto const& Results = AppState.GetComparisonResults();

        FilteredMsgs.clear();
        Results.FilterByMsgType(MsgType::All, FilteredMsgs);

        MsgCurrentState = MsgType::All;
    }


    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 0.94f));
    if (ImGui::RadioButton("[Sync]", MsgCurrentState == MsgType::Sync))
    {
        auto const& Results = AppState.GetComparisonResults();

        FilteredMsgs.clear();
        Results.FilterByMsgType(MsgType::Sync, FilteredMsgs);

        MsgCurrentState = MsgType::Sync;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 0.94f));
    if (ImGui::RadioButton("[Desync]", MsgCurrentState == MsgType::Desync))
    {
        auto const& Results = AppState.GetComparisonResults();

        FilteredMsgs.clear();
        Results.FilterByMsgType(MsgType::Desync, FilteredMsgs);

        MsgCurrentState = MsgType::Desync;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::Button("X"))
    {
        LogFilter.Clear();
    }
    ImGui::SameLine();
    LogFilter.Draw("Filter", -100.0f);

    ImGui::Separator();

    if (ImGui::BeginChild("LogViewScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        int StringIdx = 324820348390;

        for (auto const& Msg : FilteredMsgs)
        {
            if (LogFilter.PassFilter(Msg.Entry.GetName().c_str())
                || LogFilter.PassFilter(Msg.Entry.GetInfo().c_str())
                || LogFilter.PassFilter(Msg.Entry.GetCategory().c_str()))
            {
                if (Msg.Type == MsgType::Sync)
                {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[Sync]");
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[Desync]");
                }

                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Frame[%d]", Msg.FrameIdx);

                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "[%s]", Msg.Entry.GetCategory().c_str());

                ImGui::SameLine();
                ImGui::Text("Entry:");

                ImGui::SameLine();

                ImGui::PushID(StringIdx);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.0f, 0.8f, 0.94f));

                if (ImGui::Selectable(Msg.Entry.GetName().c_str(), false,
                    ImGuiSelectableFlags_None, ImGui::CalcTextSize(Msg.Entry.GetName().c_str())))
                {
                    ImGuiIO& io = ImGui::GetIO();
                    io.SetClipboardTextFn(nullptr, Msg.Entry.GetName().c_str());
                }

                ImGui::PopStyleColor();

                ImGui::SameLine();
                ImGui::Text("Info:");

                ImGui::PushID(StringIdx);

                ImGui::SameLine();
                
                if (ImGui::Selectable(Msg.Entry.GetInfo().c_str(), false, 
                    ImGuiSelectableFlags_None, ImGui::CalcTextSize(Msg.Entry.GetInfo().c_str())))
                {
                    ImGuiIO& io = ImGui::GetIO();
                    io.SetClipboardTextFn(nullptr, Msg.Entry.GetInfo().c_str());
                }

                ImGui::PopID();
                ImGui::PopID();

                StringIdx += 2;
            }
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

void AppUI::DrawControlPanelView(AppState& AppState)
{
    ImGui::SetNextWindowSize(ImVec2(800, 80), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Control Panel", &bShowControlPanelWindow))
    {
        static char WorkingDirectoryPath[1024] = { '\0' };
        strcpy_s(WorkingDirectoryPath, AppState.GetSearchFolder().c_str());

        if (ImGui::InputText("Working Directory", WorkingDirectoryPath, 1024))
        {
            AppState.SetSearchFolder(WorkingDirectoryPath);
        }

        if (ImGui::Button("Load Logs"))
        {
            UpdateClusterDataUI(AppState);
        }

        ImGui::End();
    }
}

void AppUI::UpdateClusterDataUI(AppState& AppState)
{
    AppState.UpdateClusterData();

    FilteredMsgs.clear();
    EntryData.clear();
    CategoryNames.clear();

    auto const& Results = AppState.GetComparisonResults();

    Results.FilterByMsgType(MsgType::All, FilteredMsgs);

    Results.GetEntryData(EntryData);
    Results.GetCategoryNames(CategoryNames);
}
