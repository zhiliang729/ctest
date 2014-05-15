#include  <sys/types.h>
#include  <sys/statfs.h>


int main(int argc, char **argv)
{
	struct statfs buf;
	if (argc != 2) return 1;
	statfs(argv[1], &buf, sizeof(buf), 0);
	printf("FILE SYSTEM:\ntype=%d\n", buf.f_fstyp);
	printf("bsize=%d\n", buf.f_bsize);    
	printf("blocks=%d\n",buf.f_blocks);    
	printf("bfee=%d\n",  buf.f_bfree);    
	printf("files=%d\n", buf.f_files);    
	printf("ffree=%d\n", buf.f_ffree);   
	printf("%s   : %d blocks  %d i-nodes\n",  argv[1], buf.f_bfree, buf.f_ffree);
	return 0; 
}
