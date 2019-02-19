

#include <linux/string.h> // present
#include <linux/syscalls.h> //present
#include <linux/kernel.h> //present   
#include <linux/fcntl.h> // present
#include <linux/errno.h> // present
#include <linux/unistd.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#define BUFSZ 300

/* get_memory_usage(void) method returns -1 if error occurs. Otherwise return currently using memory.*/


int atoi_(char input[100]);
char kfgetc(struct file *f);
 
asmlinkage int get_memory_usage(void){
    
  
    char location[200]="/proc/self/status";
   
    printk("Signal OK\n");
    
    int res = -1;
    int mem= 0;

    char line[BUFSZ];

    mm_segment_t oldfs;
    oldfs= get_fs();
    set_fs(get_ds());
	
    struct file * fd = filp_open(location,O_RDONLY, 0); // file open
    
    if ( fd==NULL) {
    	printk("Error in loading file.\n");
    	return -1;
    }
     /* else */	
    int done = 0;
    while(!done){
	char chr=0;
	int i =0;
	chr = kfgetc(fd);
    	while (chr != '\n' && chr != -1){
		line[i]= chr;
		i++;
		chr= kfgetc(fd);
	}
	line[i]='\0';
	if (chr == -1){
	   done= 1;
	}
	if(strncmp(line, "VmRSS:", 6) == 0) { // check for VmRSS line of the file
            		mem = (int) strlen(line);
           		char* p = line;
           		while(*p < '0' || *p > '9')p++;//extracting ram memroy use from the line
            		line[mem-3] = '\0';
           		mem = (int) atoi_(p);
			printk("VmRSS located...\n");
	   		done= 1; // loop exit condition
	}
		
	
	
    }
    //closing file
    set_fs(oldfs);
    filp_close (fd,NULL);

    printk("Finish job\n");
    res= mem;
    printk("Current Memory Use by process is: %d\n",res);
    return res;
    
}

/* read file character by character */
char kfgetc(struct file *f) {
   char chr = 0;

   f->f_op->read(f, &chr, 1, &f->f_pos);

   return chr;
}

int atoi_(char input[100])
{
    int number = 0;
    int neg = input[0] == '-';
    int i = neg ? 1 : 0;
    while ( input[i] >= '0' && input[i] <= '9' )
    {
      number *= 10;             // multiply number by 10
      number += input[i] - '0'; // convet ASCII '0'..'9' to digit 0..9 and add it to number           
      i ++;                     // step one digit forward
    }
    if ( neg )       number *= -1;
    return number;
   
}


