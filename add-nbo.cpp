#include <iostream>
#include <unistd.h>
#include <string>
#include <stdint.h>
#include <netinet/in.h>


uint32_t file_open(std::string filename){
    FILE* fp = fopen(filename.c_str(), "rb");
    unsigned char* buffer;
    int size = 4;
    uint32_t num;

    if (fp != NULL){
        buffer = (unsigned char*)malloc(sizeof(char)*size);

        int file_size = fread(buffer, 1, size, fp);
        if (file_size == 4){
            uint32_t* p = reinterpret_cast<uint32_t*>(buffer);
            num = htonl(*p);
        }
        else{
            printf("Please check file size: 4 byte only\n");

            return NULL;
        }
    }
    else{
        std::cout << "ERROR when opening the file named " << filename << std::endl;
    }

    fclose(fp);
    free(buffer);

    return num;
}


int main(int argc, char *argv[]){
    if ((argv[1]!=NULL) && (argv[2]!=NULL)){
        char current_path[1024];
        getcwd(current_path, sizeof(current_path));

        std::string basic_path = current_path;

        std::string first_file = basic_path + "/" + argv[1];
        std::string second_file = basic_path + "/" + argv[2];

        //std::cout << "first file path: " << first_file
        //         << ", second file path: " << second_file << std::endl;

        uint32_t first_num = file_open(first_file);
        uint32_t second_num = file_open(second_file);

        if ((first_num == NULL) || (second_num == NULL)){
            if (first_num == NULL){
                std::cout << "You need to check the file " << first_file << std::endl;
            }
            if (second_num == NULL){
                std::cout << "You need to check the file " << second_file << std::endl;
            }
            return 0;
        }
        uint32_t result = first_num + second_num;

        printf("%d(0x%x) + %d(0x%x) = %d(0x%x)\n", first_num, first_num,
                                         second_num, second_num,
                                         result, result);
    }
    else{
        printf("To execute this program, input 2 files like \'./add-nbo FILENAME_1 FILENAME_2\'\n");
    }
}
