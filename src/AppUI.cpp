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
}

void AppUI::DrawMainMenu(AppState& AppState)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load Log Files"))
            {
                AppState.UpdateClusterData("");

                FilteredMsgs.clear();
                EntryData.clear();
                CategoryNames.clear();

                auto const& Results = AppState.GetComparisonResults();

                Results.FilterByMsgType(MsgType::All, FilteredMsgs);

                Results.GetEntryData(EntryData);
                Results.GetCategoryNames(CategoryNames);
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

    if (ImGui::Button("[All]"))
    {
        auto const& Results = AppState.GetComparisonResults();

        FilteredMsgs.clear();
        Results.FilterByMsgType(MsgType::All, FilteredMsgs);
    }

    ImGui::SameLine();
    if (ImGui::Button("[Sync]"))
    {
        auto const& Results = AppState.GetComparisonResults();

        FilteredMsgs.clear();
        Results.FilterByMsgType(MsgType::Sync, FilteredMsgs);
    }
    ImGui::SameLine();
    if (ImGui::Button("[Desync]"))
    {
        auto const& Results = AppState.GetComparisonResults();

        FilteredMsgs.clear();
        Results.FilterByMsgType(MsgType::Desync, FilteredMsgs);
    }

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
        LogStrings.resize(FilteredMsgs.size());

        int StringIdx = 0;

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

                const size_t StringBufferLength = 256;
                LogStrings[StringIdx].resize(StringBufferLength, 0);

                ImGui::SameLine();
                ImGui::Text("Entry:");

                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", Msg.Entry.GetName().c_str());

                std::snprintf(&LogStrings[StringIdx][0], StringBufferLength, "Info: %s", Msg.Entry.GetInfo().c_str());

                ImGui::SameLine();
                if (ImGui::Selectable(LogStrings[StringIdx].c_str()))
                {

                }

                StringIdx++;
            }
        }

        ImGui::EndChild();
    }

    ImGui::End();
}
