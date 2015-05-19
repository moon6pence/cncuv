#include "cnc.h"

#include <sstream>

namespace CnC {

void setThreadPoolSize(int n)
{
	std::ostringstream _oss;
	const char * _var = "UV_THREADPOOL_SIZE";
#ifdef _WIN32
	_oss << _var << "=" << n;
	_putenv(_oss.str().c_str());
#else
	_oss << n;
	setenv(_var, _oss.str().c_str(), 1);
#endif
}

} // namespace CnC