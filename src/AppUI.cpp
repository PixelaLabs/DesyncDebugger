#include "AppUI.h"

#include "AppState.h"

#include "imgui.h"
#include "implot.h"

static ImGuiTextFilter LogFilter;
static ImGuiTextFilter EntriesFilter;
static ImGuiTextFilter CategoryFilter;

void AppUI::DrawUI(AppState& AppState)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

    DrawMainMenu(AppState);
    DrawLogView(AppState);

    DrawEntryView();
    DrawCategoryView();

    if (bShowControlPanelWindow)
    {
        DrawControlPanelView(AppState);
    }
    
    DrawFrameDebuggerView(AppState);

    ImGui::PopStyleVar();
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

                FilterMessages(TypeFilteredMsgs, FilteredMsgs);
            }
        }
    }
    ImGui::EndChild();

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

                FilterMessages(TypeFilteredMsgs, FilteredMsgs);
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void AppUI::DrawLogView(AppState& AppState)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Compare Log"))
    {
        static bool AutoScroll = true;

        static MsgType MsgCurrentState = MsgType::All;

        if (ImGui::RadioButton("[All]", MsgCurrentState == MsgType::All))
        {
            auto const& Results = AppState.GetComparisonResults();

            Results.FilterByMsgType(MsgType::All, TypeFilteredMsgs);
            FilterMessages(TypeFilteredMsgs, FilteredMsgs);

            MsgCurrentState = MsgType::All;
        }

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 0.94f));
        if (ImGui::RadioButton("[Sync]", MsgCurrentState == MsgType::Sync))
        {
            auto const& Results = AppState.GetComparisonResults();

            Results.FilterByMsgType(MsgType::Sync, TypeFilteredMsgs);
            FilterMessages(TypeFilteredMsgs, FilteredMsgs);

            MsgCurrentState = MsgType::Sync;
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 0.94f));
        if (ImGui::RadioButton("[Desync]", MsgCurrentState == MsgType::Desync))
        {
            auto const& Results = AppState.GetComparisonResults();

            Results.FilterByMsgType(MsgType::Desync, TypeFilteredMsgs);
            FilterMessages(TypeFilteredMsgs, FilteredMsgs);

            MsgCurrentState = MsgType::Desync;
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();
        if (ImGui::Button("X"))
        {
            LogFilter.Clear();
            FilterMessages(TypeFilteredMsgs, FilteredMsgs);
        }
        ImGui::SameLine();
        LogFilter.Draw("Filter", -100);

        ImGui::Separator();
        
        if (ImGui::BeginChild("LogViewScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

            ImGuiListClipper Clipper;
            Clipper.Begin(FilteredMsgs.size());

            while (Clipper.Step())
            {
                for (int MsgIdx = Clipper.DisplayStart; MsgIdx < Clipper.DisplayEnd; MsgIdx++)
                {
                    const auto& Msg = FilteredMsgs[MsgIdx];

                    ImGui::PushID(MsgIdx);

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
                    ImGui::Text("[");

                    for (size_t NodeIdx = 0; NodeIdx < Msg.LineIndices.size(); NodeIdx++)
                    {
                        int LineIdx = Msg.LineIndices[NodeIdx];

                        ImGui::SameLine();

                        if (LineIdx == -1)
                        {
                            ImGui::Button("None");
                        }
                        else
                        {
                            if (ImGui::Button(std::to_string(LineIdx).c_str()))
                            {
                                std::string NodeFileName = "Node_" + std::to_string(NodeIdx) + ".log";

                                std::string LineName = ":" + std::to_string(Msg.LineIndices[NodeIdx]);

                                std::string Cmd = std::string("Code.exe --goto \"" + AppState.GetSearchFolder() + "\\"
                                    + NodeFileName + LineName + "\" &");

                                std::system(Cmd.c_str());
                            }
                        }

                        if (NodeIdx != Msg.LineIndices.size() - 1)
                        {
                            ImGui::SameLine();
                            ImGui::Text(",");
                        }
                    }

                    ImGui::SameLine();
                    ImGui::Text("]");

                    ImGui::SameLine();
                    ImGui::Text("Entry:");

                    ImGui::SameLine();

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

                    ImGui::SameLine();

                    if (ImGui::Selectable(Msg.Entry.GetInfo().c_str(), false,
                        ImGuiSelectableFlags_None, ImGui::CalcTextSize(Msg.Entry.GetInfo().c_str())))
                    {
                        ImGuiIO& io = ImGui::GetIO();
                        io.SetClipboardTextFn(nullptr, Msg.Entry.GetInfo().c_str());
                    }

                    ImGui::PopID();
                }
            }

            ImGui::PopStyleVar();
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
    }

    ImGui::End();
}

void AppUI::DrawFrameDebuggerView(AppState& AppState)
{
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Frame Plot", &bShowControlPanelWindow))
    {
        ImGui::PushID(123);
        if (ImGui::Button("X"))
        {
            CurrentFrameFilterIndex = -1;
            FilterMessages(TypeFilteredMsgs, FilteredMsgs);
        }
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushItemWidth(-100);
        if (ImGui::InputInt("Filter", &CurrentFrameFilterIndex, 1))
        {
            CurrentFrameFilterIndex = std::max(-1, CurrentFrameFilterIndex);
        }
        ImGui::PopItemWidth();

        if (ImPlot::BeginPlot("###Frames")) 
        {
            ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            
            int SelectedFrame = -1;
            
            if (ImPlot::IsPlotHovered())
            {
                ImPlotPoint PlotPoint = ImPlot::GetPlotMousePos();

                SelectedFrame = std::min(int(SyncedFrames.size()), int(PlotPoint.x));

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    CurrentFrameFilterIndex = SelectedFrame;
                    FilterMessages(TypeFilteredMsgs, FilteredMsgs);
                }
            }

            if (!SyncedFrames.empty())
            {
                std::vector<float> SelectedFrameLocal = { 0.0f };

                ImVec4 SelectedFrameColor;;

                if (SelectedFrame != -1)
                {
                    if (SyncedFrames[SelectedFrame] > 0.0f)
                    {
                        SelectedFrameLocal[0] = 0.5f;
                        SelectedFrameColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
                    }
                    else
                    {
                        SelectedFrameLocal[0] = 1.0f;
                        SelectedFrameColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
                    }
                }

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(0.3f, 0.9f, 0.3f, 0.5f));
                ImPlot::PlotBars("[Sync]", SyncedFrames.data(), SyncedFrames.size(), 0.1, 0.0f);
                ImPlot::PopStyleColor();

                ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(0.8f, 0.0f, 0.0f, 0.5f));
                ImPlot::PlotBars("[Desync]", UnsyncedFrames.data(), UnsyncedFrames.size(), 0.1, 0.0f);
                ImPlot::PopStyleColor();

                ImPlot::PushStyleColor(ImPlotCol_Fill, SelectedFrameColor);
                ImPlot::PlotBars("##[SelectedFrame]", SelectedFrameLocal.data(), SelectedFrameLocal.size(), 0.1, SelectedFrame);
                ImPlot::PopStyleColor();
            }

            ImPlot::EndPlot();
        }

        ImGui::End();
    }
}

void AppUI::UpdateClusterDataUI(AppState& AppState)
{
    AppState.UpdateClusterData();

    CurrentFrameFilterIndex = -1;

    TypeFilteredMsgs.clear();
    FilteredMsgs.clear();
    EntryData.clear();
    CategoryNames.clear();

    auto const& Results = AppState.GetComparisonResults();

    Results.FilterByMsgType(MsgType::All, TypeFilteredMsgs);
    FilterMessages(TypeFilteredMsgs, FilteredMsgs);

    Results.GetEntryData(EntryData);
    Results.GetCategoryNames(CategoryNames);
    Results.GetSyncFramesState(SyncedFrames);

    UnsyncedFrames.resize(SyncedFrames.size());

    for (size_t i = 0; i < SyncedFrames.size(); i++)
    {
        UnsyncedFrames[i] = (SyncedFrames[i]) == 0.5f ? 1.0f : 0.0f;
        SyncedFrames[i] = (UnsyncedFrames[i]) == 0.0f ? 0.5f : 0.0f;
    }
}

void AppUI::FilterMessages(std::vector<MsgEntry> const& InMsgs, std::vector<MsgEntry>& OutMessages)
{
    OutMessages.clear();

    for (auto const& Msg : InMsgs)
    {
        if (CurrentFrameFilterIndex != -1)
        {
            if (CurrentFrameFilterIndex != int(Msg.FrameIdx))
            {
                continue;
            }
        }

        if (LogFilter.PassFilter(Msg.Entry.GetName().c_str())
            || LogFilter.PassFilter(Msg.Entry.GetInfo().c_str())
            || LogFilter.PassFilter(Msg.Entry.GetCategory().c_str()))
        {
            OutMessages.push_back(Msg);
        }
    }
}
