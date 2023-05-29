#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
typedef struct posix_header{

	char name[100];
	char mode[8];
    char uid[8];
    char gid[8];
	char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char additional[12];

}Posix;

void I_Prints(char* str){

    for(int i = 0; str[i]; i++){
        write(1, &str[i], 1);
    }
    write(1, "\n", 1);
}                                                                                                                                                                     

int I_StrCmp(char* sing1, char* sing2){
    int i=0;
    for(;sing1[i]==sing2[i]&&sing1[i]!='\0'&&sing2[i]!='\0';i++);
    
    return sing1[i]-sing2[i];
}

char* I_StrCpy(char* sing1, char* sing2){
    
    for(int i=0;sing2[i]!='\0';i++){
    
    sing1[i]=sing2[i];
    }
    return sing1;
}

void I_FilNull(char* content, int size){
    
    for(int i = 0; i < size; i++){
        content[i] = 0;
    }
}

void I_FillStr(char* content){

    for(int i = 0; content[i]; i++){
        content[i] = 0;
    }
}

int I_Strlen(char* c){
    int j = 0;
    for (int i=0;c[i];i++){
        j+=c[i];
    }
    return j;
}

int I_InterPow(int count1, int count2){
    int res = 1;
    for(int i = 0; i < count2; i++){
        res *= count1;
    }
    return res;
}

int I_OcToDec(char* str_num){
    int n = atoi(str_num);
    int p = 0, decada = 0, r;

    while(n > 0){
        r = n % 10;
        n = n / 10;
        decada = decada + r * I_InterPow( 8 , p );
        ++p;
    }
    
    return decada;
}

int I_CHkSum(Posix* text){
    int joint = 0;
joint += I_Strlen(text->name);
joint += I_Strlen(text->mode);
joint += I_Strlen(text->uid);
joint += I_Strlen(text->gid);
joint += I_Strlen(text->size);
joint += I_Strlen(text->mtime);
joint += I_Strlen(&text->typeflag);
joint += I_Strlen(text->magic);
joint += I_Strlen(text->uname);
joint += I_Strlen(text->gname);
joint += 256;
return joint;
}

char* I_octaL(int num, int size){
    char* place = (char*)malloc(sizeof(char)* 20);
    char* str = (char*)malloc(sizeof(char)* 20);
    int j  = 0, l = 0;
    while(num){
        place[j++] = num % 8 + '0';
        num = num / 8;
    }
    while(size){
        if(j < size){
            str[l++]= '0';
            j++;
        }
        size--;
    }
    for(int i = strlen(place)-1; i >= 0; i--){                                                                                                                                                                                                   //yo
        str[l++] = place[i];
    }
    return str;
}

Posix* I_file_0(Posix* tar){

    I_FilNull(tar->mode, sizeof(tar->mode) / sizeof(char));
    I_FilNull(tar->name, sizeof(tar->name) / sizeof(char));

    I_FilNull(tar->uid, sizeof(tar->uid) / sizeof(char));
    I_FilNull(tar->gid, sizeof(tar->gid) / sizeof(char));
    
    I_FilNull(tar->size, sizeof(tar->size) / sizeof(char));
    I_FilNull(tar->mtime, sizeof(tar->mtime) / sizeof(char));
    
    I_FilNull(tar->chksum, sizeof(tar->chksum) / sizeof(char));
    tar->typeflag = 0;
    
    I_FilNull(tar->linkname, sizeof(tar->linkname) / sizeof(char));
    I_FilNull(tar->magic, sizeof(tar->magic) / sizeof(char));
    
    I_FilNull(tar->version, sizeof(tar->version) / sizeof(char));
    I_FilNull(tar->uname, sizeof(tar->uname) / sizeof(char));
    
    I_FilNull(tar->gname, sizeof(tar->gname) / sizeof(char));
    I_FilNull(tar->devmajor, sizeof(tar->devmajor) / sizeof(char));
    
    I_FilNull(tar->devminor, sizeof(tar->devminor) / sizeof(char));
    I_FilNull(tar->prefix, sizeof(tar->prefix) / sizeof(char));
    
    I_FilNull(tar->additional, sizeof(tar->additional) / sizeof(char));
    return tar;
}

void I_f_tar(Posix* fst, char* av){
    struct stat fs;
    stat(av, &fs);
    
    I_StrCpy(fst->name, av);
    I_StrCpy(fst->mode, I_octaL(fs.st_mode, 7));
    I_StrCpy(fst->uid, I_octaL(fs.st_uid, 7));

    I_StrCpy(fst->gid, I_octaL(fs.st_gid, 7));
    I_StrCpy(fst->size, I_octaL(fs.st_size, 11));
    I_StrCpy(fst->mtime, I_octaL(fs.st_mtime, 11));
    
    I_StrCpy(&fst->typeflag , "0");
    I_StrCpy(fst->magic, "ustar  ");
    I_StrCpy(fst->uname, "docode");
    
    I_StrCpy(fst->gname, "docode");
    I_StrCpy(fst->chksum, I_octaL(I_CHkSum(fst), 6));
}

char* file0(char* av){
    struct stat filestat;
    stat(av, &filestat);
    char* place = (char*)malloc(sizeof(char)*512);
    I_FillStr(place);
    int fd = open(av, O_RDONLY);
    read(fd, place, filestat.st_size);
    return place;
}

void I_arxive_f_name(char* fl){
    Posix file_tar;
    int fd = open(fl, O_RDONLY);
    while(read(fd, &file_tar, 512) > 0){
        if(I_StrCmp(file_tar.magic, "ustar  ")==0){
            I_Prints(file_tar.name);
        }
    }
}

void I_Creat(int ac, char** av){
    creat(av[2], 000644);
    struct stat fstat;
    for(int i = 3; i < ac; i++){
        if(open(av[i], O_RDONLY) < 0){
            fprintf(stderr, "my_tar: %s: Cannot stat: No such file or directory\n", av[i]);
        }
        int fd = open(av[2],  O_WRONLY | O_APPEND );
        Posix tar;
        I_file_0(&tar);
        I_f_tar(&tar, av[i]);
        write(fd, &tar, 512);
        char* node = file0(av[i]);
        write(fd, node, 512);
        close(fd);
        stat(av[i], &fstat);
    }
}

void I_arxive_file(char** av){
    Posix len_tar;
    I_file_0(&len_tar);
    I_f_tar(&len_tar, av[3]);
    int fd = open(av[2], O_WRONLY | O_APPEND);
    write(fd, &len_tar, 512);
    char* str = file0(av[3]);
    write(fd, str, 512); 
    close(fd);
}

void I_arxive_uid(char* av){
    int fd = open(av, O_RDONLY);
    Posix tar;
    while( ( read(fd, &tar, 512) ) > 0){
        if(I_StrCmp(tar.magic, "ustar  ")==0){
            creat(tar.name, I_OcToDec(tar.mode));
            int td = open(tar.name, O_WRONLY | O_APPEND);
            int j  = I_OcToDec(tar.size);
            int file_size = j, l = 1 ;
            char* fl_content = (char*)malloc(sizeof(char)*j);
            while(file_size > 0){
                read(fd, fl_content, 1);
                write(td, fl_content, 1);
                file_size--;
            }
            close(td);
            l *= 512;
            l -= j;
            lseek(fd, l, SEEK_CUR);
        }
    }
    close(fd);
}                                         

void I_check_utar(char** av){
    int i = 3, j = 0;
    Posix chesk_un;
    struct stat ft;
    int fd = open(av[2], O_RDONLY);
    while ((read(fd, &chesk_un, 512)) > 0) {
        stat(av[i++], &ft);
        if(I_StrCmp(chesk_un.name,av[i])==0&&I_StrCmp(chesk_un.magic,"ustar  ")==0&&I_StrCmp(chesk_un.mtime,I_octaL(ft.st_mtime,12))==0){
            j++;

        }
    }
    close(fd);
}



int main(int argc, char** argv){
    char flag;
    int c=0,t=0,u=0,r=0,x=0; 
    for(int i = 1; i < argc; i++){
        if(I_StrCmp(argv[i], "-cf")==0){
            c =1;
        }
        else if(I_StrCmp(argv[i], "-tf")==0){
            t=1;;
        }
        else if(I_StrCmp(argv[i], "-uf")==0){
            u=1;
        }
        else if(I_StrCmp(argv[i], "-rf")==0){
            r=1;
        }
        else if(I_StrCmp(argv[i], "-xf")==0){
            x=1;
        }
    }
    if(c==1){
        flag = 'c';
    }else if(t==1) {
        flag = 't';
    }else if(u==1) {
        flag = 'u';
    }else if(r==1) {
        flag = 'r';
    }else if(x==1) {
        flag = 'x';
    }
    if(flag == 'c')
    {I_Creat(argc, argv);}
    else if(flag == 't') 
    {I_arxive_f_name(argv[2]);}
    else if(flag == 'r') 
    {I_arxive_file(argv);}
    else if(flag == 'x') 
    {I_arxive_uid(argv[2]);}
    else if(flag == 'u')
    {I_check_utar(argv);}
    
    return 0;
}
