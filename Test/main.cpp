#include "Volt/Core/Vector2.h"
#include <gflags/gflags.h>
#include <glog/logging.h>

int main (int argc, char** argv) {
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_logtostderr = true;

	LOG(INFO) << "WASSUP";

	return 0;
}
