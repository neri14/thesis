#ifndef COMMON_SERVICE_H
#define COMMON_SERVICE_H

namespace common {

class common_service
{
public:
	common_service()
	{}

	virtual ~common_service()
	{}

	virtual int start() = 0;
};

}

#endif /* COMMON_SERVICE_H */
