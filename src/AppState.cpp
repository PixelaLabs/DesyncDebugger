#include "AppState.h"

#include <filesystem>

AppState::AppState()
{
	std::filesystem::path CurrentWorkingDirectory = std::filesystem::current_path();
	SearchFolder = CurrentWorkingDirectory.string();
}

void AppState::UpdateClusterData()
{
	Result.Clear();

	ClusterData.LoadNodeData(SearchFolder);
	ClusterData.CompareNodeData(Result);
}
