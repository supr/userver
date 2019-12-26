#include <iostream>

#include "liburing.h"

int main(int argc, char** argv)
{
	std::cout << "Hello World!" << std::endl;

	struct io_uring ring;
	::io_uring_queue_init(10, &ring, 0);
	::io_uring_queue_exit(&ring);

	return 0;
}
