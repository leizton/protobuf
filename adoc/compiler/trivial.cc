// 获取当前程序的运行目录的绝对路径
// #include <unistd.h>
bool GetRuntimeAbsolutePath(std::string& path) {
  char buffer[PATH_MAX];
  int len = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (len <= 0) {
    return false;
  }
  path.assign(buffer, len);
  size_t pos = path.find_last_of("/\\");
  if (pos == std::string::npos || pos == 0) {
    return true;
  }
  path = path.substr(0, pos);
  return true;
}
