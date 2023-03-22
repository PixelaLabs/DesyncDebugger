#pragma once

#include "Cluster.h"
#include "ComparisonResult.h"

class AppState
{
public:
	AppState();

	void UpdateClusterData();

	ComparisonResult const& GetComparisonResults() const { return Result; };
	Cluster& GetClusterData() { return ClusterData; };

	std::string const& GetSearchFolder() { return SearchFolder; }
	void SetSearchFolder(std::string const& InSearchFolder) { SearchFolder = InSearchFolder; }

protected:
	std::string SearchFolder;

	ComparisonResult Result;
	Cluster ClusterData;
};
