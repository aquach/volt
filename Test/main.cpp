#include "Volt/Core/Core.h"
#include <gflags/gflags.h>
#include "Volt/Assets/DirectoryDataSource.h"

int main (int argc, char** argv) {
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_logtostderr = true;

	Volt::DataSource* source = new Volt::DirectoryDataSource("../Data");
	Volt::DataItem item;
	bool loaded = source->LoadDataItem("item.txt", &item);

	return 0;
}
