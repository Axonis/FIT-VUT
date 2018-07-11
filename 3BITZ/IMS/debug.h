#ifndef DEBUG
#define DEBUG

    #ifdef ECHO

    #include <iostream>

    #define echo(a) std::cout << __FILE__ << " : " << __LINE__ << " : " << __FUNCTION__ << " >> "<< (a) << std::endl;

	#else

	#define echo(a)

    #endif //SERVER_LOGGING
#endif //DEBUG