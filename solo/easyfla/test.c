#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *input = argv[1];
    if (strlen(input) != 36) {
        printf("Try harder!\n");
        return 0;
    }
    int success = 0;
    int tmp;
tmp = input[32];
tmp += 23;
tmp *= 3;
tmp -= 44;
if (tmp == 187) success += 1;
tmp = input[15];
tmp += 93;
tmp -= 77;
tmp -= 96;
if (tmp == -24) success += 1;
tmp = input[12];
tmp += 80;
tmp += 65;
tmp -= 32;
if (tmp == 170) success += 1;
tmp = input[33];
tmp += 86;
tmp += 99;
tmp *= 79;
if (tmp == 22673) success += 1;
tmp = input[2];
tmp *= 14;
tmp += 6;
tmp *= 37;
if (tmp == 26640) success += 1;
tmp = input[11];
tmp += 5;
tmp -= 88;
tmp += 43;
if (tmp == 59) success += 1;
tmp = input[10];
tmp += 43;
tmp += 52;
tmp *= 52;
if (tmp == 10244) success += 1;
tmp = input[20];
tmp -= 54;
tmp -= 32;
tmp -= 9;
tmp += 29;
if (tmp == 32) success += 1;
tmp = input[14];
tmp -= 7;
tmp += 81;
tmp *= 88;
tmp -= 13;
if (tmp == 11075) success += 1;
tmp = input[19];
tmp *= 27;
tmp -= 30;
tmp -= 3;
tmp += 44;
tmp *= 86;
if (tmp == 130978) success += 1;
tmp = input[28];
tmp -= 62;
tmp += 8;
tmp -= 44;
tmp += 2;
if (tmp == -43) success += 1;
tmp = input[30];
tmp *= 22;
tmp -= 34;
tmp += 13;
if (tmp == 1079) success += 1;
tmp = input[35];
tmp *= 19;
tmp -= 40;
tmp *= 82;
tmp -= 50;
if (tmp == 154028) success += 1;
tmp = input[0];
tmp -= 49;
tmp *= 78;
tmp *= 28;
tmp *= 67;
if (tmp == 7755384) success += 1;
tmp = input[23];
tmp += 62;
tmp -= 32;
tmp += 94;
tmp *= 30;
if (tmp == 5070) success += 1;
tmp = input[13];
tmp += 48;
tmp -= 39;
tmp += 60;
if (tmp == 114) success += 1;
tmp = input[21];
tmp *= 19;
tmp -= 23;
tmp += 28;
tmp -= 88;
if (tmp == 848) success += 1;
tmp = input[24];
tmp -= 25;
tmp += 75;
tmp += 41;
if (tmp == 188) success += 1;
tmp = input[16];
tmp *= 66;
tmp -= 77;
tmp *= 35;
tmp += 7;
if (tmp == 230622) success += 1;
tmp = input[9];
tmp += 94;
tmp -= 6;
tmp *= 99;
if (tmp == 14355) success += 1;
tmp = input[8];
tmp -= 94;
tmp += 57;
tmp -= 69;
tmp *= 14;
if (tmp == -854) success += 1;
tmp = input[5];
tmp *= 22;
tmp -= 87;
tmp -= 7;
if (tmp == 1072) success += 1;
tmp = input[18];
tmp -= 28;
tmp += 36;
tmp += 65;
tmp -= 52;
tmp -= 21;
if (tmp == 45) success += 1;
tmp = input[3];
tmp += 74;
tmp -= 88;
tmp *= 29;
tmp += 35;
if (tmp == 2471) success += 1;
tmp = input[4];
tmp *= 45;
tmp *= 40;
tmp -= 82;
if (tmp == 95318) success += 1;
tmp = input[7];
tmp += 38;
tmp -= 85;
tmp *= 3;
tmp += 16;
if (tmp == 37) success += 1;
tmp = input[6];
tmp += 40;
tmp += 23;
tmp += 81;
if (tmp == 243) success += 1;
tmp = input[31];
tmp += 6;
tmp += 79;
tmp += 59;
if (tmp == 200) success += 1;
tmp = input[34];
tmp -= 16;
tmp -= 89;
tmp += 29;
tmp -= 23;
if (tmp == -43) success += 1;
tmp = input[1];
tmp *= 73;
tmp -= 18;
tmp *= 19;
if (tmp == 135584) success += 1;
tmp = input[17];
tmp += 68;
tmp -= 96;
tmp -= 85;
tmp -= 26;
tmp -= 24;
if (tmp == -64) success += 1;
tmp = input[27];
tmp += 66;
tmp -= 18;
tmp += 29;
tmp -= 38;
tmp += 23;
if (tmp == 162) success += 1;
tmp = input[22];
tmp += 100;
tmp += 85;
tmp *= 74;
if (tmp == 17612) success += 1;
tmp = input[25];
tmp -= 97;
tmp += 55;
tmp *= 86;
tmp += 97;
tmp += 92;
if (tmp == 5349) success += 1;
tmp = input[26];
tmp -= 17;
tmp *= 83;
tmp *= 54;
tmp += 69;
tmp += 54;
if (tmp == 372129) success += 1;
tmp = input[29];
tmp -= 30;
tmp -= 62;
tmp -= 48;
tmp += 29;
tmp -= 32;
if (tmp == -43) success += 1;
    if (success == 36) {
        printf("Your flag is flag{%s}\n", input);
    } else {
        printf("Try harder!\n");
    }
    return 0;
}