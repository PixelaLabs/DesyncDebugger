#pragma once

#include "Common.h"

#include "Cluster.h"
#include "ComparisonResult.h"

class AppState
{
public:
	void UpdateClusterData(std::string const& FolderPath);

	ComparisonResult const& GetComparisonResults() const { return Result; };
	Cluster& GetClusterData() { return ClusterData; };
protected:
	ComparisonResult Result;
	Cluster ClusterData;
};
