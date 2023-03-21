#include "AppState.h"

void AppState::UpdateClusterData(std::string const& FolderPath)
{
	Result.Clear();

	ClusterData.LoadNodeData();
	ClusterData.CompareNodeData(Result);
}
