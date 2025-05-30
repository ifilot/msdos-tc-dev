#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define VIDEO_INT 0x10
#define MODE_13H  0x13
#define TEXT_MODE 0x03
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define VGA_SEGMENT 0xA000
#define IMAGE_SIZE 64000

void set_video_mode(unsigned char mode) {
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

int main() {
    FILE *file;
    unsigned char *buffer;
    unsigned char far* vga = (unsigned char far*)MK_FP(VGA_SEGMENT, 0);
    unsigned int i;

    buffer = (unsigned char *)malloc(IMAGE_SIZE);
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    file = fopen("VGATEST.BIN", "rb");
    if (file == NULL) {
        printf("Could not open VGATEST.BIN\n");
        free(buffer);
        return 1;
    }

    fread(buffer, 1, IMAGE_SIZE, file);
    fclose(file);

    set_video_mode(MODE_13H);

    for (i = 0; i < IMAGE_SIZE; ++i) {
        vga[i] = buffer[i];
    }

    getch();

    set_video_mode(TEXT_MODE);
    free(buffer);

    return 0;
}