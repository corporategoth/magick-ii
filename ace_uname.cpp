#include <ace/OS.h>

int main(int argc, char *argv[])
{
    struct utsname *info = new struct utsname;
    ACE_OS::uname(info);

    if (argc < 2 || ACE_OS::strcmp(argv[1], "sysname")==0)
	printf("%s\n", info->sysname);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "nodename")==0)
	printf("%s\n", info->nodename);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "release")==0)
	printf("%s\n", info->release);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "version")==0)
	printf("%s\n", info->version);
    if (argc < 2 || ACE_OS::strcmp(argv[1], "machine")==0)
	printf("%s\n", info->machine);

    delete info;
    return 0;
}
