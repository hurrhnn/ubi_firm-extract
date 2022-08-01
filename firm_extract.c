#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DUMMY_OFFSET 0x600

int main() {
    FILE *target_fp = fopen("target_ubi.bin", "rb");
    FILE *new_fp = fopen("new_ubi.bin", "wb");

	bool is_first_sector = true;
    unsigned int cnt = 0;
    unsigned char sig[0x04];
    while ((fread(sig, 0x04, 0x01, target_fp) > 0)) {
        if(memcmp("UBI#", sig, 0x04) == 0) {
            fseek(target_fp, 0x800 - 0x04, SEEK_CUR);

            while ((fread(sig, 0x04, 0x01, target_fp) > 0)) {
                if(memcmp(sig, "UBI!", 0x04) == 0) {
                    printf("[*] UBI VID_HDR FOUND - 0x%x, offset = 0x%lx\n", ++cnt, ftell(target_fp) - 0x04);
                    fseek(target_fp, 0x200 - 0x04, SEEK_CUR);

                    unsigned char data[0x20000 - 0x1000];
                    fread(data, 0x20000 - 0x1000, 0x01, target_fp);
					if(is_first_sector)
						fwrite(data + DUMMY_OFFSET, 0x20000 - 0x1000 - DUMMY_OFFSET, 0x01, new_fp);
					else
                        fwrite(data, 0x20000 - 0x1000, 0x01, new_fp);
                }
            }
        }
    }
    fclose(target_fp);
    fclose(new_fp);
    printf("[*] Extraction Complete.\n");
    return 0;
}
