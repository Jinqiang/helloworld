#include <iostream.h>
using namespace std;
main()
{
    cout << "Hello World!";
	for(int i = 0; i < 10 ; ++i )
	{
		cerr<< " test cerr" << i << endl;
		cout<< " test cout" << 2*i << endl;
	}
    return 0;
}