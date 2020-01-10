#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#include "parse.c"

int main(int argc, char **argv)
{
	int 			ch;
	int 			cmd;
	void 			*arsc;
	char 			*filename;
	FILE 			*fp;
	size_t 			size;
	unsigned char 	*buf = NULL;
	
	while ((ch = getopt_long(argc, argv, "n:hHgptre", opt, NULL)) != -1)
	{
		switch (ch)
		{
			case 'h':
				print_usage(argv[0]);
				return 0;
			case 'n':
				filename = optarg;
				//printf("filename is %s\n", filename);
				break;
			case 'H':
				cmd = 1;
				break;
			case 'g':
				cmd = 2;
				break;
			case 'p':
				cmd = 3;
				break;
			case 't':
				cmd = 4;
				break;
			case 'r':
				cmd = 5;
				break;
			case 'e':
				cmd = 6;
				break;
			default:
				print_usage(argv[0]);
				return 0;
		}
	}

	if (argc < 2)
	{
		print_usage(argv[0]);
		return 0;
	}
	
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("ERR:open file failure %s\n",strerror(errno));
		return -1;
	}
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	printf("data size is %zu\n", size);
	fseek(fp, 0, SEEK_SET);//文件指针指向文件开头
	
	buf = (unsigned char*)malloc(size);
	if (NULL == buf)
	{
		printf("ERR: buf malloc failure:%s\n", strerror(errno));
		fclose(fp);
		return -1;
	}
	
	if ( (fread(buf, 1, size, fp)) != size)
	{
		printf("ERR: read file failure；%s\n", strerror(errno));
		free(buf);
		fclose(fp);
		return -1;
	}
	
	arsc = arsc_Open(buf, size);
	
	if (NULL == arsc)
	{
		printf("ERR: arsc_Open failure\n");
		free(buf);
		fclose(fp);
		return -1;
	}

	handle_cmd(cmd, arsc, fp);
	//print_ResTable_Header(arsc);
	//print_ResStringPool_Header(arsc);
	//print_ResTable_Package(arsc);
	//print_ResTable_typeSpec(arsc, fp);

	free(buf);
	free(arsc);
	fclose(fp);
	
	return 0;
}


